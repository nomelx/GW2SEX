#include "loginclient.h"

LoginClient::LoginClient(ClientConnection &Base) : ClientConnection(), m_Packet(), m_Login(this), m_Session(this)
{
    this->m_ClientSocket = Base.m_ClientSocket;
    this->m_ClientPriority = Base.m_ClientPriority;
    memcpy(this->m_ClientIP, Base.m_ClientIP, INET_ADDRSTRLEN);
}

LoginClient::~LoginClient()
{

}

void LoginClient::Tick(ServerSSL *SSL)
{

    // If there were packets
    //if (readLength > 0) {

        // Check if there was an XML request.
        if (!m_Session.m_TSLReady) {

            // Read any incomming packets from client
            char incommingBuffer[2048];
            memset(incommingBuffer, 0x69, 2048);
            int readLength = this->Poll(incommingBuffer, 2048);

            if (readLength > 0 && m_Packet.Parse(incommingBuffer, readLength) && m_Packet.Validate()) {
                if (m_Session.Recieve(&m_Packet)) {
                    m_Session.Send();
                }
                m_Packet.Clear();
            }
        } else {
            // Check if there was a binary packet.
            //if (m_Login.Recieve(incommingBuffer, readLength, SSL)) {
            if (m_Login.Recieve(SSL) == RT_TLS_LOGIN_PACKET) {
                const char* clientLoginBuffer = m_Login.GetBuffer();
                if (clientLoginBuffer[0] != 0) {
                    if (m_Packet.Parse(clientLoginBuffer, 4096) && m_Packet.Validate()) {
                        if (m_Session.Recieve(&m_Packet)) {
                            m_Session.Send();
                        }
                        m_Packet.Clear();
                    }
                }
                //m_Login.Send();
            }
        }
        //}
}

void LoginClient::Close()
{
    //m_Session.Clear();
    ClientConnection::Close();
}

bool LoginClient::IsConnected()
{
    if (!ClientConnection::IsConnected() || m_Login.GetState() == LS_TLS_FAIL_HANDSHAKE) {
        return false;
    }
    return true;
}
