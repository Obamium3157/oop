#include "html.h"

#include <unordered_map>

namespace
{
    const std::unordered_map<std::string, char> htmlEntities = {
        {"&quot;", '"'},
        {"&apos;", '\''},
        {"&lt;",   '<'},
        {"&gt;",   '>'},
        {"&amp;",  '&'},
    };

    constexpr char entityStart = '&';
    constexpr char entityEnd = ';';
    constexpr std::size_t maxEntityLength = 6;
}

std::string HtmlDecode(const std::string & html)
{
    std::string result;
    result.reserve(html.size());

    std::size_t i = 0;

    while (i < html.size())
    {
        if (html[i] != entityStart)
        {
            result += html[i];
            ++i;
            continue;
        }

        const std::size_t semicolonPos = html.find(entityEnd, i + 1);
        const bool isTooFar = semicolonPos == std::string::npos
            || semicolonPos - i + 1 > maxEntityLength;

        if (isTooFar)
        {
            result += html[i];
            ++i;
            continue;
        }

        const std::string candidate = html.substr(i, semicolonPos - i + 1);
        const auto it = htmlEntities.find(candidate);

        if (it == htmlEntities.end())
        {
            result += html[i];
            ++i;
            continue;
        }

        result += it->second;
        i += candidate.size();
    }

    return result;
}