#include "clientconnection.h"

ClientConnection::ClientConnection() : m_ClientIP(), m_ClientSocket(0), m_ClientPriority(0) {

}

ClientConnection::~ClientConnection()
{

}

void ClientConnection::Send(const char *Data, unsigned int DataSize) {
    if (m_ClientSocket < 0) {
        return;
    }

    unsigned int totalSent = 0;
    unsigned int toSend = DataSize;

    fcntl(m_ClientSocket, F_SETFL, O_NONBLOCK);

    while (totalSent != DataSize) {
        //setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDBUF,  &DataSize, sizeof(unsigned int));
        int sent = send(m_ClientSocket, Data, DataSize, 0);
        totalSent += sent;
        toSend -= toSend;
        if (sent < 0) {
            close(m_ClientSocket);
            m_ClientSocket = -1;
            break;
        }
    }

}

int ClientConnection::Poll(char *Buffer, unsigned int BufferSize) {
    int pollResult = recv(m_ClientSocket, Buffer, BufferSize-1, 0);
    if (pollResult == -1 && errno == EWOULDBLOCK) {
        return RE_NOTHING_THERE;
    } else if (pollResult == -1) {
        return RE_UNK;
    } else if (pollResult == 0) {
        close(m_ClientSocket);
        m_ClientSocket = -1;
        return RE_CLIENT_CLOSED;
    }
    return pollResult;
}

void ClientConnection::Close() {
    close(m_ClientSocket);
}

bool ClientConnection::IsConnected() {
    if (m_ClientSocket < 0) {
        return false;
    }
    return true;
}
