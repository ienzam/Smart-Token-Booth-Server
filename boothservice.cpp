#include "boothservice.h"
#include "servicecategory.h"
#include "datamodel.h"
#include <stdio.h>
#include <QSettings>
#include <QDebug>

BoothService::BoothService(char iid)
    : Booth(STB_ALIAS_SERVICEBOOTH, iid),
      free(false),
      online(false)
{
    QSettings settings;
    settings.beginReadArray("service_booth");
    settings.setArrayIndex(iid);
    char catID = settings.value("category", "A").toInt();
    settings.endArray();

    qDebug() << "Creating service booth "<<iid<< " with category " << catID;

    serviceCategory = ServiceCategory::instance(catID);
}

Packet BoothService::getPingPacket() {
    Packet packet;
    if(isFree() && serviceCategory->isUserWaiting()) {
        packet = getSkeletonPacket(STB_SERVER_NEW_USER);

        sprintf(tokenStr, "%c%04d", serviceCategory->getCategoryID(), serviceCategory->getNextWaitingUserID());

        DataModel::serviceStarted(tokenStr, getID());

        strncpy(packet.data(), tokenStr, 5);
    } else {
        packet = getSkeletonPacket(STB_SERVER_PING_SERVICE);
    }
    return packet;
}

bool BoothService::processPacket(Packet &packet) {
    bool nowFree = isFree();
    switch(packet.commandType()) {
    case STB_SERVICE_FREE:
        isFree() = true;
        if(!nowFree) {
            DataModel::serviceStopped(tokenStr);
            return true;
        }
        break;
    case STB_SERVICE_BUSY:
        isFree() = false;
        break;
    case STB_SERVICE_TAKEN_USER:
        serviceCategory->removeWaitingUser();
        isFree() = false;
        break;
    }
    isOnline() = true;
    return false;
}

void BoothService::onTimeout() {
    isOnline() = false;
}
