#include "wbcodecompletionmodel.h"

//Code completion ( added on 03/10/17 )
WBCodeCompletionModel::WBCodeCompletionModel( const QString &filename, QObject *parent)
    : QAbstractListModel( parent ) {
    this->loadFromFile( filename );
}

void WBCodeCompletionModel::loadFromFile(const QString &filename) {
    QFile resource_file( filename );
    if ( resource_file.open( QIODevice::ReadOnly ) ) {
        QString data = resource_file.readLine();
        items = data.split( ',' );
        resource_file.close();
    }
}

int WBCodeCompletionModel::rowCount(const QModelIndex &) const {
    return this->items.size();
}

QVariant WBCodeCompletionModel::data(const QModelIndex &index, int role) const {
    if ( role == Qt::DisplayRole ) return items.at(index.row());
    else return QVariant();
}
