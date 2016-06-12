#ifndef GW2PACKET_H
#define GW2PACKET_H

#include <cstring>

enum GW2PacketType {
    GWPT_PLAIN,
    GWPT_TLS,
    GWPT_UNKNOWN
};

class GW2Packet
{
public:
    GW2Packet(const char* Buffer, size_t BufferSize);

public:
    GW2PacketType   GetType();

private:
    const char*     m_pBuffer;
    size_t          m_BufferSize;
};

#endif // GW2PACKET_H
