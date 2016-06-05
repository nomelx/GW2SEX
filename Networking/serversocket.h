#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "../Util/system.h"
#include "networking.h"
#include "clientconnection.h"

class ServerSocket
{
public:
    ServerSocket();
public:
    bool Configure(uint16_t ServerPort);
    void Flush();
public: // Blocking call that waits for a connection
    ClientConnection Accept();
private: // Debugging / Terminal helpers
    void PrintIP(const struct sockaddr *SocketAddress, const char* Message = "%s");
    void WriteIP(const struct sockaddr *SocketAddress, char* Dst);
private:
    char m_Port[6];
    int m_SocketHandle;
    addrinfo m_IntfConfig;
};

#endif // SERVERSOCKET_H
