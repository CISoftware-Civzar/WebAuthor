#include "webauthor.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WebAuthor w;

    //set up WebAuthor configuration
    QCoreApplication::setOrganizationName( "CI Software" );
    QCoreApplication::setOrganizationDomain( "cisoftware.com" );
    QCoreApplication::setApplicationName( "WebAuthor" );

    w.show();

    return a.exec();
}
