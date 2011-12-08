#include <QtGui/QApplication>
#include "serialconnectwindow.h"
#include "datamodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("BUET");
    QCoreApplication::setOrganizationDomain("buet.ac.bd");
    QCoreApplication::setApplicationName("STB Server");

    DataModel::initDB();

    QApplication a(argc, argv);
    ConnectWindow w;
    w.show();
    return a.exec();
}
