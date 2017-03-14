#include "wbhtmlsyntax.h"

WBHTMLSyntax::WBHTMLSyntax( QTextDocument *parent )
    : QSyntaxHighlighter( parent )
{
    QTextCharFormat EntityFormat;
    EntityFormat.setForeground( SYNTAX_COLOR_SECONDARY );
    EntityFormat.setFontWeight( QFont::Bold );
    types[ 0 ] = EntityFormat;
    rehighlight();

    QTextCharFormat TagFormat;
    TagFormat.setForeground( SYNTAX_COLOR_MAIN );
    TagFormat.setFontWeight( QFont::Bold );
    types[ 1 ] = TagFormat;
    rehighlight();

    QTextCharFormat CommentFormat;
    CommentFormat.setForeground( SYNTAX_COLOR_COMMENT );
    CommentFormat.setFontItalic( true );
    CommentFormat.setFontWeight( QFont::Bold );
    types[ 2 ] = CommentFormat;
    rehighlight();

    QTextCharFormat StringFormat;
    StringFormat.setForeground( SYNTAX_COLOR_STRING );
    StringFormat.setFontItalic( true );
    types[ 3 ] = StringFormat;
    rehighlight();
}

void WBHTMLSyntax::highlightBlock(const QString &text) {
    int length = text.length();
    int state = StateIdle;
    int start = 0;

    int tmpStart = 0;
    bool initialString = true; //for string syntax highlighting
    bool stringIsOnTag = false;

    //this section is basically about HTML syntax highlighting rules

    for ( int i = 0; i < length; ++i ) {
        QChar currentCharacter = text.at( i );

        if ( currentCharacter == '<' && text.mid( i, 4 ) == "<!--" ) {
            if ( state == StateIdle ) {
                state = StateComment;
                start = i;
                i += 3;
            }
        } else if ( currentCharacter == '<' ) {
            if ( state == StateIdle ) {
                state = StateTag;
                start = i + 1;
            }
        }

        if ( currentCharacter == '&' ) {
            if ( state == StateIdle ) {
                state = StateEntity;
                start = i;
            }
        }

        //terminating characters

        if ( currentCharacter == '>' || text.mid( i, 2 ) == "/>" ) {
            if ( state == StateTag ) {
                if ( text.mid( i, 2 ) == "/>" ) {
                    setFormat( start, i - start - 1, types[ 1 ]);
                    i = i + 1;
                } else {
                    setFormat( start, i - start, types[ 1 ]);
                }
                state = StateIdle;
            }
        }

        if ( text.mid( i, 3 ) == "-->" ) {
            if ( state == StateComment ) {
                setFormat( start, i - start + 3, types[ 2 ]);
                state = StateIdle;
                i += 2;
            }
        }

        if ( currentCharacter == ';' ) {
            if ( state == StateEntity ) {
                setFormat( start, i - start + 1, types[ 0 ] );
                state = StateIdle;
            }
        }
    }
    for ( int i = 0; i < length; ++i ) {
        QChar currentCharacter = text.at( i );

        //string syntax needs more work
        if ( currentCharacter == '\"' || currentCharacter == '"' ) {
            if ( state != StateString ) {
                if ( initialString ) {
                    stringIsOnTag = ( state == StateTag );
                    tmpStart = i;
                    state = StateString;
                }
            }  else {
                if ( state == StateString ) {
                    state = (stringIsOnTag ? StateTag : StateIdle );
                    setFormat( tmpStart, i - tmpStart + 1, types[ 3 ] );
                }
            }
            initialString = !initialString;
        }
    }
}
