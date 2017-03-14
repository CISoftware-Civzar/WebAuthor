#include "wbcsssyntax.h"

WBCSSSyntax::WBCSSSyntax( QTextDocument *parent )
    : QSyntaxHighlighter( parent )
{
    QTextCharFormat keyFormat;
    keyFormat.setForeground( SYNTAX_COLOR_MAIN );
    keyFormat.setFontWeight( QFont::Bold );
    types[ 0 ] = keyFormat;
    rehighlight();

    QTextCharFormat CommentFormat;
    CommentFormat.setForeground( SYNTAX_COLOR_COMMENT );
    CommentFormat.setFontItalic( true );
    CommentFormat.setFontWeight( QFont::Bold );
    types[ 1 ] = CommentFormat;
    rehighlight();

    QTextCharFormat StringFormat;
    StringFormat.setForeground( SYNTAX_COLOR_STRING );
    StringFormat.setFontWeight( QFont::Bold );
    types[ 2 ] = StringFormat;
    rehighlight();
}

void WBCSSSyntax::highlightBlock(const QString &text) {
    int length = text.length();
    int state = StateIdle;
    int start = 0;

    bool initialString = true; //for string syntax highlighting

    for ( int i = 0; i < length; ++i ) {
        QChar currentCharacter = text.at( i );

        if ( text.mid(  i, 10) == "@font-face" ) {
            setFormat( i, 10, types[0] );
            i += 10;
        }

        if ( text.mid(  i, 6) == "@media" ) {
            setFormat( i, 6, types[0] );
            i += 6;
        }

        if ( text.mid(  i, 7) == "@import" ) {
            setFormat( i, 7, types[0] );
            i += 7;
        }

        if ( currentCharacter == ' '  ) {
            if ( state == StateIdle ) {
                state = StateKey;
                start = i;
            }
        }

        if ( currentCharacter == '\t'  ) {
            if ( state == StateIdle ) {
                state = StateKey;
                start = i;
            }
        }

        if ( text.mid( i, 2 ) == "/*" ) {
            if ( state == StateIdle ) {
                state = StateComment;
                start = i;
                i += 1;
            }
        }

        //terminating characters

        if ( currentCharacter == ':' ) {
            if ( state == StateKey ) {
                setFormat( start, i - start, types[ 0 ] );
                state = StateIdle;
            }
        }

        if ( text.mid( i, 2 ) == "*/" ) {
            if ( state == StateComment ) {
                setFormat( start, i - start + 1, types[ 1 ]);
                state = StateIdle;
                i += 1;
            }
        }

        if ( currentCharacter == '\"' || currentCharacter == '"' ) {
            if ( state != StateString ) {
                if ( initialString ) {
                    start = i;
                    state = StateString;
                }
            }  else {
                if ( state == StateString ) {
                    state = StateIdle;
                    setFormat( start, i - start + 1, types[ 2 ] );
                }
            }
            initialString = !initialString;
        }

    }
}
