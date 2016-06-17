#include "securelogin.h"

#include "Util/system.h"

SecureLogin::SecureLogin(ClientConnection *Client) : m_Client(Client), m_State(LS_NONE), m_SSL()
{

}

RecieveType SecureLogin::Recieve(ServerSSL *SSL)
{
    // Client has not initiated a secure connection yet.
    if (m_State == LS_NONE) {
        m_SSL = SSL_new(SSL->GetContext());
        m_State = LS_WAIT_HANDSHAKE;
        printf("SSL Created using: %s\n", SSL_get_version(m_SSL));
        return RT_TLS_HANDSHAKE;
    } else if (m_State == LS_WAIT_HANDSHAKE || m_State == LS_TLS_WANT_READ_WRITE) {
        if (EstablishTLS()) {
            return RT_TLS_HANDSHAKE;
        } else {
            return RT_TLS_FAIL;
        }
    } else if (m_State == LS_TLS_HANDSHAKE_SENT) {
        if (ClientPacket()) {
            return RT_TLS_LOGIN_PACKET;
        } else {
            return RT_TLS_WAIT;
        }
    }

    return RT_TLS_WAIT;
}

void SecureLogin::Send(const char* Data, size_t DataSize)
{
    auto sendError = SSL_write(m_SSL, Data, DataSize);
    if (sendError == -1) {
        printf("There was an issue sending TLS data to the client, requested send size was %d\n", DataSize);
    } else {
        System::DebugWriteMessage("SSL_Trace.dat", "--Send--\n\n", strlen("--Send--\n\n"));
        System::DebugWriteMessage("SSL_Trace.dat", Data, DataSize);
        System::DebugWriteMessage("SSL_Trace.dat", "\n\n--Send END--\n\n", strlen("\n\n--Send END--\n\n"));
    }
}

LoginState SecureLogin::GetState() const
{
    return m_State;
}

const char *SecureLogin::GetBuffer() const
{
    return m_DecodeBuffer;
}

bool SecureLogin::EstablishTLS()
{
    int acceptRequest = -1;

    if (m_State == LS_WAIT_HANDSHAKE) {
        SSL_set_fd(m_SSL, m_Client->m_ClientSocket);
        SSL_set_accept_state(m_SSL);
        SSL_set_session_id_context(m_SSL, 0, 0);
        acceptRequest = SSL_accept(m_SSL);
    }

    if (m_State == LS_TLS_WANT_READ_WRITE) {
        acceptRequest = SSL_accept(m_SSL);
    }

    if ((acceptRequest) == -1) {
        int code = SSL_get_error(m_SSL, acceptRequest);
        if (!(code == SSL_ERROR_WANT_READ || code == SSL_ERROR_WANT_WRITE)) {
            printf("%s -> SSL Handshake Error - %d\n", m_Client->m_ClientIP, code);
            ERR_print_errors_fp(stderr);
            SSL_shutdown(m_SSL);
            SSL_free(m_SSL);
            m_State = LS_TLS_FAIL_HANDSHAKE;
            return false;
        } else {
            m_State = LS_TLS_WANT_READ_WRITE;
        }
    } else {
        m_State = LS_TLS_HANDSHAKE_SENT;
        printf("%s -> Handshake Sent\n", m_Client->m_ClientIP);
    }

    return true;
}

bool SecureLogin::ClientPacket() {

    memset(m_DecodeBuffer, 0, 4096);

    auto sslReadError = SSL_read(m_SSL, m_DecodeBuffer, 4095);

    if (sslReadError != -1) {
        if (sslReadError > 1) {
            System::DebugWriteMessage("SSL_Trace.dat", "--Recv--\n\n", strlen("--Recv--\n\n"));
            System::DebugWriteMessage("SSL_Trace.dat", m_DecodeBuffer, sslReadError);
            System::DebugWriteMessage("SSL_Trace.dat", "\n\n--Recv END--\n\n", strlen("\n\n--Recv END--\n\n"));
        }
        return true;
    }

    auto sslErrorCode = SSL_get_error(m_SSL, sslReadError);

    return false;
}
