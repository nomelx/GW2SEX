#ifndef LOGINSERVER_H
#define LOGINSERVER_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <vector>
#include <stdint.h>
#include <thread>
#include <mutex>

#include "../Networking/serversocket.h"

#include "loginclient.h"
#include "serverssl.h"

class LoginServer
{
public:
    LoginServer();
public:
    bool                        Startup(uint16_t ServerPort, const char *Certificate, const char *PrivateKey, bool MitmMode);
    void                        Update();
    void                        Shutdown();
private:
    // Function used to spawn the login socket server, do not call directly from main thread.
    static void                 Run(LoginServer* Instance);
private:
    bool                        m_Running;
    bool                        m_MitmMode;
    uint16_t                    m_ServerPort;
    std::thread                 m_loginThread;
private:
    std::mutex                  m_ClientsLock;
    std::vector<LoginClient*>   m_Clients;
private:
    ServerSSL                   m_SSL;
};

#endif // LOGINSERVER_H
