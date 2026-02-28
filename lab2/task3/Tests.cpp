#include <catch2/catch_test_macros.hpp>
#include <map>
#include <sstream>
#include <string>

#include "WordFrequency.h"

TEST_CASE("ToLower: lowercase is unchanged")
{
    REQUIRE(ToLower("hello") == "hello");
}

TEST_CASE("ToLower: uppercase is lowercased")
{
    REQUIRE(ToLower("HELLO") == "hello");
}

TEST_CASE("ToLower: mixed case is lowercased")
{
    REQUIRE(ToLower("HeLLo") == "hello");
}

TEST_CASE("ToLower: empty string is unchanged")
{
    REQUIRE(ToLower("").empty());
}

TEST_CASE("CountWordFrequencies: empty input produces empty map")
{
    std::istringstream input("");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.empty());
}

TEST_CASE("CountWordFrequencies: single word counted once")
{
    std::istringstream input("hello");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("hello") == 1);
}

TEST_CASE("CountWordFrequencies: same word counted multiple times")
{
    std::istringstream input("hello hello hello");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("hello") == 3);
}

TEST_CASE("CountWordFrequencies: different words counted separately")
{
    std::istringstream input("cat dog cat");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("cat") == 2);
    REQUIRE(frequencies.at("dog") == 1);
}

TEST_CASE("CountWordFrequencies: case-insensitive counting")
{
    std::istringstream input("Hello hElLO HELLO");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.size() == 1);
    REQUIRE(frequencies.at("hello") == 3);
}

TEST_CASE("CountWordFrequencies: words separated by tabs and newlines")
{
    std::istringstream input("cat\tdog\ncat\ndog\ndog");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("cat") == 2);
    REQUIRE(frequencies.at("dog") == 3);
}

TEST_CASE("CountWordFrequencies: multiple spaces between words")
{
    std::istringstream input("cat   cat    dog");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("cat") == 2);
    REQUIRE(frequencies.at("dog") == 1);
}

TEST_CASE("CountWordFrequencies: result is sorted alphabetically")
{
    std::istringstream input("zebra apple mango apple");
    const auto frequencies = CountWordFrequencies(input);
    const std::vector<std::string> keys = [&]()
    {
        std::vector<std::string> result;
        for (const auto& [word, _] : frequencies)
        {
            result.push_back(word);
        }
        return result;
    }();

    REQUIRE(keys == std::vector<std::string>{"apple", "mango", "zebra"});
}

TEST_CASE("CountWordFrequencies: punctuation is part of word")
{
    std::istringstream input("hello, hello");
    const auto frequencies = CountWordFrequencies(input);
    REQUIRE(frequencies.at("hello,") == 1);
    REQUIRE(frequencies.at("hello") == 1);
}

TEST_CASE("PrintWordFrequencies: empty map produces no output")
{
    std::ostringstream output;
    PrintWordFrequencies({}, output);
    REQUIRE(output.str().empty());
}

TEST_CASE("PrintWordFrequencies: single entry formatted correctly")
{
    std::ostringstream output;
    PrintWordFrequencies({{"hello", 3}}, output);
    REQUIRE(output.str() == "hello 3\n");
}

TEST_CASE("PrintWordFrequencies: multiple entries sorted alphabetically")
{
    std::ostringstream output;
    const std::map<std::string, int> frequencies = {
        {"apple", 2},
        {"mango", 1},
        {"zebra", 5},
    };
    PrintWordFrequencies(frequencies, output);
    REQUIRE(output.str() == "apple 2\nmango 1\nzebra 5\n");
}

TEST_CASE("Integration: full pipeline")
{
    std::istringstream input("The the THE cat sat on the mat");
    std::ostringstream output;

    const auto frequencies = CountWordFrequencies(input);
    PrintWordFrequencies(frequencies, output);

    REQUIRE(output.str() ==
        "cat 1\n"
        "mat 1\n"
        "on 1\n"
        "sat 1\n"
        "the 4\n"
    );
}