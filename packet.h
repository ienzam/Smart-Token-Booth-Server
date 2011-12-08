#ifndef PACKET_H
#define PACKET_H

#include <QString>

class Packet
{
private:
    char rawData[18];
public:
    Packet(const char *inData = NULL, int size = 17) {
        rawData[17] = '\n';
        if(inData == NULL) return;
        if(size > 17) size = 17;
        strncpy(rawData, inData, size);
    }

    bool isValidChecksum() {
        return strncmp(checkSum(), "------", 6) == 0;
    }

    char &receiverType()     { return rawData[0]; }
    char &receiverID()       { return rawData[1]; }
    char &senderType()       { return rawData[2]; }
    char &senderID()         { return rawData[3]; }
    char &commandType()      { return rawData[4]; }
    char *data()             { return rawData+5;  }
    char *checkSum()         { return rawData+10; }
    char &extraByte()        { return rawData[16];}
    void genChecksum()       { strncpy(checkSum(), "------", 6); }
    const char *getRawData() { return rawData; }
    int size()               { return 18; }
};

#endif // PACKET_H
