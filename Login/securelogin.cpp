#include "securelogin.h"

SecureLogin::SecureLogin(ClientConnection *Client) : m_Client(Client), m_State(LS_NONE), m_SSL()
{

}

//bool SecureLogin::Recieve(char *RecvBuffer, int Length, ServerSSL *SSL)
bool SecureLogin::Recieve(ServerSSL *SSL)
{
    // Client has not initiated a secure connection yet.
    if (m_State == LS_NONE) {
        m_SSL = SSL_new(SSL->GetContext());
        printf("SSL Created using %x - %x\n", (char)SSL->GetContext()->method->version, *(((char*)&SSL->GetContext()->method->version)+1));
        m_State = LS_WAIT_HANDSHAKE;
        return true;
    } else if (m_State == LS_WAIT_HANDSHAKE) {
        return EstablishTLS();
    } else if (m_State == LS_TLS_HANDSHAKE_SENT) {

    }

    return false;
}

void SecureLogin::Send()
{
}

bool SecureLogin::EstablishTLS()
{
    SSL_set_fd(m_SSL, m_Client->m_ClientSocket);
    SSL_set_accept_state(m_SSL);
    SSL_set_session_id_context(m_SSL, 0, 0);

    int acceptRequest = SSL_accept(m_SSL);

    if ((acceptRequest) == -1) {
        int code = SSL_get_error(m_SSL, acceptRequest);
        if (code != 2) {
            printf("%s -> SSL Handshake Error - %d\n", m_Client->m_ClientIP, code);
            ERR_print_errors_fp(stderr);
            SSL_shutdown(m_SSL);
            SSL_free(m_SSL);
            m_State = LS_NONE;
        }
        return false;
    } else {
        m_State = LS_TLS_HANDSHAKE_SENT;
        printf("%s -> Handshake Sent\n", m_Client->m_ClientIP);
    }

    m_State = LS_TLS_HANDSHAKE_SENT;

    return true;
}
