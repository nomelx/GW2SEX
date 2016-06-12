#ifndef SECURELOGIN_H
#define SECURELOGIN_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <openssl/ssl.h>

#include "../Networking/xmlpacket.h"
#include "../Networking/clientconnection.h"
#include "../Util/system.h"

#include "serverssl.h"

// Tells us the state of the TLS session
enum LoginState {
    LS_NONE,
    LS_WAIT_HANDSHAKE,
    LS_TLS_HANDSHAKE_SENT,
    LS_TLS_FAIL_HANDSHAKE,
    LS_TLS_WANT_READ_WRITE
};

enum RecieveType {
    RT_TLS_HANDSHAKE,
    RT_TLS_LOGIN_PACKET,
    RT_TLS_FAIL,
    RT_TLS_WAIT
};

/*
 * Secure Login
 *
 * This class handles the binary requests from the client when logging in.
 */

class SecureLogin
{
public:
    SecureLogin(ClientConnection* Client);
public:
    //bool Recieve(char* RecvBuffer, int Length, ServerSSL* SSL);
    RecieveType         Recieve(ServerSSL* SSL);
    void                Send(const char *Data, size_t DataSize);
    LoginState          GetState() const;
    const char*         GetBuffer() const;
private:
    bool                EstablishTLS();
    bool                ClientPacket();
    static int          SSL_HandshakeFunction(SSL* SslSession);
private:
    ClientConnection*   m_Client;
    LoginState          m_State;
private:
    char                m_ClientSeed[256];
    char                m_DecodeBuffer[4096];
    int                 m_SeedSize;
    SSL*                m_SSL;
};

#endif // SECURELOGIN_H
