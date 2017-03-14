#ifndef WBCODEVIEW_H
#define WBCODEVIEW_H

#include "includes.h"
#include "wbcodecompletionmodel.h"

//added in (03/10/17) <- code completion
class WBCodeView : public QPlainTextEdit {
    Q_OBJECT
public:
    WBCodeView( QCompleter *_completer = 0, bool caseSensitive = false ,QWidget *parent = 0 );
    QCompleter *completer() const;

    void showCodeCompletion();
    void unselectText();

    QString selectCurrentLine();
    QString selectLine( int line );

    void setCompleter( QCompleter *completer, bool caseSensitive );
    void insertAtCursorPosition( const QString &text );
    void setModel( WBCodeCompletionModel *model );
private:
    QCompleter *_completer;
    QString editor_cursor_text();
    QString completion_highlighted;
    WBCodeCompletionModel *model;
    int getTabCount( );

private slots:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void stringChanged( const QString &string );
    void completerClicked();

protected:
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void handleComplete( QString text );
};

#endif // WBCODEVIEW_H
