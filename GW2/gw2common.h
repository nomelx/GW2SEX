#ifndef GW2COMMON
#define GW2COMMON

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <string.h>
#include <netinet/in.h>

#define BPC_PACKET_IDENT 0x0303

enum bPacketType {
    BPT_ACK         = 0x14,
    BPT_SERV_ERROR  = 0x15,
    BPT_AUTH        = 0x16,
    BPT_GAME        = 0x17
};

enum bSubPacketType {
    BSPT_CLIENT_SEED = 0x01
};

struct bPacket {
    char            m_Type;
    short           m_Ident;
    short           m_Size;
    char            m_Reserved;
};

struct bSubPacket {
    char            m_Type;
    char            m_UNK;
    short           m_Size;
};

bPacket ReadbPacket(const char* Data);
bSubPacket ReadbSubPacket(const char* Data);

#endif // GW2COMMON
