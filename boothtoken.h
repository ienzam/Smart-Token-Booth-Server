#ifndef BOOTHTOKEN_H
#define BOOTHTOKEN_H

#include "booth.h"
#include <QHash>
class ServiceCategory;
class BoothToken : public Booth
{
private:
    bool waitingForAdding; //if waiting for to add it into ServiceCategory
    char lastCatID;
    int lastTokenNum;

public:
    BoothToken(char iid);
    bool &isWaitingForAdding() { return waitingForAdding; }

    virtual Packet getPingPacket();

    virtual bool processPacket(Packet &packet);
    virtual void onTimout();
};

#endif // BOOTHTOKEN_H
