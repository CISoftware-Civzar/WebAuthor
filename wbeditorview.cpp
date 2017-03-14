#include "wbeditorview.h"

WBEditorView::WBEditorView(WBCodeView *codeView, bool lineNumbers, QWidget *parent)
    : QWidget(parent)
{
   layoutNumber = new QHBoxLayout( );
   layoutNoNumber = new QHBoxLayout( );
   lineNumberView = new QTextBrowser( );

   //set up line number style
   lineNumberView->setStyleSheet( "background: #eee; color: #333;" );
   lineNumberView->setMinimumWidth( 50 );
   lineNumberView->setMaximumWidth( 50 );
   lineNumberView->verticalScrollBar()->setVisible( false );
   lineNumberView->horizontalScrollBar()->setVisible( false );

   this->codeView = codeView;

   layoutNumber->addWidget( lineNumberView );
   layoutNumber->addWidget( this->codeView, 40 );

   layoutNoNumber->addWidget( this->codeView );

   setupLineNumber();

   connect( this->codeView, SIGNAL(textChanged()), SLOT(codeViewChanged()) );
   connect( this->codeView->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(scrollPositionChanged(int)) );

   //this->setLayout( (lineNumbers ? layoutNumber : layoutNoNumber ) );
   this->setLayout( layoutNoNumber );
   this->_lineNumberShown = lineNumbers;
}

int WBEditorView::getLineCount(QString source) {
    int count = source.count( "\n" );
    return count;
}

void WBEditorView::setCodeView(WBCodeView *newCodeView) {
    this->codeView = newCodeView;
}

void WBEditorView::setLineNumber(bool active) {
    this->_lineNumberShown = active;

    //update layout
    this->setLayout( (active ? layoutNumber : layoutNoNumber ) );
}

void WBEditorView::setWordWrap(bool wraps) {
    this->_word_wraps = wraps;
}

void WBEditorView::refreshLineNumbers() {
    if ( _lineNumberShown ) this->setupLineNumber( false );
}

void WBEditorView::setupLineNumber( bool shouldCheck ) {
    if ( _lineNumberShown ) {
        int end = getLineCount( codeView->toPlainText() );

        if ( end == _currentLength && shouldCheck ) return;
        const int textSize = 18;

        //(03/13/17)
        //draw range = start -- > end
        //start = scrollbar position
        //soft end = widget height / text size
        //end = number of lines in editor

        int start = _startPos;
        int softend = start + ( codeView->height() / textSize ) - 1;

        lineNumberView->setPlainText( "" );

        //dynamically adjust the width of the line number widget
        int end_place = 0;

        int s = 11; //scaling factor
        int l = 0; //limit to scaling

        if ( end < 100 ) end_place = 100, l = 0;
        else if ( end >= 100 ) end_place = 1000, l = 4;
        else if ( end >= 1000 ) end_place = 10000, l = 8;

        lineNumberView->setMinimumWidth( 50 + (((end)/end_place) * ( s - l ) ) );
        lineNumberView->setMaximumWidth( 50 + (((end)/end_place) * ( s - l ) ) );

        //show line numbers again
        for ( int i = start; i < ( softend < end ? softend : end ) + 1; ++i ) {
            QString current_line = QString( (i == start ? "%1" : "%1\n") ).arg( lineNumberView->toPlainText() );
            current_line.append( QString( "%1" ).arg( i + 1 ) );

            //calculate wrap
            if ( _word_wraps ) {
                QString curline = codeView->selectLine( i );
                int nLinesWrapped = 0;

                for ( int i = 0; i < nLinesWrapped; ++i ) {
                    current_line.append( "\n" );
                }
            }

            lineNumberView->setPlainText( current_line );
        }

        _currentLength = getLineCount( codeView->toPlainText() );
    }
}

void WBEditorView::codeViewChanged() {
    if ( _lineNumberShown ) setupLineNumber();
}

void WBEditorView::scrollPositionChanged(int value) {
    if ( _lineNumberShown ) {
        _startPos = value;
        setupLineNumber( false );
    }
}
