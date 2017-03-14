#ifndef WBCOLORLISTMODEL_H
#define WBCOLORLISTMODEL_H

#include "includes.h"

class WBColorListModel : public QAbstractListModel {
    Q_OBJECT
public:
    WBColorListModel( QObject *parent = 0 );

    QStringList color_names;
    QList<QColor> color_properties;
    int count = 0;

    void addColor( QString name, int red, int green, int blue );
    void setupColors();

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};
#endif // WBCOLORLISTMODEL_H
