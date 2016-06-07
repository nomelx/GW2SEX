#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

/*
 * GW2SEX - Guild Wars 2 Server Emulator For xnix
 *
 * Developed by Nomelx
 * */

#include <stdio.h>
#include <cerrno>

#include "networking.h"

enum RecieveError {
    RE_UNK = -1,
    RE_NOTHING_THERE = -2,
    RE_CLIENT_CLOSED = -3
};

class ClientConnection
{
public:
    ClientConnection();
    virtual ~ClientConnection();
public:
    void Send(const char* Data, unsigned int DataSize);
    int Poll(char* Buffer, unsigned int BufferSize);
public:
    virtual void Close();
    virtual bool IsConnected();
public:
    char m_ClientIP[INET_ADDRSTRLEN];
    int m_ClientSocket;
    int m_ClientPriority;
};

#endif // CLIENTCONNECTION_H
