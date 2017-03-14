#include "wbjssyntax.h"

WBJSSyntax::WBJSSyntax( QTextDocument *parent )
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

    QTextCharFormat functionFormat;
    functionFormat.setForeground( SYNTAX_COLOR_SECONDARY );
    functionFormat.setFontWeight( QFont::Bold );
    types[ 3 ] = functionFormat;
    rehighlight();

    QTextCharFormat propertyFormat;
    propertyFormat.setForeground( SYNTAX_COLOR_TERTIARY );
    propertyFormat.setFontWeight( QFont::Bold );
    types[ 4 ] = propertyFormat;
    rehighlight();

}

void WBJSSyntax::highlightBlock(const QString &text) {
    int length = text.length();
    int state = StateIdle;
    int start = 0;

    bool initialString = true; //for string syntax highlighting

    for ( int i = 0; i < length; ++i ) {
        QChar currentCharacter = text.at( i );

       //highlight common Javascript keywords
       if ( text.mid(  i, 4) == "var " ) {
           setFormat( i, 3, types[0] );
           i += 4;
       }

       if ( text.mid(  i, 9) == "document." ) {
           setFormat( i, 8, types[0] );
           i += 9;
       }

       if ( text.mid(  i, 7) == "window." ) {
           setFormat( i, 6, types[0] );
           i += 7;
       }

       if ( text.mid(  i, 9) == "function " ) {
           setFormat( i, 8, types[0] );
           i += 9;
       }

       if ( text.mid(  i, 3) == "if " ) {
           setFormat( i, 2, types[0] );
           i += 3;
       }

       if ( text.mid(  i, 5) == "else " ) {
           setFormat( i, 4, types[0] );
           i += 5;
       }

       if ( text.mid(  i, 7) == "switch " ) {
           setFormat( i, 6, types[6] );
           i += 7;
       }

       if ( text.mid(  i, 5) == "case " ) {
           setFormat( i, 4, types[0] );
           i += 5;
       }

       if ( text.mid(  i, 4) == "for " ) {
           setFormat( i, 3, types[0] );
           i += 4;
       }

       if ( text.mid(  i, 6) == "while " ) {
           setFormat( i, 5, types[0] );
           i += 6;
       }

       if ( text.mid(  i, 3) == "do " ) {
           setFormat( i, 2, types[0] );
           i += 3;
       }

       if ( text.mid(  i, 7) == "return " ) {
           setFormat( i, 7, types[0] );
           i += 7;
       }

       if ( text.mid(  i, 5) == "try {" ) {
           setFormat( i, 3, types[0] );
           i += 4;
       }

       if ( text.mid(  i, 7) == "catch {" ) {
           setFormat( i, 5, types[0] );
           i += 6;
       }

       if ( text.mid(  i, 4) == "new " ) {
           setFormat( i, 3, types[0] );
           i += 4;
       }

       if ( text.mid(  i, 6) == ".value" ) {
           setFormat( i+1, 5, types[4] );
           i += 6;
       }

       if ( text.mid(  i, 6) == ".style" ) {
           setFormat( i+1, 5, types[4] );
           i += 6;
       }

       if ( text.mid(  i, 7) == ".length" ) {
           setFormat( i+1, 6, types[4] );
           i += 7;
       }

       if ( text.mid(  i, 5) == "Array" ) {
           setFormat( i, 5, types[4] );
           i += 5;
       }

       if ( text.mid(  i, 4) == "Date" ) {
           setFormat( i, 4, types[4] );
           i += 4;
       }

       if ( text.mid(  i, 6) == "Number" ) {
           setFormat( i, 6, types[4] );
           i += 6;
       }

       if ( text.mid(  i, 10) == ".innerHTML" ) {
           setFormat( i, 10, types[4] );
           i += 10;
       }

       if ( text.mid(  i, 10) == ".outerHTML" ) {
           setFormat( i, 10, types[4] );
           i += 10;
       }

       if ( text.mid(  i, 8) == ".location" ) {
           setFormat( i, 8, types[4] );
           i += 8;
       }

       if ( text.mid(  i, 6) == ".style" ) {
           setFormat( i+1, 5, types[4] );
           i += 6;
       }

       if ( text.mid(  i, 5) == "this." ) {
           setFormat( i, 4, types[4] );
           i += 5;
       }

       //highlight common function names
       if ( text.mid(  i, 15 ) == "getElementById(" ) {
           setFormat( i, 14, types[3] );
           i += 15;
       }

       if ( text.mid(  i, 6 ) == "alert(" ) {
           setFormat( i, 5, types[3] );
           i += 6;
       }

       if ( text.mid(  i, 7 ) == "getDay(" ) {
           setFormat( i, 6, types[3] );
           i += 7;
       }

       if ( text.mid(  i, 8 ) == "getDate(" ) {
           setFormat( i, 7, types[3] );
           i += 8;
       }

       if ( text.mid(  i, 9 ) == "getMonth(" ) {
           setFormat( i, 8, types[3] );
           i += 9;
       }

       if ( text.mid(  i, 9 ) == "getYear(" ) {
           setFormat( i, 8, types[3] );
           i += 9;
       }

       if ( text.mid(  i,7 ) == "prompt(" ) {
           setFormat( i, 6, types[3] );
           i += 7;
       }

       if ( text.mid(  i, 9 ) == "question(" ) {
           setFormat( i, 8, types[3] );
           i += 9;
       }

       if ( currentCharacter == ' '  ) {
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

        if ( currentCharacter == '(' ) {
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
