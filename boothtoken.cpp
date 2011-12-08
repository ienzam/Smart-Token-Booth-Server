#include "boothtoken.h"
#include "servicecategory.h"

#include <stdio.h>
#include <QDebug>
#include <assert.h>
#include "datamodel.h"

BoothToken::BoothToken(char iid)
    : Booth(STB_ALIAS_TOKENBOOTH, iid),
      waitingForAdding(false),
      lastCatID(0),
      lastTokenNum(0)
{
}

Packet BoothToken::getPingPacket() {
    Packet packet;
    if(lastCatID) { //processed the last category
        packet = getSkeletonPacket(STB_SERVER_DONE_USER);
        char tokenStr[10];
        sprintf(tokenStr, "%c%04d", lastCatID, lastTokenNum);

        DataModel::addTokenEntry(tokenStr);

        strncpy(packet.data(), tokenStr, 5);
    } else {
        packet = getSkeletonPacket(STB_SERVER_PING_TOKEN);
    }
    return packet;
}

bool BoothToken::processPacket(Packet &packet) {
    if(lastCatID) {
        if(packet.commandType() != STB_TOKEN_DONE_USER) {
            qDebug() << "Expected " << STB_TOKEN_DONE_USER << ", found " << packet.commandType() << endl;
            return false;
        }
        char catID;
        int tokenNum;
        sscanf(packet.data(), "%c%04d", &catID, &tokenNum);
        if(lastCatID != catID || tokenNum != lastTokenNum) {
            qDebug() << "Expected " << lastCatID << "," << lastTokenNum << ", found " << catID << "," << tokenNum << endl;
            return false;
        }

        ServiceCategory::instance(lastCatID)->addWaitingUser();
        lastCatID = 0;
    } else {
        switch(packet.commandType()) {
        case STB_TOKEN_NO_USER:
            break;
        case STB_TOKEN_HAS_USER: {
            char catID = packet.data()[0];

            qDebug() << "Got a new category " << catID;

            lastCatID = catID;
            lastTokenNum = ServiceCategory::instance(lastCatID)->getNextTokenID();
            return true;
            break;
        }
        default:
            qDebug() << "NOT SUPPOSED TO COME HERE";
            assert(false);
            break;
        }
    }
    return false;
}

void BoothToken::onTimout() {
    return;
}
