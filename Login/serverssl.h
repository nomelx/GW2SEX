#ifndef SERVERSSL_H
#define SERVERSSL_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <openssl/ssl.h>
#include <openssl/err.h>

class ServerSSL
{
public:
    ServerSSL();
public:
    bool Init(const char *ServerCertFile, const char* ServerPrivateKeyFile);
    SSL_CTX* GetContext();
private:
    const SSL_METHOD*   m_Method;
    SSL_CTX*            m_CTX;
};

#endif // SERVERSSL_H
