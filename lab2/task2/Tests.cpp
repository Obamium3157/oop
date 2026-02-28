#include <catch2/catch_test_macros.hpp>
#include <string>

#include "Html.h"


TEST_CASE("HtmlDecode: &quot; decodes to double quote")
{
    REQUIRE(HtmlDecode("&quot;") == "\"");
}

TEST_CASE("HtmlDecode: &apos; decodes to apostrophe")
{
    REQUIRE(HtmlDecode("&apos;") == "'");
}

TEST_CASE("HtmlDecode: &lt; decodes to less-than")
{
    REQUIRE(HtmlDecode("&lt;") == "<");
}

TEST_CASE("HtmlDecode: &gt; decodes to greater-than")
{
    REQUIRE(HtmlDecode("&gt;") == ">");
}

TEST_CASE("HtmlDecode: &amp; decodes to ampersand")
{
    REQUIRE(HtmlDecode("&amp;") == "&");
}

TEST_CASE("HtmlDecode: full example from task")
{
    REQUIRE(
        HtmlDecode("Cat &lt;says&gt; &quot;Meow&quot;. M&amp;M&apos;s")
        == "Cat <says> \"Meow\". M&M's"
    );
}

TEST_CASE("HtmlDecode: empty string")
{
    REQUIRE(HtmlDecode("").empty());
}

TEST_CASE("HtmlDecode: string without entities")
{
    REQUIRE(HtmlDecode("Hello, World!") == "Hello, World!");
}

TEST_CASE("HtmlDecode: lone ampersand is not decoded")
{
    REQUIRE(HtmlDecode("a & b") == "a & b");
}

TEST_CASE("HtmlDecode: incomplete entity is left untouched")
{
    REQUIRE(HtmlDecode("&lt") == "&lt");
}

TEST_CASE("HtmlDecode: unknown entity is left untouched")
{
    REQUIRE(HtmlDecode("&nbsp;") == "&nbsp;");
}

TEST_CASE("HtmlDecode: entities side by side")
{
    REQUIRE(HtmlDecode("&lt;&gt;") == "<>");
}

TEST_CASE("HtmlDecode: entity at start of string")
{
    REQUIRE(HtmlDecode("&amp;text") == "&text");
}

TEST_CASE("HtmlDecode: entity at end of string")
{
    REQUIRE(HtmlDecode("text&amp;") == "text&");
}

TEST_CASE("HtmlDecode: repeated same entity")
{
    REQUIRE(HtmlDecode("&amp;&amp;&amp;") == "&&&");
}

TEST_CASE("HtmlDecode: decoded ampersand is not re-encoded")
{
    REQUIRE(HtmlDecode("&amp;lt;") == "&lt;");
}

TEST_CASE("HtmlDecode: only semicolon")
{
    REQUIRE(HtmlDecode(";") == ";");
}

TEST_CASE("HtmlDecode: ampersand followed immediately by semicolon")
{
    REQUIRE(HtmlDecode("&;") == "&;");
}

TEST_CASE("HtmlDecode: multiline-like input with all entities")
{
    REQUIRE(HtmlDecode("&lt;p&gt;Hello &amp; &quot;World&quot;&lt;/p&gt;")
        == "<p>Hello & \"World\"</p>");
}