#ifndef WBHTMLSYNTAX_H
#define WBHTMLSYNTAX_H

#include "includes.h"

class WBHTMLSyntax : public QSyntaxHighlighter {
    Q_OBJECT
public:
    WBHTMLSyntax( QTextDocument *parent = 0 );
    QTextCharFormat types[4];

protected:
    void highlightBlock( const QString &text );
    enum WBState {
        StateIdle = -1,
        StateComment,
        StateTag,
        StateEntity,
        StateString
    };
};
#endif // WBHTMLSYNTAX_H
