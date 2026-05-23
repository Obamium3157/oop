#include "CHttpUrl.h"

#include <algorithm>

#include "CUrlParsingError.h"

namespace
{
    constexpr unsigned short HttpDefaultPort = 80;
    constexpr unsigned short HttpsDefaultPort = 443;
    constexpr unsigned short MinPort = 1;
    constexpr unsigned short MaxPort = 65535;

    void ValidateDomain(std::string const& domain)
    {
        if (domain.empty())
        {
            throw std::invalid_argument("Domain cannot be empty");
        }

        for (const char c : domain)
        {
            if (std::isspace(static_cast<unsigned char>(c)))
            {
                throw std::invalid_argument("Domain cannot contain whitespace characters");
            }
        }
    }

    void ValidatePort(const unsigned long port)
    {
        if (port < MinPort || port > MaxPort)
        {
            throw std::invalid_argument("Port must be in range [1, 65535], got: " + std::to_string(port));
        }
    }

    Protocol ParseProtocol(std::string const& protocolStr)
    {
        std::string lower = protocolStr;
        std::ranges::transform(lower, lower.begin(),
                               [](const unsigned char c) { return std::tolower(c); });
        if (lower == "http")
        {
            return Protocol::HTTP;
        }
        if (lower == "https")
        {
            return Protocol::HTTPS;
        }
        throw CUrlParsingError("Unknown protocol: '" + protocolStr + "'");
    }

    unsigned short ParsePort(std::string const& portStr)
    {
        if (portStr.empty())
        {
            throw CUrlParsingError("Port cannot be empty");
        }

        for (const char c : portStr)
        {
            if (!std::isdigit(static_cast<unsigned char>(c)))
            {
                throw CUrlParsingError("Port contains invalid character: '" + std::string(1, c) + "'");
            }
        }

        unsigned long port = 0;
        try
        {
            port = std::stoul(portStr);
        }
        catch (std::out_of_range const&)
        {
            throw CUrlParsingError("Port value is out of range: " + portStr);
        }

        if (port < MinPort || port > MaxPort)
        {
            throw CUrlParsingError("Port must be in range [1, 65535], got: " + portStr);
        }

        return static_cast<unsigned short>(port);
    }

    std::string NormalizeDocument(std::string const& document)
    {
        if (document.empty() || document[0] != '/')
        {
            return '/' + document;
        }

        return document;
    }

    unsigned short GetDefaultPort(const Protocol protocol)
    {
        return (protocol == Protocol::HTTP) ? HttpDefaultPort : HttpsDefaultPort;
    }

    bool IsDefaultPort(const Protocol protocol, const unsigned short port)
    {
        return port == GetDefaultPort(protocol);
    }

    size_t FindSchemeEnd(std::string const& url)
    {
        const size_t schemeEnd = url.find("://");
        if (schemeEnd == std::string::npos)
        {
            throw CUrlParsingError("Missing '://' separator in URL");
        }
        return schemeEnd;
    }

    size_t FindPathStart(std::string const& url, const size_t authorityStart)
    {
        if (authorityStart >= url.size())
        {
            throw CUrlParsingError("Missing host in URL");
        }
        return url.find('/', authorityStart);
    }

    std::string ExtractAuthority(std::string const& url, const size_t authorityStart, const size_t pathStart)
    {
        return (pathStart == std::string::npos)
                   ? url.substr(authorityStart)
                   : url.substr(authorityStart, pathStart - authorityStart);
    }

    std::string ParseDomain(std::string const& authority)
    {
        const size_t colonPos = authority.find(':');
        const std::string domain = (colonPos == std::string::npos)
                                       ? authority
                                       : authority.substr(0, colonPos);
        try
        {
            ValidateDomain(domain);
        }
        catch (std::invalid_argument const& e)
        {
            throw CUrlParsingError(e.what());
        }
        return domain;
    }

    unsigned short ParsePortFromAuthority(std::string const& authority, const Protocol protocol)
    {
        const size_t colonPos = authority.find(':');
        if (colonPos == std::string::npos)
        {
            return GetDefaultPort(protocol);
        }
        return ParsePort(authority.substr(colonPos + 1));
    }
}

CHttpUrl::CHttpUrl(std::string const& url)
{
    const std::string schemeSeparator = "://";
    const size_t schemeEnd = FindSchemeEnd(url);
    m_protocol = ParseProtocol(url.substr(0, schemeEnd));

    const size_t authorityStart = schemeEnd + schemeSeparator.size();
    const size_t pathStart = FindPathStart(url, authorityStart);
    const std::string authority = ExtractAuthority(url, authorityStart, pathStart);

    m_domain = ParseDomain(authority);
    m_port = ParsePortFromAuthority(authority, m_protocol);
    m_document = (pathStart == std::string::npos) ? "/" : url.substr(pathStart);
}

CHttpUrl::CHttpUrl(
    std::string const& domain,
    std::string const& document,
    const Protocol protocol)
    : CHttpUrl(domain, document, protocol, GetDefaultPort(protocol))
{
}

CHttpUrl::CHttpUrl(
    std::string const& domain,
    std::string const& document,
    const Protocol protocol,
    const unsigned short port)
{
    ValidateDomain(domain);
    ValidatePort(port);
    m_domain = domain;
    m_protocol = protocol;
    m_port = port;
    m_document = NormalizeDocument(document);
}

std::string CHttpUrl::GetURL() const
{
    std::string url = (m_protocol == Protocol::HTTP) ? "http" : "https";
    url += "://" + m_domain;
    if (!IsDefaultPort(m_protocol, m_port))
    {
        url += ':' + std::to_string(m_port);
    }
    url += m_document;
    return url;
}

std::string CHttpUrl::GetDomain() const
{
    return m_domain;
}

std::string CHttpUrl::GetDocument() const
{
    return m_document;
}

Protocol CHttpUrl::GetProtocol() const
{
    return m_protocol;
}

unsigned short CHttpUrl::GetPort() const
{
    return m_port;
}
