#include "loginsession.h"

const char* g_STS_Ping      = "/Sts/Ping STS/1.0";
const char* g_STS_Connect   = "/Sts/Connect STS/1.0";
const char* g_AUTH_StartTLS = "/Auth/StartTls STS/1.0";
const char* g_AUTH_GetHost = "/Auth/GetHostname STS/1.0";

LoginSession::LoginSession(ClientConnection *Client) : m_Client(Client), m_ConnectionType(0), m_Program(0),
    m_Build(0), m_Process(0), m_TLSSendBuffer(), m_TLSSendBufferLength(0), m_TLSSendNeeded(false), m_TSLReady(false)
{

}

bool LoginSession::Recieve(XMLPacket *Packet)
{
    if (!Packet) {
        return false;
    }

    if (strncmp(Packet->m_Path, g_STS_Ping, sizeof(g_STS_Ping)) == 0) {
        printf("%s -> Ping!\n", m_Client->m_ClientIP);
    }

    else if (strncmp(Packet->m_Path, g_STS_Connect, sizeof(g_STS_Connect)) == 0) {
        Init(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_StartTLS, sizeof(g_AUTH_StartTLS)) == 0) {
        StartTLS(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_GetHost, sizeof(g_AUTH_GetHost)) == 0) {
        GetHostname(Packet);
    }

    else {
        printf("Unknown command: %s\n", Packet->m_Path);
    }

    return true;
}

void LoginSession::Send(SecureLogin *tlsClient)
{
    if (m_TLSSendNeeded) {
        tlsClient->Send(m_TLSSendBuffer, m_TLSSendBufferLength);
        m_TLSSendNeeded = false;
        memset(m_TLSSendBuffer, 0, 4096);
    }
}

void LoginSession::Init(XMLPacket *Packet)
{
    rapidxml::xml_node<>* connect_node = Packet->m_XMLDocument.first_node("Connect");

    m_ConnectionType = atoi(connect_node->first_node("ConnType")->value());
    m_Program = atoi(connect_node->first_node("Program")->value());
    m_Build = atoi(connect_node->first_node("Build")->value());
    m_Process = atoi(connect_node->first_node("Process")->value());

    printf("%s -> Client Query { Type: %d , Program: %d, Build: %d, Process: %d }\n", m_Client->m_ClientIP, m_ConnectionType,
           m_Program, m_Build, m_Process);
}

void LoginSession::StartTLS(XMLPacket *Packet)
{
    char response[512];
    printf("%s -> TLS Session Requested\n", m_Client->m_ClientIP);
    int sequence = Packet->m_Meta[0] - '0';

    // If the client is not blocked
    if (true) {
        sprintf(response, "STS/1.0 400 Success\r\ns:%dR\r\nl:%d\r\n\r\n<Error server=\"1001\" module=\"4\" line=\"262\"/>\n", sequence, 45);
        m_Client->Send(response, strlen(response));
        m_TSLReady = true;
    } else {

    }
}

void LoginSession::GetHostname(XMLPacket *Packet)
{
    rapidxml::xml_node<>* requestNode = Packet->m_XMLDocument.first_node("Request");

    auto loginName = requestNode->first_node("LoginName")->value();
    auto provider = requestNode->first_node("Provider")->value();

    printf("User %s is logging in using %s\n", loginName, provider);

    memset(m_TLSSendBuffer, 0, 4096);
    sprintf(m_TLSSendBuffer, "STS/1.0 200 OK\r\nl:70\r\ns:2R\r\n\r\n<Reply>\n<Hostname>cligate-fra.101.ncplatform.net.</Hostname>\n</Reply>\n",
            strlen("STS/1.0 200 OK\r\nl:70\r\ns:2R\r\n\r\n<Reply>\n<Hostname>cligate-fra.101.ncplatform.net.</Hostname>\n</Reply>\n"));
    m_TLSSendBufferLength = strlen(m_TLSSendBuffer);
    m_TLSSendNeeded = true;

}

