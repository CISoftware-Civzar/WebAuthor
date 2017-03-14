#ifndef WBCODECOMPLETIONMODEL_H
#define WBCODECOMPLETIONMODEL_H

#include "includes.h"

class WBCodeCompletionModel : public QAbstractListModel {
    Q_OBJECT
public:
    WBCodeCompletionModel( const QString &filename = "", QObject *parent = 0 );
    QStringList items;

    void loadFromFile( const QString &filename );

    int rowCount(const QModelIndex &parent ) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
};


#endif // WBCODECOMPLETIONMODEL_H
