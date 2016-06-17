#include "loginsession.h"

const char* g_STS_Ping              = "/Sts/Ping STS/1.0";
const char* g_STS_Connect           = "/Sts/Connect STS/1.0";
const char* g_AUTH_StartTLS         = "/Auth/StartTls STS/1.0";
const char* g_AUTH_GetHost          = "/Auth/GetHostname STS/1.0";
const char* g_AUTH_StartSsoLogin    = "/Auth/StartSsoLogin STS/1.0";
const char* g_AUTH_ListGameAcc      = "/Account/ListMyGameAccounts STS/1.0";
const char* g_AUTH_Logout           = "/Auth/LogoutMyClient STS/1.0";

LoginSession::LoginSession(ClientConnection *Client) : m_Client(Client), m_ConnectionType(0), m_Program(0),
    m_Build(0), m_Process(0), m_TLSSendBuffer(), m_TLSSendBufferLength(0), m_TLSSendNeeded(false), m_TSLReady(false)
{

}

bool LoginSession::Recieve(XMLPacket *Packet)
{
    if (!Packet) {
        return false;
    }

    if (strncmp(Packet->m_Path, g_STS_Ping, sizeof(Packet->m_Path)) == 0) {
        printf("%s -> Ping!\n", m_Client->m_ClientIP);
    }

    else if (strncmp(Packet->m_Path, g_STS_Connect, sizeof(Packet->m_Path)) == 0) {
        Init(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_StartTLS, sizeof(Packet->m_Path)) == 0) {
        StartTLS(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_GetHost, sizeof(Packet->m_Path)) == 0) {
        GetHostname(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_StartSsoLogin, sizeof(Packet->m_Path)) == 0) {
        StartSsoLogin(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_ListGameAcc, sizeof(Packet->m_Path)) == 0) {
        ListGameAccounts(Packet);
    }

    else if (strncmp(Packet->m_Path, g_AUTH_Logout, sizeof(Packet->m_Path)) == 0) {
        Logout(Packet);
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

void LoginSession::StartSsoLogin(XMLPacket *Packet)
{
    rapidxml::xml_node<>* requestNode = Packet->m_XMLDocument.first_node("Request");

    char password[1024];
    memset(password, 0, 1024);
    int passwordLength = -1;

    char* username = nullptr;
    char* passwordBase64 = nullptr;

    try {
        username = requestNode->first_node("LoginName")->value();
        passwordBase64 = requestNode->first_node("Password")->value();
    }
    catch(std::exception ex)
    {
        printf("Password tokens not supported.\n");
        return;
    }
    auto    bio = BIO_new_mem_buf(passwordBase64, -1);
    auto    b64 = BIO_new(BIO_f_base64());
            bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    passwordLength = BIO_read(bio, password, strlen(passwordBase64));
    BIO_free_all(bio);

    printf("Login >> %s with %s\n", username, password);

    memset(m_TLSSendBuffer, 0, 4096);
    sprintf(m_TLSSendBuffer, "STS/1.0 200 OK\r\ns:2R\r\nl:%d\r\n\r\n<Reply>\n<UserId>0687C32C-0331-E611-80C3-ECB1D78A5C75</UserId>\n<UserCenter>5</UserCenter>\n<UserName>:nomelx.4678</UserName>\n<Parts/>\n<ResumeToken>54582F3B-CAE5-467A-A84B-23109DDA393A</ResumeToken>\n<EmailVerified>1</EmailVerified>\n</Reply>\n",
            strlen("<Reply>\n<UserId>0687C32C-0331-E611-80C3-ECB1D78A5C75</UserId>\n<UserCenter>5</UserCenter>\n<UserName>:nomelx.4678</UserName>\n<Parts/>\n<ResumeToken>54582F3B-CAE5-467A-A84B-23109DDA393A</ResumeToken>\n<EmailVerified>1</EmailVerified>\n</Reply>\n"));
    m_TLSSendBufferLength = strlen(m_TLSSendBuffer);
    m_TLSSendNeeded = true;

}

void LoginSession::ListGameAccounts(XMLPacket *Packet)
{
    rapidxml::xml_node<>* requestNode = Packet->m_XMLDocument.first_node("Request");
    auto gameCode = requestNode->first_node("GameCode")->value();

    /*GW2Packet packet = GW2Packet("/Presence/UserInfo", PT_MESSAGE);
    packet.AddElement("Status", "online");
    packet.AddElement("Status", "online");*/

    printf("Looking up game accounts...\n");

    const char* replyMessageTemp = "P /Presence/UserInfo STS/1.0\r\nl:317\r\n\r\n<Message>\n<Status>online</Status>\n<Aliases/>\n<OnlineTimes>\n<Online scope=\"gw2\" age=\"1781\"/>\n</OnlineTimes>\n<AppData/>\n<Channels/>\n<Groups/>\n<Contacts/>\n<UserId>0687C32C-0331-E611-80C3-ECB1D78A5C75</UserId>\n<UserCenter>5</UserCenter>\n<UserName>:nomelx.4678</UserName>\n<ChangeId>5</ChangeId>\n<NewBeginning/>\n</Message>\n";

    memset(m_TLSSendBuffer, 0, 4096);
    sprintf(m_TLSSendBuffer, replyMessageTemp,
            strlen(replyMessageTemp));
    m_TLSSendBufferLength = strlen(m_TLSSendBuffer);
    m_TLSSendNeeded = true;
}

void LoginSession::Logout(XMLPacket *Packet)
{

}
