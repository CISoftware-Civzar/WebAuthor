#ifndef WBJSSYNTAX_H
#define WBJSSYNTAX_H

#include "includes.h"

class WBJSSyntax : public QSyntaxHighlighter {
    Q_OBJECT
public:
    WBJSSyntax( QTextDocument *parent = 0 );
    QTextCharFormat types[5];

protected:
    void highlightBlock( const QString &text );

    enum WBState {
        StateIdle = -1,
        StateComment,
        StateString,
        StateKey
    };
};

#endif // WBJSSYNTAX_H
