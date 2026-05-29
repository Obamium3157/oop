#include <catch2/catch_test_macros.hpp>

#include "../CHttpUrl.h"
#include "../CUrlParsingError.h"

TEST_CASE("String constructor (protocol parsing)", "[constructor]")
{
    SECTION("Lowercase http is accepted")
    {
        CHttpUrl url("http://example.com");
        REQUIRE(url.GetProtocol() == Protocol::HTTP);
    }

    SECTION("Lowercase https is accepted")
    {
        CHttpUrl url("https://example.com");
        REQUIRE(url.GetProtocol() == Protocol::HTTPS);
    }

    SECTION("Uppercase HTTP is accepted")
    {
        CHttpUrl url("HTTP://example.com");
        REQUIRE(url.GetProtocol() == Protocol::HTTP);
    }

    SECTION("Mixed case HtTpS is accepted")
    {
        CHttpUrl url("HtTpS://example.com");
        REQUIRE(url.GetProtocol() == Protocol::HTTPS);
    }

    SECTION("Missing :// throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http:example.com"), CUrlParsingError);
    }

    SECTION("Unknown protocol throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("ftp://example.com"), CUrlParsingError);
    }

    SECTION("Empty string throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl(""), CUrlParsingError);
    }
}

TEST_CASE("String constructor (domain parsing)", "[constructor]")
{
    SECTION("Simple domain is parsed correctly")
    {
        CHttpUrl url("http://example.com");
        REQUIRE(url.GetDomain() == "example.com");
    }

    SECTION("Subdomain is parsed correctly")
    {
        CHttpUrl url("http://sub.example.com");
        REQUIRE(url.GetDomain() == "sub.example.com");
    }

    SECTION("Missing host after :// throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://"), CUrlParsingError);
    }

    SECTION("Domain with space throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://exa mple.com"), CUrlParsingError);
    }
}

TEST_CASE("String constructor (port parsing)", "[constructor]")
{
    SECTION("Http without explicit port uses 80")
    {
        CHttpUrl url("http://example.com");
        REQUIRE(url.GetPort() == 80);
    }

    SECTION("Https without explicit port uses 443")
    {
        CHttpUrl url("https://example.com");
        REQUIRE(url.GetPort() == 443);
    }

    SECTION("Minimum valid port 1 is accepted")
    {
        CHttpUrl url("http://example.com:1/");
        REQUIRE(url.GetPort() == 1);
    }

    SECTION("Maximum valid port 65535 is accepted")
    {
        CHttpUrl url("http://example.com:65535/");
        REQUIRE(url.GetPort() == 65535);
    }

    SECTION("Port 0 throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://example.com:0/"), CUrlParsingError);
    }

    SECTION("Port 65536 throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://example.com:65536/"), CUrlParsingError);
    }

    SECTION("Port with letters throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://example.com:abc/"), CUrlParsingError);
    }

    SECTION("Empty port throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://example.com:/"), CUrlParsingError);
    }

    SECTION("Overflowing port number throws CUrlParsingError")
    {
        REQUIRE_THROWS_AS(CHttpUrl("http://example.com:999999999999/"), CUrlParsingError);
    }
}

TEST_CASE("String constructor (document parsing)", "[constructor]")
{
    SECTION("Missing document defaults to /")
    {
        CHttpUrl url("http://example.com");
        REQUIRE(url.GetDocument() == "/");
    }

    SECTION("Root path / is preserved")
    {
        CHttpUrl url("http://example.com/");
        REQUIRE(url.GetDocument() == "/");
    }

    SECTION("Nested path is parsed correctly")
    {
        CHttpUrl url("http://example.com/images/photo.jpg");
        REQUIRE(url.GetDocument() == "/images/photo.jpg");
    }

    SECTION("Query string is included in document")
    {
        CHttpUrl url("http://example.com/search?q=test");
        REQUIRE(url.GetDocument() == "/search?q=test");
    }

    SECTION("Domain is not contaminated by query string characters")
    {
        CHttpUrl url("http://example.com?q=test");
        REQUIRE(url.GetDomain() == "example.com");
    }

    SECTION("Document starting with ? does not include /")
    {
        CHttpUrl url("example.com", "?q=test", Protocol::HTTP);
        REQUIRE(url.GetDocument() == "?q=test");
    }
}

TEST_CASE("Parametric constructor (domain validation)", "[constructor]")
{
    SECTION("Empty domain throws invalid_argument")
    {
        REQUIRE_THROWS_AS(CHttpUrl("", "/", Protocol::HTTP), std::invalid_argument);
    }

    SECTION("Domain with space throws invalid_argument")
    {
        REQUIRE_THROWS_AS(CHttpUrl("exa mple.com", "/", Protocol::HTTP), std::invalid_argument);
    }

    SECTION("Valid domain is accepted")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP);
        REQUIRE(url.GetDomain() == "example.com");
    }
}

TEST_CASE("Parametric constructor (document normalization)", "[constructor]")
{
    SECTION("Document starting with / is unchanged")
    {
        CHttpUrl url("example.com", "/index.html", Protocol::HTTP);
        REQUIRE(url.GetDocument() == "/index.html");
    }

    SECTION("Document without leading / gets / prepended")
    {
        CHttpUrl url("example.com", "index.html", Protocol::HTTP);
        REQUIRE(url.GetDocument() == "/index.html");
    }

    SECTION("Empty document becomes /")
    {
        CHttpUrl url("example.com", "", Protocol::HTTP);
        REQUIRE(url.GetDocument() == "/");
    }
}

TEST_CASE("Parametric constructor (port validation)", "[constructor]")
{
    SECTION("Without port http uses 80")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP);
        REQUIRE(url.GetPort() == 80);
    }

    SECTION("Without port https uses 443")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTPS);
        REQUIRE(url.GetPort() == 443);
    }

    SECTION("Port 0 throws invalid_argument")
    {
        REQUIRE_THROWS_AS(CHttpUrl("example.com", "/", Protocol::HTTP, 0), std::invalid_argument);
    }

    SECTION("Port 1 is accepted")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP, 1);
        REQUIRE(url.GetPort() == 1);
    }

    SECTION("Port 65535 is accepted")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP, 65535);
        REQUIRE(url.GetPort() == 65535);
    }
}

TEST_CASE("Port inclusion rules", "[GetURL]")
{
    SECTION("Http with default port 80 omits port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP, 80);
        REQUIRE(url.GetURL() == "http://example.com/");
    }

    SECTION("Https with default port 443 omits port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTPS, 443);
        REQUIRE(url.GetURL() == "https://example.com/");
    }

    SECTION("Http with non-default port 8080 includes port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP, 8080);
        REQUIRE(url.GetURL() == "http://example.com:8080/");
    }

    SECTION("Https with non-default port 8443 includes port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTPS, 8443);
        REQUIRE(url.GetURL() == "https://example.com:8443/");
    }

    SECTION("Http with port 443 includes port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTP, 443);
        REQUIRE(url.GetURL() == "http://example.com:443/");
    }

    SECTION("Https with port 80 includes port")
    {
        CHttpUrl url("example.com", "/", Protocol::HTTPS, 80);
        REQUIRE(url.GetURL() == "https://example.com:80/");
    }
}

TEST_CASE("Getters return correct values after string constructor")
{
    CHttpUrl url("https://sub.example.com:8443/path/to/doc.html");

    REQUIRE(url.GetProtocol() == Protocol::HTTPS);
    REQUIRE(url.GetDomain() == "sub.example.com");
    REQUIRE(url.GetPort() == 8443);
    REQUIRE(url.GetDocument() == "/path/to/doc.html");
    REQUIRE(url.GetURL() == "https://sub.example.com:8443/path/to/doc.html");
}

TEST_CASE("Getters return correct values for URL with nested URL in query string")
{
    CHttpUrl url("https://example.com?url=localhost.ru:1111/doc.html");

    REQUIRE(url.GetProtocol() == Protocol::HTTPS);
    REQUIRE(url.GetDomain() == "example.com");
    REQUIRE(url.GetPort() == 443);
    REQUIRE(url.GetDocument() == "?url=localhost.ru:1111/doc.html");
    REQUIRE(url.GetURL() == "https://example.com?url=localhost.ru:1111/doc.html");
}