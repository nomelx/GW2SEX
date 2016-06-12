#include "mitmsocket.h"

MitmSocket::MitmSocket() : m_Created(false), m_TLSRequested(false), m_TLSReady(false), m_TLSEstablished(false), m_TLSWantRead(false),
    m_MitmSocket(-1), m_MitmSocketDescriptor(), m_ClientTLSMethod(nullptr), m_MitmSSLContext(nullptr), m_MitmSSLSocket(nullptr)
{

}

bool MitmSocket::Create()
{
    // Check to see if the socket is already created
    if (m_MitmSocket != -1) {
        printf("<MITM> Trying to connect to NCSoft servers when a connection already exists.\n");
        return false;
    }

    // Reserve our socket!
    m_MitmSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Initalize an instance of sockaddr_in for connecting to NCSoft.
    memset(&m_MitmSocketDescriptor, 0, sizeof(m_MitmSocketDescriptor));
    m_MitmSocketDescriptor.sin_family       = AF_INET;
    m_MitmSocketDescriptor.sin_addr.s_addr  = inet_addr ("64.25.40.110");
    m_MitmSocketDescriptor.sin_port         = htons(6112);

    // Try connecting to NCSoft servers.
    auto connectError = connect(m_MitmSocket, (struct sockaddr*)&m_MitmSocketDescriptor, sizeof(m_MitmSocketDescriptor));
    if (connectError == -1) {
        printf("<MITM> connect() failed!");
        memset(&m_MitmSocketDescriptor, 0, sizeof(m_MitmSocketDescriptor));
        m_MitmSocket = -1;
        return false;
    }

    // Set socket to non blocking
    fcntl(m_MitmSocket, F_SETFL, O_NONBLOCK);

    // Flag that all went well and return.
    m_Created = true;
    return true;
}

bool MitmSocket::EstablishTLSSession()
{
    if (!m_TLSWantRead) {
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();

        m_ClientTLSMethod = SSLv23_client_method();
        if (m_ClientTLSMethod == nullptr) return false;

        m_MitmSSLContext = SSL_CTX_new(m_ClientTLSMethod);
        if (m_MitmSSLContext == nullptr) return false;

        m_MitmSSLSocket = SSL_new (m_MitmSSLContext);
        if (m_MitmSSLSocket == nullptr) return false;

        SSL_set_fd (m_MitmSSLSocket, m_MitmSocket);
        SSL_set_connect_state(m_MitmSSLSocket);
    }

    auto sslConnectError = SSL_connect(m_MitmSSLSocket);
    if (sslConnectError == -1) {

        auto sslConnectErrorCode = SSL_get_error(m_MitmSSLSocket, sslConnectError);
        //ERR_print_errors_fp(stderr);

        if (sslConnectErrorCode == SSL_ERROR_WANT_READ) {
            printf("<MITM> SSL_connect issued SSL_ERROR_WANT_READ, this means there was an error in the communication with ANet.\n");
            m_TLSWantRead = true;
            return false;
            //return false;
        } else {
            printf("<MITM> SSL_connect failed with %d, this means there was an error in the communication with ANet.\n", sslConnectErrorCode);
            //return false;
        }

        m_TLSReady = false;
        m_TLSEstablished = false;

        return false;
    }

    m_TLSEstablished = true;
    return true;
}

bool MitmSocket::OnServerRecieve(XMLPacket *Packet, const char *Data, size_t DataSize)
{
    if (strncmp(Packet->m_Path, "/Sts/Connect STS/1.0", sizeof("/Sts/Connect STS/1.0")) == 0) {
        SendToANet(Data, DataSize);
    }

    if (strncmp(Packet->m_Path, "/Auth/StartTls STS/1.0", sizeof("/Auth/StartTls STS/1.0")) == 0) {
        OnTLSStart(Packet);
        SendToANet(Data, DataSize);
        WaitForANet(); // Reply
        m_TLSReady = true;
    }

    return true;
}

void MitmSocket::SendToANet(const char *Buffer, size_t BufferSize)
{
    if (m_MitmSocket < 0) {
        return;
    }

    unsigned int totalSent = 0;
    unsigned int toSend = BufferSize;

    while (totalSent != BufferSize) {
        //setsockopt(m_ClientSocket, SOL_SOCKET, SO_SNDBUF,  &DataSize, sizeof(unsigned int));
        int sent = send(m_MitmSocket, Buffer, BufferSize, 0);
        totalSent += sent;
        toSend -= toSend;
        if (sent < 0) {
            close(m_MitmSocket);
            m_MitmSocket = -1;
            break;
        }
    }
}

void MitmSocket::SendTLSToANet(const char *Buffer, size_t BufferSize)
{
    if (BufferSize <= 0)
        return;
    auto sendError = SSL_write(m_MitmSSLSocket, Buffer, BufferSize);
    if (sendError == -1) {
        auto sslWriteErrorCode = SSL_get_error(m_MitmSSLSocket, sendError);
        if (sslWriteErrorCode == SSL_ERROR_WANT_READ) {
            printf("<MITM> SSL_write issued SSL_ERROR_WANT_READ, please flush recieve buffers first.\n");
        }
        else {
            printf("<MITM> SSL_write error, please check connection logs.\n");
            ERR_print_errors_fp(stderr);
        }
    }
}

void MitmSocket::WaitForANet()
{
    int recieveCode = -1;
    do {
        char buff[4096];
        recieveCode = recv(m_MitmSocket, buff, 4096, 0);
    } while (recieveCode == -1);
}

void MitmSocket::FlushANet()
{
    char buff[4096];
    int res = 0;
    do {
        res = recv(m_MitmSocket, buff, 4096, 0);
    } while(res != -1);
}

void MitmSocket::OnTLSStart(XMLPacket *Packet)
{
    printf("<MITM> TLS session request sent to ANet.\n");
    m_TLSRequested = true;
}
