#ifndef WEBAUTHOR_H
#define WEBAUTHOR_H

#include "wbhtmlsyntax.h"
#include "wbcsssyntax.h"
#include "wbjssyntax.h"
#include "wbcolorlistmodel.h"
#include "wbcodecompletionmodel.h"
#include "wblibrarian.h"
#include "wbeditorview.h"

class WebAuthor : public QMainWindow
{
    Q_OBJECT

public:
    WebAuthor(QWidget *parent = 0);
    ~WebAuthor();

    QClipboard *clipboard;

    //(03/12/17) Text search
    QLineEdit *textSearch;

    //(03/12/17) Editor View
    WBEditorView *markupEditor;
    WBEditorView *cssEditor;
    WBEditorView *jsEditor;

    //(03/11/17) Librarian widget
    WBLibrarian *librarianWidget;

    //WebAuthor widgets
    QMenuBar *menuBar;
    QStatusBar *WBStatusbar;            

    //text areas
    WBCodeView *markupView;
    WBCodeView *cssView;
    WBCodeView *jsView;
    QTextBrowser *previewView;

    //docks
    QDockWidget *previewDock;
    QDockWidget *cssDock;
    QDockWidget *jsDock;

    //extras
    QWidget *colorPickerWidget;
    QColorDialog *colorPickerDialog;
    QListView *colorPickerList;
    WBColorListModel *colorPickerModel;
    QLineEdit *colorPickerOutput;
    QSlider *colorPickerRedValue;
    QSlider *colorPickerGreenValue;
    QSlider *colorPickerBlueValue;

    //document variables
    bool _settingWordWrap = false;
    bool _settingFullscreen = false;

    bool _settingPreviewDockShown = true;
    bool _settingCSSDockShown = false;
    bool _settingJSDockShown = false;
    bool _propertyHadPreviewed = false;
    Qt::DockWidgetArea _settingPreviewDockLocation;
    Qt::DockWidgetArea _settingCSSDockLocation;
    Qt::DockWidgetArea _settingJSDockLocation;

    //(03/12/14) Line number
    bool _settingLineNumberShown = true;

    QString _settingWebAuthorTempPath;

    //file variables
    QString markupCode;
    QString stylesheetCode;
    QString scriptCode;

    //dynamically labeled actions
    QAction *_viewMenuFullscreen;
    QAction *_viewMenuWordwrap;
    QAction *_viewMenuLineNumber; //(03/12/14)
    QAction *_viewMenuPreviewDock;
    QAction *_viewMenuCSSDock;
    QAction *_viewMenuJSDock;

    //function definition(s)
    void setupMenuBar( );
    void applyPreferences( );
    void setPreferences();

    void colorApplySlider();

private:
    QTextBrowser *help;
    QSettings *_userPreferences;
    int currentlyActive = -1;

    bool hasSaved = false;
    QString savedFileName = "";

private slots:
    void newProject();
    void openProject();
    void handleOpenProject(const QString &filename);
    void saveProject();
    void handleSaveProject(const QString &filename);
    void previewPage();
    void exportProject();
    void handleExport(const QString &filename);

    //(03/12/17)
    void showTextSearch();
    void textSearchStringChanged(const QString &searchString);

    void toggleFullscreen();
    void toggleWordwrap();
    void togglePreviewDock();
    void toggleCSSDock();
    void toggleJSDock();

    void showColorPickerWindow( );
    void colorPickerColorSelected( const QColor &color );
    void colorPickerSetText( int red, int green, int blue );
    void colorPickerPickedFromList( const QModelIndex &index );
    void showPicker();
    void colorPickerAdjustRed(int);
    void colorPickerAdjustGreen(int);
    void colorPickerAdjustBlue(int);

    void handlePreviewDockChanges( const Qt::DockWidgetArea &area );
    void handleCSSDockChanges( const Qt::DockWidgetArea &area );
    void handleJSDockChanges( const Qt::DockWidgetArea &area );

    void showGuide();
    void showAbout();
    void showAboutQt();

    void previewDockDisabled( bool s );
    void cssDockDisabled( bool s );
    void jsDockDisabled( bool s );

    //productivity shortcuts
    void switchToMain();
    void switchToStyle();
    void switchToScript();

    //code completion (03/10/17)
    void showCodeCompletion();
    void focusToMarkup();
    void focusToStylesheet();
    void focusToScript();

    //librarian (03/11/17)
    void showLibrarian();

    //line number (03/12/17)
    void toggleLineNumber( );

    void updatePreview( );
};


#endif // WEBAUTHOR_H
