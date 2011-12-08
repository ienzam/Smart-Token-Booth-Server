#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QtSql>
#include <QDateTime>

class DataModel
{
public:
    static QSqlDatabase db;
    static bool initDB()
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("sonali.db");
        return db.open();
    }

    static void addTokenEntry(char *tokenStr) {
        QString q=QString("insert into history (token,arrive_time,service) values(\'%1\',\'%2\',\'%3\')")
                .arg(tokenStr).arg(QDateTime::currentDateTimeUtc().toTime_t()).arg(tokenStr[0]);
        QSqlQuery(q).exec();
    }
    static void serviceStarted(char *tokenStr, char serviceBoothID) {
        QString q = QString("update history set start_time=%1,booth_id=%2 where token=%3")
                .arg(QDateTime::currentDateTimeUtc().toTime_t()).arg(serviceBoothID).arg(tokenStr);
        QSqlQuery(q).exec();
    }

    static void serviceStopped(char *tokenStr) {
        QString q = QString("update history set end_time=%1 where token=%3")
                .arg(QDateTime::currentDateTimeUtc().toTime_t()).arg(tokenStr);
        QSqlQuery(q).exec();
    }
};

#endif // DATAMODEL_H
