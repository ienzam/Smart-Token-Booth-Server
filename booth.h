#ifndef BOOTH_H
#define BOOTH_H

#include "packet.h"
#include "config.h"
class Booth
{
    char type;
    char id;
public:
    Booth(char type, char id) :type(type), id(id) {}
    char getType() { return type; }
    char getID() { return id; }

    Packet getSkeletonPacket(char serviceType='-') {
        Packet packet;
        packet.receiverType() = getType();
        packet.receiverID() = getID()+'0';
        packet.commandType() = serviceType;
        strncpy(packet.data(), "-----", 6);
        packet.extraByte() = '-';
        return packet;
    }

    virtual Packet getPingPacket() = 0;

    virtual bool processPacket(Packet &packet) = 0; //return if you need next slot
    virtual void onTimout() {}
};

#endif // BOOTH_H
