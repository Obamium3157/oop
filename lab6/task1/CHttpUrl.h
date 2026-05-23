#ifndef OOP_CHTTPURL_H
#define OOP_CHTTPURL_H


#include <stdexcept>
#include <string>

enum class Protocol
{
    HTTP,
    HTTPS
};

class CHttpUrl
{
public:
    explicit CHttpUrl(std::string const& url);

    CHttpUrl(
        std::string const& domain,
        std::string const& document,
        Protocol protocol = Protocol::HTTP);

    CHttpUrl(
        std::string const& domain,
        std::string const& document,
        Protocol protocol,
        unsigned short port);

    std::string GetURL() const;
    std::string GetDomain() const;
    std::string GetDocument() const;
    Protocol GetProtocol() const;
    unsigned short GetPort() const;

private:
    Protocol m_protocol;
    std::string m_domain;
    std::string m_document;
    unsigned short m_port;
};

#endif //OOP_CHTTPURL_H
