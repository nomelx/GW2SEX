#ifndef LOGINSESSION_H
#define LOGINSESSION_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include "../Networking/xmlpacket.h"
#include "../Networking/clientconnection.h"

class LoginSession
{
public:
    LoginSession(ClientConnection* Client);
public:
    bool Recieve(XMLPacket* Packet);
    void Send();
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
public:
    bool m_TSLReady;
};

#endif // LOGINSESSION_H
