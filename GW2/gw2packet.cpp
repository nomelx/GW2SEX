#include "gw2packet.h"

GW2Packet::GW2Packet(const char *Buffer, size_t BufferSize) : m_pBuffer(Buffer), m_BufferSize(BufferSize)
{
    if (m_pBuffer == nullptr) {
        m_BufferSize = 0;
    }
}

GW2PacketType GW2Packet::GetType()
{
    char bufferCode     = m_pBuffer[0];
    GW2PacketType code  = GWPT_UNKNOWN;

    switch(bufferCode) {
        case 'p':
        case 'P':
            code = GWPT_PLAIN;
            break;
    }

    return code;
}
