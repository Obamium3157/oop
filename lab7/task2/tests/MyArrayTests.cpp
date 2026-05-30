#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "../CMyArray.h"

struct ThrowingCopy
{
    static inline int s_copyCount = 0;
    static inline int s_throwAfter = 0;

    int m_value = 0;

    explicit ThrowingCopy(const int value = 0)
        : m_value(value)
    {
    }

    ThrowingCopy(const ThrowingCopy& other)
    {
        ++s_copyCount;
        if (s_throwAfter > 0 && s_copyCount >= s_throwAfter)
        {
            throw std::runtime_error("Simulated copy failure");
        }
        m_value = other.m_value;
    }

    ThrowingCopy& operator=(const ThrowingCopy& other)
    {
        ++s_copyCount;
        if (s_throwAfter > 0 && s_copyCount >= s_throwAfter)
        {
            throw std::runtime_error("Simulated assignment failure");
        }
        m_value = other.m_value;
        return *this;
    }

    static void SetThrowAfter(const int n) noexcept
    {
        s_copyCount = 0;
        s_throwAfter = n;
    }

    static void Reset() noexcept
    {
        s_copyCount = 0;
        s_throwAfter = 0;
    }
};

struct ThrowingCopyGuard
{
    ~ThrowingCopyGuard() noexcept
    {
        ThrowingCopy::Reset();
    }
};


TEST_CASE("Default constructor", "[constructor]")
{
    CMyArray<int> arr;

    REQUIRE(arr.GetSize() == 0);
    REQUIRE(arr.begin() == arr.end());
    REQUIRE(arr.rbegin() == arr.rend());
    REQUIRE(arr.cbegin() == arr.cend());
    REQUIRE(arr.crbegin() == arr.crend());
    REQUIRE_THROWS_AS(arr[0], std::out_of_range);
    REQUIRE_NOTHROW(arr.Resize(0));
    REQUIRE_NOTHROW(arr.Clear());
    REQUIRE(arr.GetSize() == 0);
}

TEST_CASE("PushBack adds elements in insertion order", "[pushback]")
{
    CMyArray<int> arr;

    SECTION("First push")
    {
        arr.PushBack(42);

        REQUIRE(arr.GetSize() == 1);
        REQUIRE(arr[0] == 42);
    }

    SECTION("Elements survive all capacity-doubling reallocations")
    {
        for (int i = 1; i <= 9; ++i)
        {
            arr.PushBack(i);
        }

        REQUIRE(arr.GetSize() == 9);
        for (int i = 1; i <= 9; ++i)
        {
            REQUIRE(arr[i - 1] == i);
        }
    }

    SECTION("PushBack with rvalue moves the element into the array")
    {
        CMyArray<std::string> strArr;
        std::string s = "hello";
        strArr.PushBack(std::move(s));

        REQUIRE(strArr.GetSize() == 1);
        REQUIRE(strArr[0] == "hello");
    }
}


TEST_CASE("operator[] provides correct access and throws on out-of-range", "[indexing]")
{
    CMyArray<int> arr;
    arr.PushBack(10);
    arr.PushBack(20);
    arr.PushBack(30);

    SECTION("First and last valid indices are accessible")
    {
        REQUIRE(arr[0] == 10);
        REQUIRE(arr[2] == 30);
    }

    SECTION("Index equal to size throws out_of_range")
    {
        REQUIRE_THROWS_AS(arr[3], std::out_of_range);
    }

    SECTION("Arbitrary large index throws out_of_range")
    {
        REQUIRE_THROWS_AS(arr[100], std::out_of_range);
    }

    SECTION("Maximum size_t index throws out_of_range")
    {
        REQUIRE_THROWS_AS(arr[std::numeric_limits<size_t>::max()], std::out_of_range);
    }

    SECTION("Assignment through operator[] is reflected in subsequent read")
    {
        arr[1] = 99;
        REQUIRE(arr[1] == 99);
    }

    SECTION("Const operator[] works through const reference")
    {
        const CMyArray<int>& constRef = arr;

        REQUIRE(constRef[0] == 10);
        REQUIRE(constRef[2] == 30);
        REQUIRE_THROWS_AS(constRef[3], std::out_of_range);
    }
}

TEST_CASE("Resize adjusts size in both directions", "[resize]")
{
    SECTION("Resize to same size does nothing")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.PushBack(2);
        arr.PushBack(3);

        arr.Resize(3);

        REQUIRE(arr.GetSize() == 3);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[2] == 3);
    }

    SECTION("Resize down removes trailing elements and invalidates their indices")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 5; ++i)
        {
            arr.PushBack(i);
        }

        arr.Resize(2);

        REQUIRE(arr.GetSize() == 2);
        REQUIRE(arr[0] == 0);
        REQUIRE(arr[1] == 1);
        REQUIRE_THROWS_AS(arr[2], std::out_of_range);
    }

    SECTION("Resize to zero leaves an empty but usable array")
    {
        CMyArray<int> arr;
        arr.PushBack(1);

        arr.Resize(0);

        REQUIRE(arr.GetSize() == 0);
        REQUIRE_THROWS_AS(arr[0], std::out_of_range);
        REQUIRE_NOTHROW(arr.PushBack(42));
        REQUIRE(arr[0] == 42);
    }

    SECTION("New elements after Resize are initialized")
    {
        CMyArray<int> intArr;
        intArr.Resize(3);
        for (size_t i = 0; i < 3; ++i)
        {
            REQUIRE(intArr[i] == 0);
        }

        CMyArray<std::string> strArr;
        strArr.Resize(3);
        for (size_t i = 0; i < 3; ++i)
        {
            REQUIRE(strArr[i] == "");
        }
    }

    SECTION("Resize up beyond capacity (existing elements preserved, new ones zeroed)")
    {
        CMyArray<int> arr;
        arr.PushBack(7);
        arr.PushBack(8);

        arr.Resize(5);

        REQUIRE(arr.GetSize() == 5);
        REQUIRE(arr[0] == 7);
        REQUIRE(arr[1] == 8);
        REQUIRE(arr[2] == 0);
        REQUIRE(arr[4] == 0);
    }

    SECTION("Resize up within existing capacity requires no reallocation")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 5; ++i)
        {
            arr.PushBack(i);
        }

        arr.Resize(2);
        arr.Resize(6);

        REQUIRE(arr.GetSize() == 6);
        REQUIRE(arr[0] == 0);
        REQUIRE(arr[1] == 1);
        REQUIRE(arr[2] == 0);
        REQUIRE(arr[5] == 0);
    }

    SECTION("Resize from empty array")
    {
        CMyArray<int> arr;
        arr.Resize(4);

        REQUIRE(arr.GetSize() == 4);
        for (size_t i = 0; i < 4; ++i)
        {
            REQUIRE(arr[i] == 0);
        }
    }
}

TEST_CASE("Clear empties the array while keeping it reusable", "[clear]")
{
    SECTION("Clear on empty array does not crash")
    {
        CMyArray<int> arr;
        REQUIRE_NOTHROW(arr.Clear());
        REQUIRE(arr.GetSize() == 0);
    }

    SECTION("Clear removes all elements")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.PushBack(2);
        arr.PushBack(3);

        arr.Clear();

        REQUIRE(arr.GetSize() == 0);
        REQUIRE_THROWS_AS(arr[0], std::out_of_range);
    }

    SECTION("PushBack after Clear produces correct state")
    {
        CMyArray<std::string> arr;
        arr.PushBack("hello");
        arr.PushBack("world");

        arr.Clear();
        arr.PushBack("new");

        REQUIRE(arr.GetSize() == 1);
        REQUIRE(arr[0] == "new");
    }

    SECTION("Fill => Clear => Fill")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 4; ++i)
        {
            arr.PushBack(i * 100);
        }

        arr.Clear();

        for (int i = 0; i < 4; ++i)
        {
            arr.PushBack(i);
        }

        for (int i = 0; i < 4; ++i)
        {
            REQUIRE(arr[i] == i);
        }
    }
}


TEST_CASE("Copy constructor creates an independent deep copy", "[copy]")
{
    SECTION("Copying empty array gives empty array")
    {
        CMyArray<int> original;
        CMyArray<int> copy(original);

        REQUIRE(copy.GetSize() == 0);
    }

    SECTION("Copied elements equal source elements")
    {
        CMyArray<std::string> original;
        original.PushBack("alpha");
        original.PushBack("beta");
        original.PushBack("gamma");

        CMyArray<std::string> copy(original);

        REQUIRE(copy.GetSize() == 3);
        REQUIRE(copy[0] == "alpha");
        REQUIRE(copy[1] == "beta");
        REQUIRE(copy[2] == "gamma");
    }

    SECTION("Modifying original after copy does not affect the copy")
    {
        CMyArray<int> original;
        original.PushBack(1);
        original.PushBack(2);

        CMyArray<int> copy(original);
        original[0] = 99;

        REQUIRE(copy[0] == 1);
    }

    SECTION("Modifying copy does not affect the original")
    {
        CMyArray<int> original;
        original.PushBack(1);
        original.PushBack(2);

        CMyArray<int> copy(original);
        copy[1] = 88;

        REQUIRE(original[1] == 2);
    }
}

TEST_CASE("Copy assignment operator", "[copy]")
{
    SECTION("Self-assignment leaves array unchanged")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.PushBack(2);

        arr = arr;

        REQUIRE(arr.GetSize() == 2);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[1] == 2);
    }

    SECTION("Assigning larger array to smaller replaces all content")
    {
        CMyArray<int> small;
        small.PushBack(0);

        CMyArray<int> large;
        for (int i = 0; i < 5; ++i)
        {
            large.PushBack(i);
        }

        small = large;

        REQUIRE(small.GetSize() == 5);
        for (int i = 0; i < 5; ++i)
        {
            REQUIRE(small[i] == i);
        }
    }

    SECTION("Assigning smaller array to larger truncates the result")
    {
        CMyArray<int> small;
        small.PushBack(42);

        CMyArray<int> large;
        for (int i = 0; i < 5; ++i)
        {
            large.PushBack(i);
        }

        large = small;

        REQUIRE(large.GetSize() == 1);
        REQUIRE(large[0] == 42);
        REQUIRE_THROWS_AS(large[1], std::out_of_range);
    }

    SECTION("Assigning empty to non-empty clears the destination")
    {
        CMyArray<int> empty;
        CMyArray<int> dest;
        dest.PushBack(1);
        dest.PushBack(2);

        dest = empty;

        REQUIRE(dest.GetSize() == 0);
    }

    SECTION("Result of assignment is an independent copy")
    {
        CMyArray<int> src;
        src.PushBack(10);
        src.PushBack(20);

        CMyArray<int> dst;
        dst = src;
        src[0] = 99;

        REQUIRE(dst[0] == 10);
    }
}


TEST_CASE("Move constructor transfers ownership", "[move]")
{
    SECTION("Moved-into array holds all original elements")
    {
        CMyArray<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);

        CMyArray<int> dest(std::move(source));

        REQUIRE(dest.GetSize() == 3);
        REQUIRE(dest[0] == 1);
        REQUIRE(dest[2] == 3);
    }

    SECTION("Moved-from array is in a valid and reusable state")
    {
        CMyArray<int> source;
        source.PushBack(1);

        CMyArray<int> dest(std::move(source));

        REQUIRE_NOTHROW(source.GetSize());
        REQUIRE_NOTHROW(source.Clear());
        source.PushBack(99);
        REQUIRE(source[0] == 99);
    }

    SECTION("Moving empty array is safe")
    {
        CMyArray<int> source;
        CMyArray<int> dest(std::move(source));

        REQUIRE(dest.GetSize() == 0);
    }
}

TEST_CASE("Move assignment operator", "[move]")
{
    SECTION("Destination receives source elements")
    {
        CMyArray<std::string> source;
        source.PushBack("foo");
        source.PushBack("bar");

        CMyArray<std::string> dest;
        dest.PushBack("old");
        dest = std::move(source);

        REQUIRE(dest.GetSize() == 2);
        REQUIRE(dest[0] == "foo");
        REQUIRE(dest[1] == "bar");
    }

    SECTION("Old content of destination is replaced")
    {
        CMyArray<int> source;
        source.PushBack(42);

        CMyArray<int> dest;
        dest.PushBack(1);
        dest.PushBack(2);
        dest.PushBack(3);

        dest = std::move(source);

        REQUIRE(dest.GetSize() == 1);
        REQUIRE(dest[0] == 42);
    }

    SECTION("Moved-from array can be reused after assignment")
    {
        CMyArray<int> source;
        source.PushBack(1);
        source.PushBack(2);

        CMyArray<int> dest = std::move(source);
        source.PushBack(99);

        REQUIRE(source.GetSize() == 1);
        REQUIRE(source[0] == 99);
    }
}


TEST_CASE("Iterators traverse elements correctly", "[iterators]")
{
    SECTION("All iterator pairs are equal for an empty array")
    {
        CMyArray<int> arr;

        REQUIRE(arr.begin() == arr.end());
        REQUIRE(arr.rbegin() == arr.rend());
        REQUIRE(arr.cbegin() == arr.cend());
        REQUIRE(arr.crbegin() == arr.crend());
    }

    SECTION("Distance from begin to end equals GetSize()")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 5; ++i)
        {
            arr.PushBack(i);
        }

        const auto dist = static_cast<size_t>(std::distance(arr.begin(), arr.end()));
        REQUIRE(dist == arr.GetSize());
    }

    SECTION("Range-based for loop produces correct sequence")
    {
        CMyArray<int> arr;
        arr.PushBack(10);
        arr.PushBack(20);
        arr.PushBack(30);

        std::vector<int> collected;
        for (const auto& v : arr)
        {
            collected.push_back(v);
        }

        REQUIRE(collected == std::vector<int>{10, 20, 30});
    }

    SECTION("Reverse iterators produce reversed sequence")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.PushBack(2);
        arr.PushBack(3);

        const std::vector<int> reversed(arr.rbegin(), arr.rend());
        REQUIRE(reversed == std::vector<int>{3, 2, 1});
    }

    SECTION("Non-const iterator allows in-place modification")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.PushBack(2);
        arr.PushBack(3);

        for (auto& v : arr)
        {
            v *= 2;
        }

        REQUIRE(arr[0] == 2);
        REQUIRE(arr[1] == 4);
        REQUIRE(arr[2] == 6);
    }

    SECTION("Const iterator from const reference traverses correctly")
    {
        CMyArray<int> arr;
        arr.PushBack(5);
        arr.PushBack(10);
        arr.PushBack(15);

        const CMyArray<int>& constRef = arr;
        int sum = 0;
        for (const auto& v : constRef)
        {
            sum += v;
        }

        REQUIRE(sum == 30);
    }

    SECTION("std::find works on the array's iterator range")
    {
        CMyArray<int> arr;
        arr.PushBack(10);
        arr.PushBack(20);
        arr.PushBack(30);

        const auto found = std::find(arr.begin(), arr.end(), 20);
        REQUIRE(found != arr.end());
        REQUIRE(*found == 20);

        const auto notFound = std::find(arr.begin(), arr.end(), 99);
        REQUIRE(notFound == arr.end());
    }
}

TEST_CASE("Failed copy construction does not leak resources", "[exception_safety]")
{
    ThrowingCopyGuard guard;

    CMyArray<ThrowingCopy> original;
    for (int i = 0; i < 5; ++i)
    {
        ThrowingCopy::Reset();
        original.PushBack(ThrowingCopy(i));
    }
    ThrowingCopy::Reset();

    ThrowingCopy::SetThrowAfter(3);
    REQUIRE_THROWS(CMyArray<ThrowingCopy>(original));

    ThrowingCopy::Reset();

    REQUIRE(original.GetSize() == 5);
    for (int i = 0; i < 5; ++i)
    {
        REQUIRE(original[i].m_value == i);
    }
}

TEST_CASE("Copy assignment provides strong exception guarantee", "[exception_safety]")
{
    ThrowingCopyGuard guard;

    CMyArray<ThrowingCopy> source;
    for (int i = 0; i < 5; ++i)
    {
        ThrowingCopy::Reset();
        source.PushBack(ThrowingCopy(i));
    }
    ThrowingCopy::Reset();

    CMyArray<ThrowingCopy> dest;
    dest.PushBack(ThrowingCopy(99));
    ThrowingCopy::Reset();

    ThrowingCopy::SetThrowAfter(3);
    REQUIRE_THROWS(dest = source);

    ThrowingCopy::Reset();

    REQUIRE(dest.GetSize() == 1);
    REQUIRE(dest[0].m_value == 99);
}

TEST_CASE("Combined operations maintain consistency", "[combined]")
{
    SECTION("PushBack after Clear reuses pre-existing capacity")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 4; ++i)
        {
            arr.PushBack(i);
        }

        arr.Clear();
        arr.PushBack(42);

        REQUIRE(arr.GetSize() == 1);
        REQUIRE(arr[0] == 42);
    }

    SECTION("Resize down then up")
    {
        CMyArray<int> arr;
        for (int i = 0; i < 6; ++i)
        {
            arr.PushBack(i);
        }

        arr.Resize(3);
        arr.Resize(6);

        REQUIRE(arr[0] == 0);
        REQUIRE(arr[2] == 2);
        REQUIRE(arr[3] == 0);
        REQUIRE(arr[5] == 0);
    }

    SECTION("Copying after Resize gives complete and independent copy")
    {
        CMyArray<int> arr;
        arr.PushBack(1);
        arr.Resize(5);

        CMyArray<int> copy(arr);

        REQUIRE(copy.GetSize() == 5);
        REQUIRE(copy[0] == 1);
        REQUIRE(copy[4] == 0);

        copy[0] = 99;
        REQUIRE(arr[0] == 1);
    }

    SECTION("Iteration on moved-into array works correctly")
    {
        CMyArray<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);

        CMyArray<int> dest(std::move(source));

        int sum = 0;
        for (const auto& v : dest)
        {
            sum += v;
        }

        REQUIRE(sum == 6);
    }

    SECTION("std::string array")
    {
        CMyArray<std::string> arr;
        arr.PushBack("one");
        arr.PushBack("two");
        arr.PushBack("three");

        CMyArray<std::string> copy = arr;
        arr.Clear();

        REQUIRE(arr.GetSize() == 0);
        REQUIRE(copy.GetSize() == 3);
        REQUIRE(copy[1] == "two");
    }

    SECTION("double array")
    {
        CMyArray<double> arr;
        arr.PushBack(1.5);
        arr.PushBack(2.5);
        arr.PushBack(3.0);

        double sum = 0.0;
        for (const auto& v : arr)
        {
            sum += v;
        }

        REQUIRE(sum == 7.0);
    }
}
