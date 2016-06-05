#include "serversocket.h"

addrinfo g_HintsInstance = {
    AI_PASSIVE,
    AF_INET,
    SOCK_STREAM,
    IPPROTO_TCP,
    0, 0, 0, 0
};

ServerSocket::ServerSocket() : m_SocketHandle(-1), m_IntfConfig()
{
    memset(m_Port, 0, 6);
}

bool ServerSocket::Configure(uint16_t ServerPort)
{
    System::IgnoreSigPipe();

    addrinfo *serverInfo;
    sprintf(m_Port, "%d", ServerPort);

    int result = getaddrinfo(NULL, m_Port, &g_HintsInstance, &serverInfo);

    if (result != 0) {
        printf("!) getaddrinfo error: %s\n", gai_strerror(result));
        return false;
    }

    m_IntfConfig = *serverInfo;
    freeaddrinfo(serverInfo);

    m_SocketHandle = socket(m_IntfConfig.ai_family, m_IntfConfig.ai_socktype, m_IntfConfig.ai_protocol);

    if (m_SocketHandle == -1) {
        printf("!) socket error: \n");
        return false;
    }

    int yes = 1;
    if (setsockopt(m_SocketHandle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        printf("!) setsockopt error: \n");
        return false;
    }

    result = bind(m_SocketHandle, m_IntfConfig.ai_addr, m_IntfConfig.ai_addrlen);

    if (result == -1) {
        printf("!) bind error: \n");
        return false;
    }

    result = listen(m_SocketHandle, 20);

    if (result == -1) {
        printf("!) listen error: \n");
        return false;
    }

    sockaddr_in socketInformation;
    memset(&socketInformation, 0, sizeof(socketInformation));
    socklen_t len = sizeof(sockaddr);
    if (getsockname(m_SocketHandle, (struct sockaddr *) &socketInformation, &len) != 0) {
        printf("!) getsockname error: \n");
    }

    if (ntohs(socketInformation.sin_port) != ServerPort) {
        printf("Application port does not match requested port, this is a bug. Restart the server.\n");
        return false;
    }

    return true;
}

void ServerSocket::Flush() {

}

ClientConnection ServerSocket::Accept() {
    ClientConnection    newClient;
    sockaddr_storage    incomming_address;
    socklen_t           addr_size = sizeof(incomming_address);

    newClient.m_ClientSocket = accept(m_SocketHandle, (struct sockaddr *)&incomming_address, &addr_size);

    // Do not use blocking calls
    fcntl(newClient.m_ClientSocket, F_SETFL, O_NONBLOCK);

    sockaddr connectingClient;
    socklen_t len = sizeof(connectingClient);
    getpeername(newClient.m_ClientSocket, &connectingClient, &len);

    PrintIP(&connectingClient, "Client Connected: %s\n");
    WriteIP(&connectingClient, newClient.m_ClientIP);

    return newClient;
}

void ServerSocket::PrintIP(const sockaddr *SocketAddress, const char* Message)
{
    char connectingIPString[INET_ADDRSTRLEN];
    switch(SocketAddress->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)SocketAddress)->sin_addr),
                    connectingIPString, INET_ADDRSTRLEN);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)SocketAddress)->sin6_addr),
                    connectingIPString, INET_ADDRSTRLEN);
            break;

        default:
            strncpy(connectingIPString, "Unknown AF", INET_ADDRSTRLEN);
    }

    printf(Message, connectingIPString);
}

void ServerSocket::WriteIP(const sockaddr *SocketAddress, char *Dst)
{
    switch(SocketAddress->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)SocketAddress)->sin_addr),
                    Dst, INET_ADDRSTRLEN);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)SocketAddress)->sin6_addr),
                    Dst, INET_ADDRSTRLEN);
            break;

        default:
            strncpy(Dst, "Unknown AF", INET_ADDRSTRLEN);
    }
}

