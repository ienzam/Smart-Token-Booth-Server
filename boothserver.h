#ifndef BOOTHSERVER_H
#define BOOTHSERVER_H

#include <QObject>
#include <QVector>
#include <QHash>
#include <QTimer>

class Booth;
class Packet;
class QIODevice;
class ServiceCategory;
class BoothServer : public QObject
{
    Q_OBJECT
public:
    explicit BoothServer(QIODevice *ioDev, QObject *parent = 0);
    ~BoothServer();

private:
    QIODevice *ioDev;
    bool isWaiting;
    int curBoothNum;
    QVector<Booth*> availBooths;
    QTimer waitTimer;

    Booth* currentBooth();
    void incrementCurrentBooth();

    void pingNextBooth();
    void pingCurrentBooth();
    void sendPacket(Packet packet);
    void processIncomingData(QByteArray data);

signals:

public slots:
    void onDeviceTimeout();
    void onReadyRead();
};

#endif // BOOTHSERVER_H
