#include "webauthor.h"

WebAuthor::WebAuthor(QWidget *parent)
    : QMainWindow(parent)
{

    _userPreferences = new QSettings( QSettings::IniFormat, QSettings::UserScope, "CI Software", "WebAuthor" );
    applyPreferences();

    //set up Help Window
    QFile guide( tr( ":/gettingstarted.htm" ) );
    if ( guide.open( QIODevice::ReadOnly ) ) {
        QTextStream *stream = new QTextStream( &guide );
        const QString &content = stream->readAll();
        help = new QTextBrowser( NULL );
        help->setHtml( content );
        help->setWindowTitle( tr( "Getting Started With WebAuthor" ) );
        help->setMinimumSize( 400, 500 );
        help->setMaximumSize( 400, 500 );
    }

    //set icon
    setWindowIcon( QIcon( tr( ":/icon.png" ) ) );

    //set default values for document variables
    markupCode = tr( "<html>\n\t<head>\n\t</head>\n\t<body>\n\t</body>\n</html>" );
    stylesheetCode = tr( "/*Your styles go here*/" );
    scriptCode = tr( "/*Enter Javascript here*/" );

    //set up window
    setMinimumSize( 300, 200 );
    resize( 700, 480 );

    //initialize clipboard
    clipboard = QApplication::clipboard( );

    //initialize Librarian
    librarianWidget = new WBLibrarian( );

    //(03/12/17) text search
    textSearch = new QLineEdit( );
    textSearch->setPlaceholderText( tr( "Search for text" ) );
    connect( textSearch, SIGNAL(textChanged(QString)), SLOT(textSearchStringChanged(QString)));
    textSearch->setMinimumSize( 300, 30 );
    textSearch->resize( 300, 30 );
    textSearch->setWindowTitle( tr( "WebAuthor: Text Search" ) );
    textSearch->setMaximumSize( 500, 30 );

    //initialize code completion
    //added on (03/10/17)
    WBCodeCompletionModel *_htmlCodeCompletionModel = new WBCodeCompletionModel( ":/html_elements.wblist" );
    WBCodeCompletionModel *_cssCodeCompletionModel = new WBCodeCompletionModel( ":/css_elements.wblist" );
    WBCodeCompletionModel *_jsCodeCompletionModel = new WBCodeCompletionModel( ":/js_elements.wblist" );

    QCompleter *_htmlCompleter = new QCompleter( );
    _htmlCompleter->setModel( _htmlCodeCompletionModel );

    QCompleter *_cssCompleter = new QCompleter();
    _cssCompleter->setModel( _cssCodeCompletionModel );

    QCompleter *_jsCompleter = new QCompleter();
    _jsCompleter->setModel( _jsCodeCompletionModel );

    //initialize views
    markupView = new WBCodeView( _htmlCompleter, false );
    previewView = new QTextBrowser( );
    cssView = new WBCodeView( _cssCompleter, false );
    jsView = new WBCodeView( _jsCompleter, true );
    colorPickerWidget = new QWidget();

    WBStatusbar = new QStatusBar( );
    WBStatusbar->showMessage( tr( "Welcome to WebAuthor!" ) );

    menuBar = new QMenuBar( );
    setupMenuBar();

    //initialize syntax highlighter
    WBHTMLSyntax *markupSyntax = new WBHTMLSyntax();
    markupSyntax->setParent( markupView );
    markupSyntax->setDocument( markupView->document() );

    WBCSSSyntax *stylesheetSyntax = new WBCSSSyntax();
    stylesheetSyntax->setParent( cssView );
    stylesheetSyntax->setDocument( cssView->document() );

    WBJSSyntax *scriptSyntax = new WBJSSyntax();
    scriptSyntax->setParent( jsView );
    scriptSyntax->setDocument( jsView->document() );

    //set up widgets

    //set up color picker dialog
    colorPickerOutput = new QLineEdit( );
    colorPickerDialog = new QColorDialog( colorPickerWidget );
    colorPickerList = new QListView( );

    QWidget *colorPickerSliders = new QWidget();
    QFormLayout *colorPickerSliderLayout = new QFormLayout();
    colorPickerRedValue = new QSlider( Qt::Horizontal );
    colorPickerGreenValue = new QSlider( Qt::Horizontal );
    colorPickerBlueValue = new QSlider( Qt::Horizontal );

    connect( colorPickerRedValue, SIGNAL(valueChanged(int)), SLOT(colorPickerAdjustRed(int)) );
    connect( colorPickerGreenValue, SIGNAL(valueChanged(int)), SLOT(colorPickerAdjustGreen(int)) );
    connect( colorPickerBlueValue, SIGNAL(valueChanged(int)), SLOT(colorPickerAdjustBlue(int)) );

    colorPickerRedValue->setSingleStep( 5 );
    colorPickerGreenValue->setSingleStep( 5 );
    colorPickerBlueValue->setSingleStep( 5 );

    colorPickerRedValue->setMinimum( 0 );
    colorPickerGreenValue->setMinimum( 0 );
    colorPickerBlueValue->setMinimum( 0 );

    colorPickerRedValue->setMaximum( 255 );
    colorPickerGreenValue->setMaximum( 255 );
    colorPickerBlueValue->setMaximum( 255 );

    colorPickerRedValue->setValue( 255 );
    colorPickerGreenValue->setValue( 255 );
    colorPickerBlueValue->setValue( 255 );

    int _colorPickerTickInterval = 5;
    colorPickerRedValue->setTickInterval( _colorPickerTickInterval );
    colorPickerGreenValue->setTickInterval( _colorPickerTickInterval );
    colorPickerBlueValue->setTickInterval( _colorPickerTickInterval );

    colorPickerSliderLayout->addWidget( new QLabel( tr( "Red: " ) ) );
    colorPickerSliderLayout->addWidget( colorPickerRedValue );
    colorPickerSliderLayout->addWidget( new QLabel( tr( "Green: " ) ) );
    colorPickerSliderLayout->addWidget( colorPickerGreenValue );
    colorPickerSliderLayout->addWidget( new QLabel( tr( "Blue: " ) ) );
    colorPickerSliderLayout->addWidget( colorPickerBlueValue );

    colorPickerSliders->setLayout( colorPickerSliderLayout );

    colorPickerWidget->setWindowTitle( tr( "Color Picker" ) );

    QVBoxLayout *colorPickerLayout = new QVBoxLayout();
    QLabel *colorPickerText = new QLabel( tr( "Pick a color" ) );
    QPushButton *showColorPickerDialog = new QPushButton( tr( "Pick a Color" ) );
    colorPickerModel = new WBColorListModel( );

    colorPickerList->setModel( colorPickerModel );

    connect( showColorPickerDialog, SIGNAL(clicked()), SLOT(showPicker()) );
    connect( colorPickerList, SIGNAL(clicked(QModelIndex)), SLOT(colorPickerPickedFromList(QModelIndex)) );

    colorPickerLayout->addWidget( colorPickerText );
    colorPickerLayout->addWidget( colorPickerList );
    colorPickerLayout->addWidget( colorPickerOutput );
    colorPickerLayout->addWidget( colorPickerSliders );
    colorPickerLayout->addWidget( showColorPickerDialog );

    colorPickerWidget->setLayout( colorPickerLayout );
    connect( colorPickerDialog, SIGNAL(currentColorChanged(QColor)), SLOT(colorPickerColorSelected(QColor)) );

    //set up text widgets
    markupView->setTabStopWidth( 10 );
    markupView->setPlainText( markupCode );
    markupView->setTabChangesFocus( false );

    cssView->setTabStopWidth( 10 );
    cssView->setPlainText( stylesheetCode );
    cssView->setTabChangesFocus( false );

    jsView->setTabStopWidth( 10 );
    jsView->setPlainText( scriptCode );
    jsView->setTabChangesFocus( false );

    WBCodeCompletionModel *markupModel = new WBCodeCompletionModel( ":/html_elements.wblist" );
    WBCodeCompletionModel *stylesheetModel = new WBCodeCompletionModel( ":/css_elements.wblist" );
    WBCodeCompletionModel *scriptModel = new WBCodeCompletionModel( ":/js_elements.wblist" );

    markupView->setModel( markupModel );
    cssView->setModel( stylesheetModel );
    jsView->setModel( scriptModel );

    //(03/12/17) - Editor views
    markupEditor = new WBEditorView( markupView, _settingLineNumberShown );
    cssEditor = new WBEditorView( cssView, _settingLineNumberShown );
    jsEditor = new WBEditorView( jsView, _settingLineNumberShown );

    //set up docks
    previewDock = new QDockWidget( tr( "Layout Preview" ) );
    previewDock->setFloating( false );
    previewDock->setWidget( previewView );
    previewDock->setFeatures( QDockWidget::DockWidgetMovable );

    cssDock = new QDockWidget( tr( "CSS" ) );
    cssDock->setFloating( false );
    cssDock->setWidget( cssEditor );
    cssDock->setFeatures( QDockWidget::DockWidgetMovable );

    jsDock = new QDockWidget( tr( "Javascript" ) );
    jsDock->setFloating( false );
    jsDock->setWidget( jsEditor );
    jsDock->setFeatures( QDockWidget::DockWidgetMovable );

    previewDock->setVisible( _settingPreviewDockShown );
    cssDock->setVisible( _settingCSSDockShown );
    jsDock->setVisible( _settingJSDockShown );

    //03/09/17 - Cross platform support added
    //point to up temporary directory
    #ifdef Q_OS_LINUX
        _settingWebAuthorTempPath = "/tmp";
    #elif defined( Q_OS_MACX )
        _settingWebAuthorTempPath = "/var/tmp";
    #elif defined( Q_OS_WIN )
        _settingWebAuthorTempPath = "C:/Users/Public";
    #else
        #error "Unsupported system"
    #endif

    //apply preferences ( post )
    if ( _settingFullscreen ) this->showFullScreen();
    if ( !_settingWordWrap ) {
        markupView->setLineWrapMode( QPlainTextEdit::NoWrap );
        cssView->setLineWrapMode( QPlainTextEdit::NoWrap );
        jsView->setLineWrapMode( QPlainTextEdit::NoWrap );
        markupEditor->setWordWrap( false );
        cssEditor->setWordWrap( false );
        jsEditor->setWordWrap( false );
    } else {
        markupEditor->setWordWrap( true );
        cssEditor->setWordWrap( true );
        jsEditor->setWordWrap( true );
    }

    //set up WebAuthor layout
    this->setCentralWidget( markupEditor );
    this->setStatusBar( WBStatusbar );
    this->addDockWidget( _settingPreviewDockLocation, previewDock );
    this->addDockWidget( _settingCSSDockLocation, cssDock );
    this->addDockWidget( _settingJSDockLocation, jsDock );

    //invisible actions
    //productivity shortcuts (03/10/17)
    QAction *_ia_switchToMain = new QAction( NULL );
    _ia_switchToMain->setShortcut( QKeySequence( "Ctrl+1" ) );
    connect( _ia_switchToMain, SIGNAL(triggered(bool)), SLOT(switchToMain()) );

    QAction *_ia_switchToStyle = new QAction( NULL );
    _ia_switchToStyle->setShortcut( QKeySequence( "Ctrl+2" ) );
    connect( _ia_switchToStyle, SIGNAL(triggered(bool)), SLOT(switchToStyle()) );

    QAction *_ia_switchToScript = new QAction( NULL );
    _ia_switchToScript->setShortcut( QKeySequence( "Ctrl+3" ) );
    connect( _ia_switchToScript, SIGNAL(triggered(bool)), SLOT(switchToScript()) );

    addAction( _ia_switchToMain );
    addAction( _ia_switchToStyle );
    addAction( _ia_switchToScript );

    //set up connections
    connect( markupView, SIGNAL(textChanged()), SLOT(updatePreview()) );
    connect( previewDock, SIGNAL(visibilityChanged(bool)), SLOT(previewDockDisabled(bool))  );
    connect( cssDock, SIGNAL(visibilityChanged(bool)), SLOT(cssDockDisabled(bool))  );
    connect( jsDock, SIGNAL(visibilityChanged(bool)), SLOT(jsDockDisabled(bool))  );

    connect( previewDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(handlePreviewDockChanges(Qt::DockWidgetArea)) );
    connect( cssDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(handleCSSDockChanges(Qt::DockWidgetArea)) );
    connect( jsDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(handleJSDockChanges(Qt::DockWidgetArea)) );

    connect( markupView, SIGNAL(cursorPositionChanged()), SLOT(focusToMarkup()) );
    connect( cssView, SIGNAL(cursorPositionChanged()), SLOT(focusToStylesheet()) );
    connect( jsView, SIGNAL(cursorPositionChanged()), SLOT(focusToScript()) );

}

void WebAuthor::updatePreview() {
    //update code
     markupCode = markupView->document()->toPlainText();
     stylesheetCode = cssView->document()->toPlainText();
     scriptCode = jsView->document()->toPlainText();

     if ( markupView->isVisible() ) markupView->update();
     if ( cssView->isVisible() ) cssView->update();
     if ( jsView->isVisible() ) jsView->update();

     /*compile the document
     QString compiledHTML = markupCode;
     compiledHTML.append( tr( "<style> %1 </style>" ).arg( stylesheetCode ) );
     compiledHTML.append( tr( "<script> %1 </script>" ).arg( scriptCode ) );
     */

     //preview the site
     previewView->document()->setHtml( markupCode );
}

void WebAuthor::setupMenuBar() {

    //create menus and entries
    QMenu *_fileMenu = new QMenu( tr( "&File" ) );

    QAction *_fileMenuNew = new QAction( NULL );
    _fileMenuNew->setText( tr( "&New Webpage" ) );
    _fileMenuNew->setShortcut( QKeySequence::New );
    _fileMenuNew->setStatusTip( tr( "Create a new webpage." ) );

    QAction *_fileMenuOpen = new QAction( NULL );
    _fileMenuOpen->setText( tr( "&Open WebAuthor Project" ) );
    _fileMenuOpen->setShortcut( QKeySequence::Open );
    _fileMenuOpen->setStatusTip( tr( "Open an existing WebAuthor file." ) );

    QAction *_fileMenuSave = new QAction( NULL  );
    _fileMenuSave->setText( tr( "&Save Project" ) );
    _fileMenuSave->setShortcut( QKeySequence::Save );
    _fileMenuSave->setStatusTip( tr( "Save as a WebAuthor project." ) );

    QAction *_fileMenuPreview = new QAction( NULL );
    _fileMenuPreview->setText( tr( "&Preview in Browser" ) );
    _fileMenuPreview->setStatusTip( tr( "Previews your project in a browser." ) );
    _fileMenuPreview->setShortcut( QKeySequence( "F5" ) );

    QAction *_fileMenuCompile = new QAction( NULL );
    _fileMenuCompile->setText( tr( "&Export Webpage" ) );
    _fileMenuCompile->setShortcut( QKeySequence::SaveAs );
    _fileMenuCompile->setStatusTip( tr( "Export webpage as an html document." ) );

    QAction *_fileMenuQuit = new QAction( NULL );
    _fileMenuQuit->setText( tr( "&Quit" ) );
    _fileMenuQuit->setShortcut( QKeySequence::Quit );
    _fileMenuQuit->setStatusTip( tr( "Close WebAuthor." ) );

    QMenu *_viewMenu = new QMenu( tr( "&View" ) );

    _viewMenuFullscreen = new QAction( NULL );
    _viewMenuFullscreen->setText( tr( "&Fullscreen" )  );
    _viewMenuFullscreen->setCheckable( true );
    _viewMenuFullscreen->setChecked( _settingFullscreen );
    _viewMenuFullscreen->setShortcut( QKeySequence::FullScreen );
    _viewMenuFullscreen->setStatusTip( tr( "Toggle fullscreen mode." ) );

    _viewMenuWordwrap = new QAction( NULL );
    _viewMenuWordwrap->setText( tr( "&Word Wrap" ) );
    _viewMenuWordwrap->setCheckable( true );
    _viewMenuWordwrap->setChecked( _settingWordWrap );
    _viewMenuWordwrap->setStatusTip( tr( "Toggle word wrapping." ) );

    _viewMenuLineNumber = new QAction( NULL );
    _viewMenuLineNumber->setText( tr( "S&how Line Numbers" ) );
    _viewMenuLineNumber->setCheckable( true );
    _viewMenuLineNumber->setChecked( _settingLineNumberShown );
    _viewMenuLineNumber->setStatusTip( tr( "Toggles the visibility of line numbers in editors." ) );

    _viewMenuPreviewDock = new QAction( NULL );
    _viewMenuPreviewDock->setText( tr( "Show &Preview Dock" ) );
    _viewMenuPreviewDock->setCheckable( true );
    _viewMenuPreviewDock->setChecked( _settingPreviewDockShown );
    _viewMenuPreviewDock->setStatusTip( tr( "Show the Layout Preview Dock." ) );
    _viewMenuPreviewDock->setShortcut( QKeySequence( "Ctrl+Shift+1" ) );

    _viewMenuCSSDock = new QAction( NULL );
    _viewMenuCSSDock->setText( tr( "Show &CSS Dock") );
    _viewMenuCSSDock->setCheckable( true );
    _viewMenuCSSDock->setChecked( _settingCSSDockShown );
    _viewMenuCSSDock->setStatusTip( tr( "Show the webpage's stylesheet editor." ) );
    _viewMenuCSSDock->setShortcut( QKeySequence( "Ctrl+Shift+2" ) );

    _viewMenuJSDock = new QAction( NULL );
    _viewMenuJSDock->setText( tr( "Show &Javascript Dock" ) );
    _viewMenuJSDock->setCheckable( true );
    _viewMenuJSDock->setChecked( _settingJSDockShown );
    _viewMenuJSDock->setStatusTip( tr( "Show the webpage's javascript source editor." ) );
    _viewMenuJSDock->setShortcut( QKeySequence( "Ctrl+Shift+3" ) );

    QAction *_viewMenuCodeCompletion = new QAction( NULL );
    _viewMenuCodeCompletion->setText( tr( "&Invoke Code Completion" ) );
    _viewMenuCodeCompletion->setShortcut( QKeySequence( "Ctrl+space" ) );
    _viewMenuCodeCompletion->setStatusTip( tr( "Shows the code completion dialog." ) );

    QAction *_viewMenuColorTool = new QAction( NULL );
    _viewMenuColorTool->setText( tr( "Show &Color Picker" ) );
    _viewMenuColorTool->setStatusTip( tr( "Pick a color from an easy to use dialog." ) );
    _viewMenuColorTool->setShortcut( QKeySequence( "Ctrl+i" ) );

    //(03/12/17)
    QAction *_viewMenuFindText = new QAction( NULL );
    _viewMenuFindText->setText( tr( "Search &text" ) );
    _viewMenuFindText->setShortcut( QKeySequence::Find );
    _viewMenuFindText->setStatusTip( tr( "Search for text in your webpage." ) );

    QMenu *_helpMenu = new QMenu( tr( "&Help" ) );

    QAction *_helpMenuGuide = new QAction( NULL );
    _helpMenuGuide->setText( tr( "&Quick Start Guide" )  );
    _helpMenuGuide->setShortcut( QKeySequence( "Shift+F1" ) );
    _helpMenuGuide->setStatusTip( tr( "Show WebAuthor's quick start guide," ) );

    //(03/11/17) - Librarian documentation
    QAction *_helpLibrarian = new QAction( NULL );
    _helpLibrarian->setText(  tr( "Show &Librarian Documents" ) );
    _helpLibrarian->setShortcut( QKeySequence::HelpContents );
    _helpLibrarian->setStatusTip( tr( "Opens the Librarian Documentation Browser" ) );

    QAction *_helpMenuAbout = new QAction( NULL );
    _helpMenuAbout->setText( tr( "About &WebAuthor" ) );
    _helpMenuAbout->setStatusTip( tr( "Show WebAuthor's about message." ) );

    QAction *_helpMenuAboutQt = new QAction( NULL );
    _helpMenuAboutQt->setText( tr( "&About Qt" ) );
    _helpMenuAboutQt->setStatusTip( tr( "Show Qt Info message." ) );

    //connect to slots
    connect( _fileMenuNew, SIGNAL(triggered(bool)), SLOT(newProject()));
    connect( _fileMenuOpen, SIGNAL(triggered(bool)), SLOT(openProject()) );
    connect( _fileMenuSave, SIGNAL(triggered(bool)), SLOT(saveProject()) );
    connect( _fileMenuCompile, SIGNAL(triggered(bool)), SLOT(exportProject()) );
    connect( _fileMenuPreview, SIGNAL(triggered(bool)), SLOT(previewPage()));
    connect( _fileMenuQuit, SIGNAL(triggered(bool)), SLOT(close()) );

    connect( _viewMenuFullscreen, SIGNAL(triggered(bool)), SLOT(toggleFullscreen()) );
    connect( _viewMenuWordwrap, SIGNAL(triggered(bool)), SLOT(toggleWordwrap()) );
    connect( _viewMenuLineNumber, SIGNAL(triggered(bool)), SLOT(toggleLineNumber()) );
    connect( _viewMenuPreviewDock, SIGNAL(triggered(bool)), SLOT(togglePreviewDock()) );
    connect( _viewMenuCSSDock, SIGNAL(triggered(bool)), SLOT(toggleCSSDock()) );
    connect( _viewMenuJSDock, SIGNAL(triggered(bool)), SLOT(toggleJSDock()) );
    connect( _viewMenuColorTool, SIGNAL(triggered(bool)), SLOT(showColorPickerWindow()) );
    connect( _viewMenuCodeCompletion, SIGNAL(triggered(bool)), SLOT(showCodeCompletion()) );
    connect( _viewMenuFindText, SIGNAL(triggered(bool)), SLOT(showTextSearch()) );

    connect( _helpLibrarian, SIGNAL(triggered(bool)), SLOT(showLibrarian()) );
    connect( _helpMenuGuide, SIGNAL(triggered(bool)), SLOT(showGuide()) );
    connect( _helpMenuAbout, SIGNAL(triggered(bool)), SLOT(showAbout()) );
    connect( _helpMenuAboutQt, SIGNAL(triggered(bool)), SLOT(showAboutQt()) );

    //insert entries
    _fileMenu->addAction( _fileMenuNew );
    _fileMenu->addAction( _fileMenuOpen );
    _fileMenu->addAction( _fileMenuSave );
    _fileMenu->addAction( _fileMenuPreview );
    _fileMenu->addAction( _fileMenuCompile );
    _fileMenu->addSeparator();
    _fileMenu->addAction( _fileMenuQuit );

    _viewMenu->addAction( _viewMenuFullscreen );
    _viewMenu->addAction( _viewMenuWordwrap );
    //_viewMenu->addAction( _viewMenuLineNumber );
    _viewMenu->addSeparator();
    _viewMenu->addAction( _viewMenuPreviewDock );
    _viewMenu->addAction( _viewMenuCSSDock );
    _viewMenu->addAction( _viewMenuJSDock );
    _viewMenu->addSeparator();
    _viewMenu->addAction( _viewMenuCodeCompletion );
    _viewMenu->addAction( _viewMenuColorTool );
    _viewMenu->addAction( _viewMenuFindText );

    _helpMenu->addAction( _helpLibrarian );
    _helpMenu->addAction( _helpMenuGuide );
    _helpMenu->addSeparator();
    _helpMenu->addAction( _helpMenuAbout );
    _helpMenu->addAction( _helpMenuAboutQt );

    //add menus
    menuBar->addMenu( _fileMenu );
    menuBar->addMenu( _viewMenu );
    menuBar->addMenu( _helpMenu );

    this->setMenuBar( menuBar );
}

void WebAuthor::showAbout() {
    QString extra = "";

    #ifdef Q_OS_LINUX
        extra = tr( " for Linux " );
    #elif defined Q_OS_MACX
        extra = tr( " for Mac " );
    #elif defined Q_OS_WIN
        extra = tr( " for Windows" );
    #endif

    QMessageBox::information( this, tr( "About WebAuthor" ), tr( "<center><b>WebAuthor %1 v1.0</b></center><br /><b>WebAuthor</b> is an easy to use webpage creation environment. <br />"
                                                                 "Original Programmer: Charles Ivan Mozar<br /><br />"
                                                                 "Grab the latest source code / binaries at our <a href=http://webauthor.net16.net>Website</a><br /><Br />"
                                                                 "WebAuthor is created using the <a href=http://qt.io>Qt 5 Framework</a>. <Br /> <br />"
                                                                 "WebAuthor is maintained by <a href=https://www.facebook.com/CISoftwareOfficial>CI Software</a> (2017-2018)" ).arg( extra ) );
}

void WebAuthor::showAboutQt() {
    QMessageBox::aboutQt( this );
}

void WebAuthor::showGuide() {
    help->show();
}

void WebAuthor::showPicker( ) {
    colorPickerDialog->setCurrentColor( QColor( colorPickerRedValue->value(),
                                                colorPickerGreenValue->value(),
                                                colorPickerBlueValue->value() ));
    colorPickerDialog->show();
}

void WebAuthor::showColorPickerWindow() {
    colorPickerWidget->show();
}

void WebAuthor::colorPickerColorSelected( const QColor &color ) {
    colorPickerSetText( color.red(), color.green(), color.blue() );
}

void WebAuthor::colorPickerSetText(int red, int green, int blue) {
    QString textShown = tr("rgb( %1,").arg( red );
    textShown.append( tr( "%1," ).arg( green ) );
    textShown.append( tr( "%1 )" ).arg( blue ) );
    colorPickerOutput->setText( textShown );

    colorPickerRedValue->setValue( red );
    colorPickerGreenValue->setValue( green );
    colorPickerBlueValue->setValue( blue );

    //color value determines whether or not the color is either light or dark
    int color_value = red + green * 2 + blue; //Cv = r + 2g + b
    int dark_threshold = 255 + 255; //this is our baseline for color lightness

    //copy to clipboard
    clipboard->setText( textShown );

    //modify color dialog values
    colorPickerDialog->setCurrentColor( QColor(red,green,blue) );

    //preview the selected color
    if ( color_value >= dark_threshold ) colorPickerOutput->setStyleSheet( QString( "text-align: center; background: %1; color: #000;" ).arg( textShown ) );
    else colorPickerOutput->setStyleSheet( QString( "text-align: center; background: %1; color: #FFF;" ).arg( textShown ) );
}

void WebAuthor::applyPreferences() {
    _settingFullscreen = _userPreferences->value( "editor/fullscreen", false ).toBool();
    _settingPreviewDockShown = _userPreferences->value( "editor/preview_visibility", true ).toBool();
    _settingCSSDockShown = _userPreferences->value( "editor/styles_visibility", false ).toBool();
    _settingJSDockShown = _userPreferences->value( "editor/script_visibility", false ).toBool();
    _settingWordWrap = _userPreferences->value( "editor/word_wrapping", false ).toBool();
    _settingLineNumberShown = _userPreferences->value( "editor/line_numbers", true ).toBool();

    this->move( _userPreferences->value( "editor/last_position", QPoint( 300, 300 ) ).toPoint() );
    this->resize( _userPreferences->value( "editor/last_size", QSize( 700, 480 ) ).toSize() );

    _settingCSSDockLocation = (Qt::DockWidgetArea)_userPreferences->value( "editor/styles_location", Qt::BottomDockWidgetArea ).toInt();
    _settingJSDockLocation = (Qt::DockWidgetArea)_userPreferences->value( "editor/script_location", Qt::BottomDockWidgetArea ).toInt();
    _settingPreviewDockLocation = (Qt::DockWidgetArea)_userPreferences->value( "editor/preview_location", Qt::RightDockWidgetArea ).toInt();
}

void WebAuthor::setPreferences() {
    _userPreferences->setValue( "editor/fullscreen", _settingFullscreen );
    _userPreferences->setValue( "editor/preview_visibility", previewDock->isVisible() );
    _userPreferences->setValue( "editor/styles_visibility", cssDock->isVisible() );
    _userPreferences->setValue( "editor/script_visibility", jsDock->isVisible() );
    _userPreferences->setValue( "editor/word_wrapping", _viewMenuWordwrap->isChecked() );
    _userPreferences->setValue( "editor/line_numbers", _viewMenuLineNumber->isChecked() );

    _userPreferences->setValue( "editor/last_position", this->pos() );
    _userPreferences->setValue( "editor/last_size", this->size() );

    _userPreferences->setValue( "editor/preview_location", _settingPreviewDockLocation );
    _userPreferences->setValue( "editor/styles_location", _settingCSSDockLocation );
    _userPreferences->setValue( "editor/script_location", _settingJSDockLocation );

}

void WebAuthor::handlePreviewDockChanges( const Qt::DockWidgetArea &area ) {
    _settingPreviewDockLocation = area;
    _viewMenuPreviewDock->setChecked( previewDock->isVisible() );
    setPreferences();
}

void WebAuthor::handleCSSDockChanges( const Qt::DockWidgetArea &area ) {
    _settingCSSDockLocation = area;
    _viewMenuCSSDock->setChecked( cssDock->isVisible() );
    setPreferences();
}

void WebAuthor::handleJSDockChanges( const Qt::DockWidgetArea &area ) {
    _settingJSDockLocation = area;
    _viewMenuJSDock->setChecked( jsDock->isVisible() );
    setPreferences();
}

void WebAuthor::newProject() {
    int response = QMessageBox::question( this, tr( "New Project?" ), tr( "This will destroy any unsaved changes!" ), QMessageBox::Yes | QMessageBox::No );

    if ( response == QMessageBox::Yes ) {
        //reset everything, once the user creates a new project

        markupCode = "<html>\n\t<head>\n\t</head>\n\t<body>\n\t</body>\n</html>";

        markupView->setPlainText( markupCode );
        cssView->setPlainText( "/*Your styles go here*/" );
        jsView->setPlainText( "/*Enter javascript here*/" );

        updatePreview();

        //reset saved status
        hasSaved = false;
    }
}

void WebAuthor::openProject() {
    QFileDialog *openDialog = new QFileDialog( this );
    #ifdef Q_OS_MACX
            openDialog->setOptions( QFileDialog::DontUseNativeDialog );
    #elif defined( Q_OS_WIN )
            openDialog->setOptions( QFileDialog::DontUseNativeDialog );
    #endif
    openDialog->setAcceptMode( QFileDialog::AcceptOpen );
    openDialog->setFileMode( QFileDialog::ExistingFile );
    openDialog->setNameFilter( tr( "WebAuthor Project files (*.wbproj)" ) );
    openDialog->setDefaultSuffix( tr( "wbproj" ) );
    connect( openDialog, SIGNAL(fileSelected(QString)), SLOT(handleOpenProject(QString)) );
    openDialog->show();
}

void WebAuthor::handleOpenProject(const QString &filename) {
    QFile openFile( filename );
    if ( openFile.open( QIODevice::ReadOnly ) ) {
        QTextStream *stream = new QTextStream( &openFile );
        QString documentData = stream->readAll();
        QStringList documentComponent = documentData.split( '~' );

        //if the document has a WBPROJ header, it is a webauthor project file
        if ( documentComponent.first() == tr( "WBPROJ" ) ) {
            QString markup = documentComponent[1];
            QString cssstylesheet = documentComponent[ 2 ];
            QString jscript = documentComponent[ 3 ];

            //assign components
            markupView->setPlainText( markup );
            cssView->setPlainText( cssstylesheet );
            jsView->setPlainText( jscript );
            updatePreview(); //show the newly loaded document in the layout preview

            //update saved status
            hasSaved = true;
            savedFileName = filename;

        } else {
            QMessageBox::warning( this, tr( "Unable to open file!" ), tr( "You can only read WebAuthor project files." )  );
        }

        openFile.close();
    } else {
        QMessageBox::warning( this, tr( "Unable to open file!" ), tr( "Unable to read file." )  );
    }
}

void WebAuthor::saveProject() {
    if ( !hasSaved ) {
        QFileDialog *saveDialog = new QFileDialog( this );

        #ifdef Q_OS_MACX
                saveDialog->setOptions( QFileDialog::DontUseNativeDialog );
        #elif defined( Q_OS_WIN )
                saveDialog->setOptions( QFileDialog::DontUseNativeDialog );
        #endif

        saveDialog->setAcceptMode( QFileDialog::AcceptSave );
        saveDialog->setDefaultSuffix( tr( "wbproj" ) );
        saveDialog->setNameFilter( tr( "WebAuthor Project files (*.wbproj)" ) );
        connect( saveDialog, SIGNAL(fileSelected(QString)), SLOT(handleSaveProject(QString)) );
        saveDialog->show();
    } else {
        handleSaveProject( savedFileName );
    }
}

void WebAuthor::handleSaveProject(const QString &filename) {
    QFile save( filename );
    if ( save.open( QIODevice::WriteOnly )) {
        QTextStream stream( &save );
        updatePreview(); //update document code
        stream << "WBPROJ~" << markupCode << "~" << stylesheetCode << "~" << scriptCode;
        hasSaved = true;
        savedFileName = filename;
        save.close();
    } else {
        QMessageBox::warning( this, tr( "Save" ), tr( "Unable to save project at this location." ) );
    }
}

void WebAuthor::previewPage() {

    QString browseExecCode = NULL;
    #define expectedReturnCode 0

    //03/09/17 - Cross platform support
    #ifdef Q_OS_LINUX
        browseExecCode = QString("firefox %1/.web_author_preview.html").arg( _settingWebAuthorTempPath );
    #elif defined( Q_OS_MACX )
        browseExecCode = QString("/Applications/Firefox.app/Contents/MacOS/Firefox %1/.web_author_preview.html").arg( _settingWebAuthorTempPath );
    #elif defined( Q_OS_WIN )
        browseExecCode = QString("\"C:\\Program Files\\Internet Explorer\\iexplore.exe\" %1/.web_author_preview.html").arg( _settingWebAuthorTempPath );
    #else
        #error "Unsupported system"
    #endif

    if ( browseExecCode != NULL ) {
        QFile save( QString("%1/.web_author_preview.html").arg( _settingWebAuthorTempPath ).toStdString().c_str() );
        _propertyHadPreviewed = true; //delete the preview page before application termination

        if ( save.open( QIODevice::WriteOnly )) {
            QTextStream stream( &save );
            updatePreview();
            stream <<  markupCode << "<style>" << stylesheetCode << "</style><script>" << scriptCode << "</script>";
            save.close();

            if ( !(std::system( browseExecCode.toStdString().c_str() ) == expectedReturnCode ) ) {

                #ifdef Q_OS_LINUX
                //if firefox wasn't installed, show a message ( Linux only )
                QMessageBox::warning( this, tr( "Preview Error" ), tr( "Firefox wasn't found on your system, is it installed? <br />"
                                                                       "Get the latest version of <a href=www.mozilla.org/en-US/firefox/new>Firefox by clicking here</a>" ) );
                #elif defined ( Q_OS_MACX )
                QMessageBox::warning( this, tr( "Preview Error" ), tr( "Firefox wasn't found on your system, is it installed? <br />"
                                                                       "Get the latest version of <a href=www.mozilla.org/en-US/firefox/new>Firefox by clicking here</a>" ) );
                #endif


            }

        } else {
            QMessageBox::warning( this, tr( "Preview" ), tr( "Unable to access the %1/ directory" ).arg( _settingWebAuthorTempPath ) );
        }
    }
}

void WebAuthor::exportProject() {
    QFileDialog *saveDialog = new QFileDialog( this );
    #ifdef Q_OS_MACX
            saveDialog->setOptions( QFileDialog::DontUseNativeDialog );
    #elif defined( Q_OS_WIN )
            saveDialog->setOptions( QFileDialog::DontUseNativeDialog );
    #endif
    saveDialog->setAcceptMode( QFileDialog::AcceptSave );
    saveDialog->setNameFilter( tr( "HTML Documents (*.html, *.htm)" ) );
    saveDialog->setDefaultSuffix( tr( "html" ) );
    connect( saveDialog, SIGNAL(fileSelected(QString)), SLOT(handleExport(QString)) );
    saveDialog->show();
}

void WebAuthor::handleExport(const QString &filename) {
    QFile save( filename );
    if ( save.open( QIODevice::WriteOnly )) {
        QTextStream stream( &save );
        updatePreview();
        stream <<  markupCode << "<style>" << stylesheetCode << "</style><script>" << scriptCode << "</script>";
        save.close();
    } else {
        QMessageBox::warning( this, tr( "Export" ), tr( "Unable to export project at this location." ) );
    }
}

void WebAuthor::toggleFullscreen() {
    _settingFullscreen = !isFullScreen();
    _viewMenuFullscreen->setChecked( _settingFullscreen );

    ( _settingFullscreen ? showFullScreen() : showNormal() );
    this->setPreferences();
}

void WebAuthor::toggleWordwrap() {
    _settingWordWrap = !_settingWordWrap;
    _viewMenuWordwrap->setChecked( _settingWordWrap );

    if ( _settingWordWrap ) {
        markupView->setLineWrapMode( QPlainTextEdit::WidgetWidth );
        cssView->setLineWrapMode( QPlainTextEdit::WidgetWidth );
        jsView->setLineWrapMode( QPlainTextEdit::WidgetWidth );
        markupEditor->setWordWrap( true );
        cssEditor->setWordWrap( true );
        jsEditor->setWordWrap( true );
    } else {
        markupView->setLineWrapMode( QPlainTextEdit::NoWrap );
        cssView->setLineWrapMode( QPlainTextEdit::NoWrap );
        jsView->setLineWrapMode( QPlainTextEdit::NoWrap );
        markupEditor->setWordWrap( false );
        cssEditor->setWordWrap( false );
        jsEditor->setWordWrap( false );
    }
    this->setPreferences();

}

void WebAuthor::previewDockDisabled( bool s ) {
    if ( !s ) {
        _settingPreviewDockShown = false;
        _viewMenuPreviewDock->setChecked( _settingPreviewDockShown );
    }
}

void WebAuthor::cssDockDisabled( bool s ) {
    if ( !s ) {
        _settingCSSDockShown = false;
        _viewMenuCSSDock->setChecked( _settingCSSDockShown );
    }
}

void WebAuthor::jsDockDisabled( bool s) {
    if ( !s ) {
        _settingJSDockShown = false;
        _viewMenuJSDock->setChecked( _settingJSDockShown );
    }
}

void WebAuthor::togglePreviewDock() {
    _settingPreviewDockShown = !_settingPreviewDockShown;
    _viewMenuPreviewDock->setChecked( _settingPreviewDockShown );
    previewDock->setVisible( _settingPreviewDockShown );
    this->setPreferences();
}

void WebAuthor::toggleCSSDock() {
    _settingCSSDockShown = !_settingCSSDockShown;
    _viewMenuCSSDock->setChecked( _settingCSSDockShown );
    cssDock->setVisible( _settingCSSDockShown );
    this->setPreferences();
}

void WebAuthor::toggleJSDock() {
    _settingJSDockShown = !_settingJSDockShown;
    _viewMenuJSDock->setChecked( _settingJSDockShown );
    jsDock->setVisible( _settingJSDockShown );
    this->setPreferences();
}

WebAuthor::~WebAuthor()
{
    this->setPreferences(); //save user preferences

    if ( _propertyHadPreviewed ) {
        //remove preview page before quitting
        #ifdef Q_OS_LINUX
        std::system( QString("rm %1/.web_author_preview.html").arg( _settingWebAuthorTempPath ).toStdString().c_str() );
        #elif defined( Q_OS_MACX )
        std::system( QString("rm %1/.web_author_preview.html").arg( _settingWebAuthorTempPath ).toStdString().c_str() ); //remove the preview page
        #elif defined ( Q_OS_WIN)
        std::system( QString("del C:\\Users\\Public\\.web_author_preview.html").arg( _settingWebAuthorTempPath ).toStdString().c_str() ); //remove the preview page
        #else
        #error "Unsupported System"
        #endif
    }
}

void WebAuthor::colorPickerPickedFromList(const QModelIndex &index) {
    if ( index.isValid() ) {
        if ( index.row() >= 0 && index.row() < colorPickerModel->count ) {
            QColor color_picked = colorPickerModel->color_properties.at( index.row() );
            colorPickerSetText( color_picked.red(), color_picked.green(), color_picked.blue() );
        }
    }
}

void WebAuthor::switchToMain() {
    markupView->setFocus();
    currentlyActive = 0;
}

void WebAuthor::switchToStyle() {
    if ( cssDock->isVisible() ) {
        cssView->setFocus();
        currentlyActive = 1;
    }
}

void WebAuthor::switchToScript() {
    if ( jsDock->isVisible() ) {
        jsView->setFocus();
        currentlyActive = 2;
    }
}

void WebAuthor::showCodeCompletion() {
    if ( currentlyActive != -1 ) {
        switch ( currentlyActive ) {
            case 0:
                markupView->showCodeCompletion();
            break;
            case 1:
                cssView->showCodeCompletion();
                break;
            case 2:
                jsView->showCodeCompletion();
                break;
            default:
            break;
        }
    }

}

void WebAuthor::colorPickerAdjustRed(int) {
    colorApplySlider();
}

void WebAuthor::colorPickerAdjustGreen(int) {
    colorApplySlider();
}

void WebAuthor::colorPickerAdjustBlue(int) {
    colorApplySlider();
}

void WebAuthor::colorApplySlider() {
    int red = colorPickerRedValue->value();
    int green = colorPickerGreenValue->value();
    int blue = colorPickerBlueValue->value();
    colorPickerSetText( red, green, blue );
}

void WebAuthor::showLibrarian() {
    librarianWidget->show();
}

void WebAuthor::showTextSearch() {
    bool isVisible = !textSearch->isVisible();
    if ( isVisible ) {
        textSearch->setText( "" );
        textSearch->move( this->pos() );
        textSearch->show();
    } else {
        textSearch->close();
    }
}

void WebAuthor::textSearchStringChanged(const QString &searchString) {
    if ( !searchString.isEmpty() ) {
        markupView->unselectText();
        cssView->unselectText();
        jsView->unselectText();

        QString search = textSearch->text();

        if ( !markupView->find( search ) ) markupView->find( search, QTextDocument::FindBackward );
        if ( !cssView->find( search )) cssView->find( search, QTextDocument::FindBackward );
        if ( !jsView->find( search )) jsView->find( search, QTextDocument::FindBackward );
    }
}

void WebAuthor::toggleLineNumber() {
    _settingLineNumberShown = !_viewMenuLineNumber->isChecked();

    if ( _settingLineNumberShown ) {
        markupEditor->setLineNumber( true );
        cssEditor->setLineNumber( true );
        jsEditor->setLineNumber( true );
    } else {
        markupEditor->setLineNumber( false );
        cssEditor->setLineNumber( false );
        jsEditor->setLineNumber( false );
    }

    QMessageBox::information( this, tr( "WebAuthor" ), tr( "Restart WebAuthor to apply your new configuration." ) );

    this->setPreferences();
}

void WebAuthor::focusToMarkup() {
    currentlyActive = 0;
}

void WebAuthor::focusToStylesheet() {
    currentlyActive = 1;
}

void WebAuthor::focusToScript() {
    currentlyActive = 2;
}
