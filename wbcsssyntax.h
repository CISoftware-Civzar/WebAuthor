#ifndef WBCSSSYNTAX_H
#define WBCSSSYNTAX_H

#include "includes.h"

class WBCSSSyntax : public QSyntaxHighlighter {
    Q_OBJECT
public:
    WBCSSSyntax( QTextDocument *parent = 0 );
    QTextCharFormat types[3];

protected:
    void highlightBlock( const QString &text );

    enum WBState {
        StateIdle = -1,
        StateComment,
        StateString,
        StateKey
    };
};

#endif // WBCSSSYNTAX_H
