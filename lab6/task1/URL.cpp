#include <iostream>
#include <string>
#include "CHttpUrl.h"
#include "CUrlParsingError.h"

namespace
{
    std::string ProtocolToString(const Protocol protocol)
    {
        return (protocol == Protocol::HTTP) ? "http" : "https";
    }

    void PrintUrlInfo(const CHttpUrl& url)
    {
        std::cout << "URL:      " << url.GetURL() << "\n"
            << "Protocol: " << ProtocolToString(url.GetProtocol()) << "\n"
            << "Domain:   " << url.GetDomain() << "\n"
            << "Port:     " << url.GetPort() << "\n"
            << "Document: " << url.GetDocument() << "\n";
    }

    void PrintHelp()
    {
        std::cout << "<protocol>://<domain>[:<port>][<document>]\n";
    }

    void ProcessLine(const std::string& line)
    {
        if (line == "help")
        {
            PrintHelp();
            return;
        }

        try
        {
            const CHttpUrl url(line);
            PrintUrlInfo(url);
        }
        catch (const CUrlParsingError& e)
        {
            std::cout << "Error parsing URL: " << e.what() << "\n";
        }
    }
}

int main()
{
    PrintHelp();

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (!line.empty())
        {
            ProcessLine(line);
            std::cout << "\n";
        }
    }

    return 0;
}
