#include "wbcolorlistmodel.h"

WBColorListModel::WBColorListModel(QObject *parent)
    : QAbstractListModel( parent ) {
    setupColors();
}

void WBColorListModel::addColor(QString name, int red, int green, int blue) {
    this->color_names << name;
    this->color_properties << QColor( red, green, blue );
    this->count ++;
}

void WBColorListModel::setupColors() {
    addColor( tr( "Red" ), 255, 0, 0 );
    addColor( tr( "Green" ), 0, 255, 0 );
    addColor( tr( "Blue" ), 0, 0, 255 );
    addColor( tr( "Yellow" ), 255, 255, 0 );
    addColor( tr( "Purple" ), 255, 0, 255 );
    addColor( tr( "Pink" ), 255, 24, 116 );
    addColor( tr( "Sky Blue" ), 99, 248, 255 );
    addColor( tr( "Orange" ), 255, 124, 17 );
    addColor( tr( "White" ), 255, 255, 255 );
    addColor( tr( "Black" ), 0, 0, 0 );
    addColor( tr( "Silver" ), 200, 200, 200 );
}

int WBColorListModel::rowCount(const QModelIndex &) const {
    return this->count;
}

QVariant WBColorListModel::data(const QModelIndex &index, int role) const {
    if ( role == Qt::DisplayRole ) return color_names.at( index.row() );
    else return QVariant();
}
