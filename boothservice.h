#ifndef BOOTHSERVICE_H
#define BOOTHSERVICE_H

#include "booth.h"

class ServiceCategory;
class BoothService : public Booth
{
private:
    bool free, online;
    ServiceCategory *serviceCategory;

    char tokenStr[10];
public:
    BoothService(char iid);

    bool& isFree() { return free; }
    bool& isOnline() { return online; }

    virtual Packet getPingPacket();

    virtual bool processPacket(Packet &packet);
    virtual void onTimeout();
};

#endif // BOOTHSERVICE_H
