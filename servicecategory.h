#ifndef SERVICECATEGORY_H
#define SERVICECATEGORY_H

#include <QList>
#include "config.h"
#include <QPair>
#include <QHash>

//#include <QDebug>

class ServiceCategory
{
    int curIndex;
    int waitingCount;
    char categoryID;
    static QHash<char, ServiceCategory*> id2cat;
    ServiceCategory(char catID) : curIndex(1), waitingCount(0), categoryID(catID) { } //qDebug() << "Creating " << catID; }
public:
    static ServiceCategory* instance(char catID) {
        if(!id2cat.contains(catID)) id2cat[catID] = new ServiceCategory(catID);
        return id2cat[catID];
    }
    char getCategoryID() { return categoryID; }
    int getNextWaitingUserID() { return curIndex; }
    int getNextTokenID() { return curIndex + waitingCount; }
    bool isUserWaiting() { return waitingCount != 0; }
    void addWaitingUser() { waitingCount++; }
    void removeWaitingUser() { curIndex++; waitingCount--; } //if(waitingCount == 0) curIndex = 1; }

    static QList< QPair<QString, char> > getCategoryList() {
        QList< QPair<QString, char> > categories;
        categories << qMakePair(QString("Withdraw"), STB_CATEGORY_WITHDRAW);
        categories << qMakePair(QString("Help"), STB_CATEGORY_HELP);
        categories << qMakePair(QString("Deposit"), STB_CATEGORY_DEPOSIT);
        return categories;
    }
};

#endif // SERVICECATEGORY_H
