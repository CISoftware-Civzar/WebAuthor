#ifndef WBLIBRARIAN_H
#define WBLIBRARIAN_H

#include "includes.h"
#include "wblibrarianmodel.h"

class WBLibrarian : public QWidget
{
    Q_OBJECT
public:
    explicit WBLibrarian( QWidget *parent = 0);
    QListView *librarianList;
    QLineEdit *librarianSearchField;
    QTextBrowser *librarianDoc;

    QCompleter *searchCompleter;

    //the data model
    WBLibrarianModel *model;
    WBLibrarianModel *filteredModel;

signals:

private slots:
    void handleIndexChange(const QModelIndex &index);
    void handleFilterChange(const QString &searchString);
    void handleComplete(const QString completeSearch);
    void switchToHTML();
    void switchToCSS();
    void switchToJS();

private:
    void refreshTable();
    bool usingFilteredTable = false;
};

#endif // WBLIBRARIAN_H
