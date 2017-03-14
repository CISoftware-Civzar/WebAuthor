#ifndef WBEDITORVIEW_H
#define WBEDITORVIEW_H

#include "includes.h"
#include "wbcodeview.h"

class WBEditorView : public QWidget
{
    Q_OBJECT
public:
    explicit WBEditorView( WBCodeView *codeView, bool lineNumbers = true, QWidget *parent = 0);
    QTextBrowser *lineNumberView;
    WBCodeView *codeView;

    void setCodeView( WBCodeView *newCodeView );
    void setLineNumber( bool active );
    void setWordWrap( bool wraps );
    void refreshLineNumbers();

private slots:
    int getLineCount( QString source );
    void setupLineNumber( bool shouldCheck = true );
    void codeViewChanged();
    void scrollPositionChanged( int value );

private:
    int _startPos = 0;
    int _currentLength = 0;
    bool _lineNumberShown = false;
    bool _word_wraps = false;
    QHBoxLayout *layoutNumber;
    QHBoxLayout *layoutNoNumber;
};

#endif // WBEDITORVIEW_H
