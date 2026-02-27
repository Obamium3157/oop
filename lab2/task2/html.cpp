#include "html.h"

#include <unordered_map>

namespace
{
    const std::unordered_map<std::string, char> HTML_ENTITIES = {
        {"&quot;", '"'},
        {"&apos;", '\''},
        {"&lt;",   '<'},
        {"&gt;",   '>'},
        {"&amp;",  '&'},
    };

    constexpr char ENTITY_START = '&';
    constexpr char ENTITY_END = ';';
    constexpr std::size_t MAX_ENTITY_LENGTH = 6;
}

std::string HtmlDecode(const std::string & html)
{
    std::string result;
    result.reserve(html.size());

    std::size_t i = 0;

    while (i < html.size())
    {
        if (html[i] != ENTITY_START)
        {
            result += html[i];
            ++i;
            continue;
        }

        const std::size_t semicolonPos = html.find(ENTITY_END, i + 1);
        const bool isTooFar = semicolonPos == std::string::npos
            || semicolonPos - i + 1 > MAX_ENTITY_LENGTH;

        if (isTooFar)
        {
            result += html[i];
            ++i;
            continue;
        }

        const std::string candidate = html.substr(i, semicolonPos - i + 1);
        const auto it = HTML_ENTITIES.find(candidate);

        if (it == HTML_ENTITIES.end())
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