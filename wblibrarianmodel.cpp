#include "wblibrarianmodel.h"

WBLibrarianModel::WBLibrarianModel( const QString &filename, QObject *parent )
    : QAbstractListModel( parent )
{
    if ( filename != NULL ) {
        this->loadLibrarianFile( filename ); //if a librarian file is provided, load it
    }
}

QString WBLibrarianModel::getItemContentAt(int index) {
    if ( index >= 0 && index < counter ) {
        return this->librarian_item_names.at( index );
    } else return QString( );
}

QString WBLibrarianModel::getItemNameAt(int index) {
    if ( index >= 0 && index < counter ) {
        return this->librarian_item_contents.at( index );
    } else return QString();
}

int WBLibrarianModel::rowCount(const QModelIndex &) const {
    return this->counter;
}

QVariant WBLibrarianModel::data(const QModelIndex &index, int role) const {
    if ( role == Qt::DisplayRole ) return this->librarian_item_names.at( index.row() );
    else return QVariant();
}

void WBLibrarianModel::insertData(QString name, QString content) {
    this->librarian_item_names << name;
    this->librarian_item_contents << content;
    counter ++;
}

void WBLibrarianModel::removeData(int index) {
    if ( index >= 0 && index < counter ) {
        this->librarian_item_names.removeAt( index );
        this->librarian_item_contents.removeAt( index );
        counter --;
    }
}

void WBLibrarianModel::loadLibrarianFile(const QString &filename) {
    librarian_item_names.clear();
    librarian_item_contents.clear();
    counter = 0;

    QFile librarianFile( filename );
    if ( librarianFile.open( QIODevice::ReadOnly )) {

        QTextStream *librarian_stream = new QTextStream( &librarianFile );
        bool shouldContinue = true;
        QStringList librarian_data = librarian_stream->readAll().split( "$$$" );

        for ( int i = 0; i < librarian_data.size(); ++i ) {
            if ( shouldContinue ) {
                QStringList current_item_data = librarian_data[i].split( "%%%" );
                if ( current_item_data.size() == 2 ) {
                    //read data from the current line
                    QString librarian_current_item_title = current_item_data[0];
                    QString librarian_current_item_content = current_item_data[1];

                    //load data to array
                    insertData( librarian_current_item_title, librarian_current_item_content );

                } else {
                    shouldContinue = false; //cancel read operation
                }
            } else return;
        }

        librarianFile.close();
    }

}

void WBLibrarianModel::setCounter(int count) {
    this->counter = count;
}

void WBLibrarianModel::saveLibrarianFile(const QString &filename) {
    QFile librarianFile( filename );
    if ( librarianFile.open( QIODevice::WriteOnly )) {
        QTextStream librarian_stream( &librarianFile );
        for ( int i = 0; i < counter; ++i ) {
            librarian_stream << librarian_item_names[ i ] << "%%%" << librarian_item_contents[ i ] << "$$$";
        }
        librarianFile.close();
    }
}

QStringList WBLibrarianModel::get_filtered_names(QList<int> indexes) {
    QStringList filteredArray;

    foreach ( int index, indexes ) {
        filteredArray << librarian_item_names[ index ];
    }

    return filteredArray;
}

QStringList WBLibrarianModel::get_filtered_contents(QList<int> indexes) {
    QStringList filteredArray;

    foreach ( int index, indexes ) {
        filteredArray << librarian_item_contents[ index ];
    }

    return filteredArray;
}

QList<int> WBLibrarianModel::getCompatibleItemIndexes(QString filter) {
    QList<int> indexes;

    for ( int i = 0; i < counter; ++i ) {
        QString current_item_name = librarian_item_names.at( i );
        QString current_item_content = librarian_item_contents.at( i );

        //if it finds the filter string, append the index
        if ( current_item_name.contains( filter ) || current_item_content.contains( filter, Qt::CaseInsensitive ) ) {
            indexes.append( i );
        }
    }

    return indexes;
}

void WBLibrarianModel::dataReset() {
    librarian_item_names.clear();
    librarian_item_contents.clear();
    counter = 0;
}
