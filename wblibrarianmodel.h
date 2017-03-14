#ifndef WBLIBRARIANMODEL_H
#define WBLIBRARIANMODEL_H

#include "includes.h"

class WBLibrarianModel : public QAbstractListModel
{
public:
    WBLibrarianModel( const QString &filename = NULL, QObject *parent = 0 );
    QStringList librarian_item_names;
    QStringList librarian_item_contents;

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    void setCounter( int count );

    QString getItemNameAt( int index );
    QString getItemContentAt( int index );
    void dataReset();

    void insertData( QString name, QString content );
    void removeData( int index );
    void setFilterString( QString filter );

    //subset extraction functions
    QList<int> getCompatibleItemIndexes( QString filter );
    QStringList get_filtered_names( QList<int> index );
    QStringList get_filtered_contents( QList<int> index );

    //librarian IO operations
    void loadLibrarianFile( const QString &filename );
    void saveLibrarianFile( const QString &filename);

private:
    int counter;
};

#endif // WBLIBRARIANMODEL_H
