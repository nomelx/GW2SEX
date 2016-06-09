#include "serverssl.h"

ServerSSL::ServerSSL() : m_Method(), m_CTX()
{

}

bool ServerSSL::Init(const char* ServerCertFile, const char* ServerPrivateKeyFile)
{
    if (!ServerCertFile || !ServerPrivateKeyFile) {
        return false;
    }

    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();

    m_Method = SSLv23_server_method();
    m_CTX = SSL_CTX_new(m_Method);

    SSL_CTX_set_session_cache_mode(m_CTX, SSL_SESS_CACHE_OFF);

    if (!m_CTX) {
        printf("[----- SSL_ERROR -----\n");
        ERR_print_errors_fp(stderr);
        printf("[----- END SSL_ERROR -----\n");
        return false;
    }

    if (SSL_CTX_use_certificate_file(m_CTX, ServerCertFile, SSL_FILETYPE_PEM) <= 0 ||
            SSL_CTX_use_PrivateKey_file(m_CTX, ServerPrivateKeyFile, SSL_FILETYPE_PEM) <= 0) {
        printf("[----- SSL_ERROR -----\n");
        ERR_print_errors_fp(stderr);
        printf("[----- END SSL_ERROR -----\n");
        return false;
    }

    if (!SSL_CTX_check_private_key(m_CTX)) {
        fprintf(stderr,"Private key does not match the certificate public key\n");
        return false;
    }

    return true;
}

SSL_CTX *ServerSSL::GetContext()
{
    return m_CTX;
}

