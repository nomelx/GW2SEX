#ifndef MITMSOCKET_H
#define MITMSOCKET_H

#include "../Networking/networking.h"
#include "../Networking/xmlpacket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../Util/system.h"

class MitmSocket
{
public:
    MitmSocket();

public:
    bool                Create();
    bool                IsCreated() {return m_Created;}
    bool                TLSReady() {return m_TLSReady;}
    bool                TLSEstablished() {return m_TLSEstablished;}
    bool                EstablishTLSSession();

public:
    //                  When data is recieved from the client to the MITM server...
    bool                OnServerRecieve(XMLPacket *Packet, const char* Data, size_t DataSize);
    void                SendTLSToANet(const char* Buffer, size_t BufferSize);

private:
    void                SendToANet(const char* Buffer, size_t BufferSize);
    void                WaitForANet();
    void                FlushANet();
    void                OnTLSStart(XMLPacket *Packet);

private:
    bool                m_Created;
    bool                m_TLSRequested;
    bool                m_TLSReady;
    bool                m_TLSEstablished;
    bool                m_TLSWantRead;

private:
    int                 m_MitmSocket;
    struct sockaddr_in  m_MitmSocketDescriptor;

private:
    const SSL_METHOD*   m_ClientTLSMethod;
    SSL_CTX*            m_MitmSSLContext;
    SSL*                m_MitmSSLSocket;

};

#endif // MITMSOCKET_H
