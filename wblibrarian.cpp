#include "wblibrarian.h"

WBLibrarian::WBLibrarian(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *helpLayout = new QHBoxLayout();

    model = new WBLibrarianModel( );
    filteredModel = new WBLibrarianModel( );

    QWidget *view = new QWidget();
    QVBoxLayout *vlayout = new QVBoxLayout();

    librarianList = new QListView( );
    librarianSearchField = new QLineEdit( );

    //set up search completion
    searchCompleter = new QCompleter( );
    searchCompleter->setModel( model );
    searchCompleter->setCaseSensitivity( Qt::CaseInsensitive );
    searchCompleter->setCompletionMode( QCompleter::PopupCompletion );
    searchCompleter->setCompletionRole( Qt::DisplayRole );
    searchCompleter->setModelSorting( QCompleter::CaseInsensitivelySortedModel );
    searchCompleter->setMaxVisibleItems( 8 );
    searchCompleter->setWidget( librarianSearchField );

    QPushButton *htmlSwitch = new QPushButton( tr( "HTML" ) );
    QPushButton *cssSwitch = new QPushButton( tr( "CSS" ) );
    QPushButton *jsSwitch = new QPushButton( tr( "JS" ) );

    connect( htmlSwitch, SIGNAL(clicked(bool)), SLOT(switchToHTML()) );
    connect( cssSwitch, SIGNAL(clicked(bool)), SLOT(switchToCSS() ) );
    connect( jsSwitch, SIGNAL(clicked(bool)), SLOT(switchToJS()) );

    QWidget *switcherView = new QWidget();
    QHBoxLayout *slayout = new QHBoxLayout( );

    slayout->addWidget( htmlSwitch );
    slayout->addWidget( cssSwitch );
    slayout->addWidget( jsSwitch );

    switcherView->setLayout( slayout );

    librarianSearchField->setPlaceholderText( tr( "Search Library" ) );

    vlayout->addWidget( librarianSearchField );
    vlayout->addWidget( librarianList );
    vlayout->addWidget( switcherView );

    view->setLayout( vlayout );

    librarianDoc = new QTextBrowser( );

    //set up layout
    helpLayout->addWidget( view, 30 );
    helpLayout->addWidget( librarianDoc, 100 );

    model->loadLibrarianFile( ":/html_doc.wblibrarian" );
    librarianList->setModel( model );

    this->setLayout( helpLayout );
    this->setMinimumSize( 810, 440 );
    this->resize( 810, 440 );
    this->setWindowTitle( tr( "Librarian Document Browser" ) );

    //set connections
    connect( searchCompleter, SIGNAL(activated(QString)), SLOT(handleComplete(QString)) );
    connect( librarianSearchField, SIGNAL(textChanged(QString)), SLOT(handleFilterChange(QString)) );
    connect( librarianList, SIGNAL(clicked(QModelIndex)), SLOT(handleIndexChange(QModelIndex)) );
}

void WBLibrarian::handleFilterChange( const QString &searchString ) {
    if ( searchString.isEmpty() ) {

        //index selection will use the original model
        usingFilteredTable = false;

        //if filter text is empty, move back to the original model
        librarianList->setModel( model );
        refreshTable();
    } else {

        //show search completer
        QRect completionRect = librarianSearchField->rect();
        searchCompleter->setCompletionPrefix( searchString );

        searchCompleter->complete( completionRect );

        //index selection will use the filtered model
        usingFilteredTable = true;

        //perform filter
        QList<int> compatible_items = model->getCompatibleItemIndexes( searchString );
        QStringList filtered_names = model->get_filtered_names( compatible_items );
        QStringList filtered_content = model->get_filtered_contents( compatible_items );

        //update filteredModel
        filteredModel->librarian_item_names = filtered_names;
        filteredModel->librarian_item_contents = filtered_content;
        filteredModel->setCounter( compatible_items.length() );

        //respond to the user's query
        librarianList->reset();
        librarianList->setModel( filteredModel );
        refreshTable();
    }
}

void WBLibrarian::switchToHTML( ) {
   model->loadLibrarianFile( ":/html_doc.wblibrarian" );
   librarianSearchField->setText( "" );
   refreshTable();
}

void WBLibrarian::switchToCSS( ) {
   model->loadLibrarianFile( ":/css_doc.wblibrarian" );
   librarianSearchField->setText( "" );
   refreshTable();
}

void WBLibrarian::switchToJS( ) {
   model->loadLibrarianFile( ":/js_doc.wblibrarian" );
   librarianSearchField->setText( "" );
   refreshTable();
}

void WBLibrarian::handleComplete(const QString completeSearch) {
    librarianSearchField->setText( completeSearch );
}

void WBLibrarian::handleIndexChange(const QModelIndex &index) {
    if ( index.isValid() ) {
        QString selectedEntry = ( usingFilteredTable ? filteredModel : model )->librarian_item_contents[index.row()];
        librarianDoc->setHtml( selectedEntry );
    }
}

void WBLibrarian::refreshTable() {
    this->librarianList->reset();
}
