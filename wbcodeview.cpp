#define WB_DEBUG_MODE
#include "wbcodeview.h"

WBCodeView::WBCodeView( QCompleter *_completer, bool caseSensitive, QWidget *parent )
    : QPlainTextEdit( parent ), _completer(0)
{
    this->setCompleter( _completer, caseSensitive );
    connect( _completer, SIGNAL(highlighted(QString)), SLOT(stringChanged(QString)) );
    connect( _completer, SIGNAL(activated(QString)), SLOT(completerClicked()) );
}

void WBCodeView::keyPressEvent(QKeyEvent *event) {
    if ( _completer->popup()->isVisible() ) {
        switch ( event->key() ) {
            case Qt::Key_Enter:
            case Qt::Key_Return:

                handleComplete( completion_highlighted );
                _completer->popup()->setVisible( false );
            break;

            case Qt::Key_Escape:
            case Qt::Key_Backspace:
            case Qt::Key_Delete:
                _completer->popup()->close();
            break;
        default:
            _completer->setCompletionPrefix( editor_cursor_text() );
            QPlainTextEdit::keyPressEvent( event );
            break;
     }
   } else {
        //(03/13/17) Auto indentation
        int tabCount = 0;
        if ( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter ) {
            tabCount = getTabCount();
        }
        QPlainTextEdit::keyPressEvent( event );

        /*if ( editor_cursor_text().length() > 3 ) {
            if ( !_completer->popup()->isVisible() ) {
                showCodeCompletion();
            }
        }*/

        if ( event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter ) {
            QString tabs = "";

            for ( int i = 0; i < tabCount; ++i ) {
                tabs.append( "\t" );
            }

            this->insertAtCursorPosition( tabs );
        }
    }
}

int WBCodeView::getTabCount() {
    return this->selectCurrentLine().count( "\t" );
}

void WBCodeView::setCompleter(QCompleter *completer, bool caseSensitive ) {
    _completer = completer;

    _completer->setCompletionMode( QCompleter::PopupCompletion );
    _completer->setCaseSensitivity( ( !caseSensitive ? Qt::CaseInsensitive : Qt::CaseSensitive ) );
    _completer->setCompletionRole( Qt::DisplayRole );
    _completer->setMaxVisibleItems( 10 );

    _completer->setWidget( this );
}

void WBCodeView::stringChanged(const QString &string) {
    completion_highlighted = string;
}

void WBCodeView::insertAtCursorPosition( const QString &text ) {
    QTextCursor editor_cursor = textCursor();
    editor_cursor.insertText( text );
    setTextCursor( editor_cursor );
}

QString WBCodeView::selectCurrentLine() {
    QTextCursor editor_cursor = textCursor();
    editor_cursor.select( QTextCursor::LineUnderCursor );
    return editor_cursor.selectedText();
}

QString WBCodeView::selectLine(int line) {
    QStringList lines = this->toPlainText().split( "\n" );
    return lines[ line ];
}

void WBCodeView::setModel(WBCodeCompletionModel *model) {
    this->model = model;
    this->_completer->setModel( model );
}

void WBCodeView::completerClicked() {
    handleComplete( completion_highlighted );
    _completer->popup()->setVisible( false );
}

QCompleter *WBCodeView::completer() const { return this->_completer; }

void WBCodeView::focusInEvent(QFocusEvent *event) {
    if ( _completer ) _completer->setWidget( this );
    QPlainTextEdit::focusInEvent( event );
}

void WBCodeView::handleComplete(QString text) {
    QTextCursor editor_cursor = textCursor();

    if ( editor_cursor_text().length() > 0 ) {
        editor_cursor.movePosition( QTextCursor::Left );
        editor_cursor.movePosition( QTextCursor::EndOfWord );
    }

    int delta = abs( text.length() - _completer->completionPrefix().size() ) +
                        ( _completer->completionPrefix().size() >= 3 ? -1 : 0 );

    editor_cursor.insertText( text.right( delta ) );
    setTextCursor( editor_cursor );
    _completer->setCompletionPrefix( "" );
}

void WBCodeView::unselectText() {
    QTextCursor editor_cursor = textCursor();
    editor_cursor.movePosition( QTextCursor::Left );
    editor_cursor.movePosition( QTextCursor::Right );
    setTextCursor( editor_cursor );
}

QString WBCodeView::editor_cursor_text() {
    QTextCursor editor_cursor = textCursor();
    editor_cursor.select( QTextCursor::WordUnderCursor );
    return editor_cursor.selectedText();
}

void WBCodeView::showCodeCompletion() {
    QRect rect = cursorRect();
    rect.setWidth( this->width() - 100 );
    _completer->setCompletionPrefix( editor_cursor_text() );
    _completer->complete(rect);
}
