#include "gw2common.h"

bPacket ReadbPacket(const char* Data) {
    bPacket packet;
    char type = Data[0];
    short ident, size;
    memcpy(&ident, Data+1, sizeof(short));
    memcpy(&size, Data+3, sizeof(short));
    packet.m_Type = type;
    packet.m_Ident = ntohs(ident);
    packet.m_Size = ntohs(size);
    return packet;
}


bSubPacket ReadbSubPacket(const char *Data)
{
    bSubPacket packet;
    char type = Data[0], unk = Data[1];
    short size;
    memcpy(&size, Data+2, sizeof(short));
    packet.m_Type = type;
    packet.m_UNK = unk;
    packet.m_Size = ntohs(size);
    return packet;
}
