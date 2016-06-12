#ifndef LOGINSESSION_H
#define LOGINSESSION_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include "securelogin.h"
#include "../Networking/xmlpacket.h"
#include "../Networking/clientconnection.h"

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
private:
    ClientConnection* m_Client;
private:
    int m_ConnectionType;
    int m_Program;
    int m_Build;
    int m_Process;
    char m_TLSSendBuffer[4096];
    int m_TLSSendBufferLength;
    bool m_TLSSendNeeded;
public:
    bool m_TSLReady;
};

#endif // LOGINSESSION_H
