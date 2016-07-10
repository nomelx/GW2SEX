#ifndef LOGINSESSION_H
#define LOGINSESSION_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include "securelogin.h"
#include "../GW2/gw2packet.h"
#include "../Networking/xmlpacket.h"
#include "../Networking/clientconnection.h"

#include <openssl/bio.h>
#include <openssl/evp.h>

struct SessionSendPacket {
    char m_TLSSendBuffer[4096];
    int m_TLSSendBufferLength;
    bool m_TLSSendNeeded;
};

class LoginSession
{
public:
    LoginSession(ClientConnection* Client);
public:
    bool Recieve(XMLPacket* Packet);
    void Send(SecureLogin* tlsClient);
private:
    void Init(XMLPacket* Packet);
    void StartTLS(XMLPacket* Packet);
    void GetHostname(XMLPacket* Packet);
    void StartSsoLogin(XMLPacket* Packet);
    void ListGameAccounts(XMLPacket* Packet);
    void RequestGameToken(XMLPacket* Packet);
    void Logout(XMLPacket *Packet);
private:
    ClientConnection* m_Client;
private:
    int m_ConnectionType;
    int m_Program;
    int m_Build;
    int m_Process;

private:
    /*char m_TLSSendBuffer[4096];
    int m_TLSSendBufferLength;
    bool m_TLSSendNeeded;*/
    std::vector<SessionSendPacket> m_SendPackets;
public:
    bool m_TSLReady;
};

#endif // LOGINSESSION_H
