#include "boothserver.h"
#include <QIODevice>
#include <QTimer>

#include <QDebug>

#include "packet.h"
#include "config.h"
#include "booth.h"
#include "servicecategory.h"
#include "boothservice.h"
#include "boothtoken.h"

BoothServer::BoothServer(QIODevice *ioDev, QObject *parent) :
    QObject(parent),
    ioDev(ioDev),
    curBoothNum(0)
{
    Booth *b = new BoothToken(3);
    availBooths.push_back(b);

    for(int i = 1; i <= NUM_SERVICE_BOOTH; i++) {
        Booth *b = new BoothService(i);
        availBooths.push_back(b);
    }

    waitTimer.setSingleShot(true);
    connect(&waitTimer, SIGNAL(timeout()),this, SLOT(onDeviceTimeout()));

    connect(ioDev, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    pingNextBooth();
}

BoothServer::~BoothServer() {
    foreach(Booth *booth, availBooths) delete booth;
}

void BoothServer::onReadyRead() {
    static QByteArray buffer = "";
    QByteArray b = ioDev->readAll();
    buffer += b;
    if(b.length()) QTimer::singleShot(STB_NEXTCHAR_INTERVAL, this, SLOT(onReadyRead()));
    else if(buffer.size()) {
        processIncomingData(buffer);
        buffer = "";
    }
}

void BoothServer::sendPacket(Packet packet) {
    packet.senderID() = STB_SELF_ID;
    packet.senderType() = STB_ALIAS_SERVER;
    packet.genChecksum();

    qDebug() << " Pre-Write : " << QByteArray(packet.getRawData(), packet.size() - 1);
    ioDev->write(packet.getRawData(), packet.size());
    qDebug() << "Post-Write : " << QByteArray(packet.getRawData(), packet.size() - 1);
}

Booth* BoothServer::currentBooth() { return availBooths[curBoothNum]; }

void BoothServer::incrementCurrentBooth() { curBoothNum = (curBoothNum + 1) % availBooths.size(); }

void BoothServer::pingNextBooth() {
    incrementCurrentBooth();
    pingCurrentBooth();
}

void BoothServer::pingCurrentBooth() {
    waitTimer.start(STB_TIMEOUT);
    isWaiting = true;
    sendPacket(currentBooth()->getPingPacket());
}

void BoothServer::onDeviceTimeout() {
    currentBooth()->onTimout();
    isWaiting = false;
    pingNextBooth();
}

void BoothServer::processIncomingData(QByteArray data) {

    //disconnect(ioDev, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    Packet p(data.constData(), data.size());

    qDebug() << "Read Data : " << p.senderType()<< p.senderID() << data << endl;

    if(!p.isValidChecksum()) {//error check
        qDebug() << "Checksum error";
        return;
    }

    if(p.receiverType() != STB_ALIAS_SERVER || p.receiverID() != STB_SELF_ID) {
        qDebug() << "Me not receiver :-o";
        return;
    }

    if(p.senderType() != currentBooth()->getType() || p.senderID()-'0' != currentBooth()->getID()) {
        qDebug() << "Sender is not expected device";
        return;
    }

    if(!isWaiting) {
        qDebug() << "Why I received a packet!!!";
        return;
    }

    isWaiting = false;
    bool needNextSlot = currentBooth()->processPacket(p);
    if(needNextSlot) pingCurrentBooth();
}
