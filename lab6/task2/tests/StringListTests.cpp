#include <catch2/catch_test_macros.hpp>

#include "../CStringList.h"

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

namespace
{
    CStringList MakeList(const std::initializer_list<std::string> values)
    {
        CStringList list;
        for (const auto& value : values)
        {
            list.PushBack(value);
        }
        return list;
    }

    std::vector<std::string> ToVector(const CStringList& list)
    {
        return {list.begin(), list.end()};
    }
}


TEST_CASE("Default constructor creates empty list", "[constructor]")
{
    const CStringList list;
    REQUIRE(list.IsEmpty());
    REQUIRE(list.GetSize() == 0);
    REQUIRE(list.begin() == list.end());
}


TEST_CASE("PushBack appends elements to the end", "[modifier]")
{
    CStringList list;

    SECTION("Single element")
    {
        list.PushBack("hello");
        REQUIRE(list.GetSize() == 1);
        REQUIRE_FALSE(list.IsEmpty());
        REQUIRE(*list.begin() == "hello");
    }

    SECTION("Multiple elements preserve insertion order")
    {
        list.PushBack("a");
        list.PushBack("b");
        list.PushBack("c");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("Rvalue overload")
    {
        std::string s = "moved";
        list.PushBack(std::move(s));
        REQUIRE(*list.begin() == "moved");
    }

    SECTION("Empty string is valid value")
    {
        list.PushBack("");
        REQUIRE(list.GetSize() == 1);
        REQUIRE(*list.begin() == "");
    }
}


TEST_CASE("PushFront prepends elements to the beginning", "[modifier]")
{
    CStringList list;

    SECTION("Single element")
    {
        list.PushFront("hello");
        REQUIRE(list.GetSize() == 1);
        REQUIRE(*list.begin() == "hello");
    }

    SECTION("Multiple elements produce reversed order")
    {
        list.PushFront("a");
        list.PushFront("b");
        list.PushFront("c");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "c", "b", "a" });
    }

    SECTION("Rvalue overload")
    {
        std::string s = "moved";
        list.PushFront(std::move(s));
        REQUIRE(*list.begin() == "moved");
    }
}


TEST_CASE("Insert places element at the given position", "[modifier]")
{
    CStringList list = MakeList({"a", "c"});

    SECTION("At begin() equals PushFront")
    {
        const auto it = list.Insert(list.cbegin(), "z");
        REQUIRE(*it == "z");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "z", "a", "c" });
    }

    SECTION("At end() equals PushBack")
    {
        const auto it = list.Insert(list.cend(), "z");
        REQUIRE(*it == "z");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "c", "z" });
    }

    SECTION("In the middle")
    {
        const auto it = list.Insert(std::next(list.cbegin()), "b");
        REQUIRE(*it == "b");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("Into empty list")
    {
        CStringList empty;
        const auto it = empty.Insert(empty.cend(), "x");
        REQUIRE(*it == "x");
        REQUIRE(empty.GetSize() == 1);
    }

    SECTION("Size increases by one")
    {
        const size_t sizeBefore = list.GetSize();
        list.Insert(list.cbegin(), "x");
        REQUIRE(list.GetSize() == sizeBefore + 1);
    }

    SECTION("Rvalue overload")
    {
        std::string s = "b";
        list.Insert(std::next(list.cbegin()), std::move(s));
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b", "c" });
    }
}


TEST_CASE("Erase removes element at the given position", "[modifier]")
{
    SECTION("Only element => empty list")
    {
        CStringList list = MakeList({"a"});
        const auto it = list.Erase(list.cbegin());
        REQUIRE(list.IsEmpty());
        REQUIRE(it == list.end());
    }

    SECTION("First element => iterator points to new first")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const auto it = list.Erase(list.cbegin());
        REQUIRE(*it == "b");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "b", "c" });
    }

    SECTION("Last element returns end()")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const auto it = list.Erase(std::prev(list.cend()));
        REQUIRE(it == list.end());
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b" });
    }

    SECTION("Middle element")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const auto it = list.Erase(std::next(list.cbegin()));
        REQUIRE(*it == "c");
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "c" });
    }

    SECTION("Size decreases by one")
    {
        CStringList list = MakeList({"a", "b"});
        const size_t sizeBefore = list.GetSize();
        list.Erase(list.cbegin());
        REQUIRE(list.GetSize() == sizeBefore - 1);
    }
}


TEST_CASE("Clear empties the list", "[modifier]")
{
    SECTION("Clearing empty list does nothing")
    {
        CStringList list;
        REQUIRE_NOTHROW(list.Clear());
        REQUIRE(list.IsEmpty());
    }

    SECTION("Size becomes zero; begin() equals end()")
    {
        CStringList list = MakeList({"a", "b", "c"});
        list.Clear();
        REQUIRE(list.IsEmpty());
        REQUIRE(list.GetSize() == 0);
        REQUIRE(list.begin() == list.end());
    }

    SECTION("List is reusable after clear")
    {
        CStringList list = MakeList({"a", "b"});
        list.Clear();
        list.PushBack("x");
        REQUIRE(list.GetSize() == 1);
        REQUIRE(*list.begin() == "x");
    }
}

TEST_CASE("Swap exchanges contents of two lists", "[modifier]")
{
    CStringList a = MakeList({"a", "b"});
    CStringList b = MakeList({"x", "y", "z"});
    a.Swap(b);
    REQUIRE(ToVector(a) == std::vector<std::string>{ "x", "y", "z" });
    REQUIRE(ToVector(b) == std::vector<std::string>{ "a", "b" });
    REQUIRE(a.GetSize() == 3);
    REQUIRE(b.GetSize() == 2);
}

TEST_CASE("Swap edge cases", "[modifier]")
{
    SECTION("Swap with empty list")
    {
        CStringList a = MakeList({"a", "b", "c"});
        CStringList empty;
        a.Swap(empty);
        REQUIRE(a.IsEmpty());
        REQUIRE(a.GetSize() == 0);
        REQUIRE(ToVector(empty) == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("Swap two empty lists")
    {
        CStringList a;
        CStringList b;
        a.Swap(b);
        REQUIRE(a.IsEmpty());
        REQUIRE(b.IsEmpty());
    }

    SECTION("Self-swap leaves list unchanged")
    {
        CStringList list = MakeList({"a", "b", "c"});
        list.Swap(list);
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b", "c" });
    }
}


TEST_CASE("Copy constructor produces deep copy", "[constructor]")
{
    SECTION("Elements are copied")
    {
        const CStringList original = MakeList({"a", "b", "c"});
        const CStringList copy(original);
        REQUIRE(ToVector(copy) == ToVector(original));
    }

    SECTION("Modifying copy does not affect original")
    {
        CStringList original = MakeList({"a", "b"});
        CStringList copy(original);
        copy.PushBack("c");
        REQUIRE(original.GetSize() == 2);
    }

    SECTION("Modifying original does not affect copy")
    {
        CStringList original = MakeList({"a", "b"});
        CStringList copy(original);
        original.PushBack("c");
        REQUIRE(copy.GetSize() == 2);
    }

    SECTION("Copying empty list")
    {
        const CStringList empty;
        const CStringList copy(empty);
        REQUIRE(copy.IsEmpty());
        REQUIRE(copy.begin() == copy.end());
    }
}

TEST_CASE("Move constructor transfers ownership", "[constructor]")
{
    SECTION("All elements are present in new object")
    {
        CStringList original = MakeList({"a", "b", "c"});
        const std::vector<std::string> expected = ToVector(original);
        const CStringList moved(std::move(original));
        REQUIRE(ToVector(moved) == expected);
    }

    SECTION("Destructor of moved-from object does not crash")
    {
        auto* p = new CStringList(MakeList({"a", "b"}));
        CStringList moved(std::move(*p));
        REQUIRE_NOTHROW(delete p);
    }

    SECTION("Move constructor is noexcept")
    {
        STATIC_REQUIRE(std::is_nothrow_move_constructible_v<CStringList>);
    }
}

TEST_CASE("Copy assignment operator", "[assignment]")
{
    SECTION("Elements are copied")
    {
        const CStringList source = MakeList({"a", "b", "c"});
        CStringList target;
        target = source;
        REQUIRE(ToVector(target) == ToVector(source));
    }

    SECTION("Target is independent from source after assignment")
    {
        const CStringList source = MakeList({"a", "b"});
        CStringList target;
        target = source;
        target.PushBack("c");
        REQUIRE(source.GetSize() == 2);
        REQUIRE(target.GetSize() == 3);
    }

    SECTION("Old content of target is replaced")
    {
        const CStringList source = MakeList({"x", "y"});
        CStringList target = MakeList({"a", "b", "c"});
        target = source;
        REQUIRE(ToVector(target) == std::vector<std::string>{ "x", "y" });
    }

    SECTION("Self-assignment leaves list unchanged")
    {
        CStringList list = MakeList({"a", "b", "c"});
        list = list;
        REQUIRE(ToVector(list) == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("Copy assignment of empty over non-empty clears target")
    {
        const CStringList empty;
        CStringList target = MakeList({"a", "b", "c"});
        target = empty;
        REQUIRE(target.IsEmpty());
        REQUIRE(target.GetSize() == 0);
        REQUIRE(target.begin() == target.end());
    }
}

TEST_CASE("Move assignment operator", "[assignment]")
{
    SECTION("Elements are transferred")
    {
        CStringList source = MakeList({"a", "b", "c"});
        const std::vector<std::string> expected = ToVector(source);
        CStringList target;
        target = std::move(source);
        REQUIRE(ToVector(target) == expected);
    }

    SECTION("Source is in valid empty state after move")
    {
        CStringList source = MakeList({"a", "b"});
        CStringList target;
        target = std::move(source);
        REQUIRE(source.IsEmpty());
    }

    SECTION("Old content of target is released")
    {
        CStringList source = MakeList({"x"});
        CStringList target = MakeList({"a", "b", "c"});
        target = std::move(source);
        REQUIRE(target.GetSize() == 1);
        REQUIRE(*target.begin() == "x");
    }

    SECTION("Self-move-assignment does not crash")
    {
        CStringList list = MakeList({"a", "b"});
        REQUIRE_NOTHROW(list = std::move(list));
    }

    SECTION("Move assignment is noexcept")
    {
        STATIC_REQUIRE(std::is_nothrow_move_assignable_v<CStringList>);
    }

    SECTION("Move assignment of empty over non-empty clears target")
    {
        CStringList source;
        CStringList target = MakeList({"a", "b", "c"});
        target = std::move(source);
        REQUIRE(target.IsEmpty());
        REQUIRE(target.GetSize() == 0);
    }
}


TEST_CASE("Iterator traversal and mutation", "[iterator]")
{
    SECTION("begin() equals end() for empty list")
    {
        CStringList list;
        REQUIRE(list.begin() == list.end());
    }

    SECTION("begin() points to first element")
    {
        CStringList list = MakeList({"a", "b", "c"});
        REQUIRE(*list.begin() == "a");
    }

    SECTION("Decrementing end() gives last element")
    {
        CStringList list = MakeList({"a", "b", "c"});
        REQUIRE(*std::prev(list.end()) == "c");
    }

    SECTION("Prefix increment returns reference to self")
    {
        CStringList list = MakeList({"a", "b"});
        auto it = list.begin();
        auto& returned = ++it;
        REQUIRE(*it == "b");
        REQUIRE(&returned == &it);
    }

    SECTION("Postfix increment returns copy before advance")
    {
        CStringList list = MakeList({"a", "b"});
        auto it = list.begin();
        const auto before = it++;
        REQUIRE(*before == "a");
        REQUIRE(*it == "b");
    }

    SECTION("Prefix decrement returns reference to self")
    {
        CStringList list = MakeList({"a", "b"});
        auto it = list.end();
        auto& returned = --it;
        REQUIRE(*it == "b");
        REQUIRE(&returned == &it);
    }

    SECTION("Postfix decrement returns copy before retreat")
    {
        CStringList list = MakeList({"a", "b"});
        auto it = list.end();
        const auto before = it--;
        REQUIRE(before == list.end());
        REQUIRE(*it == "b");
    }

    SECTION("Iterator allows mutation of elements")
    {
        CStringList list = MakeList({"a"});
        *list.begin() = "z";
        REQUIRE(*list.begin() == "z");
    }

    SECTION("operator-> gives access to string members")
    {
        CStringList list = MakeList({"hello"});
        REQUIRE(list.begin()->size() == 5);
    }

    SECTION("Range-based for visits all elements in order")
    {
        CStringList list = MakeList({"a", "b", "c"});
        std::vector<std::string> result;
        for (const auto& s : list)
        {
            result.push_back(s);
        }
        REQUIRE(result == std::vector<std::string>{ "a", "b", "c" });
    }
}

TEST_CASE("ConstIterator provides read-only access", "[iterator]")
{
    SECTION("const list iterates via begin()/end()")
    {
        const CStringList list = MakeList({"a", "b", "c"});
        const std::vector<std::string> result(list.begin(), list.end());
        REQUIRE(result == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("Iterator implicitly converts to ConstIterator")
    {
        CStringList list = MakeList({"a", "b"});
        const CStringList::ConstIterator cit = list.begin();
        REQUIRE(*cit == "a");
    }

    SECTION("cbegin()/cend() work on non-const list")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const std::vector<std::string> result(list.cbegin(), list.cend());
        REQUIRE(result == std::vector<std::string>{ "a", "b", "c" });
    }

    SECTION("operator-> gives access to string members")
    {
        const CStringList list = MakeList({"hello"});
        REQUIRE(list.begin()->size() == 5);
    }

    SECTION("cbegin() equals cend() for empty list")
    {
        CStringList list;
        REQUIRE(list.cbegin() == list.cend());
    }
}

TEST_CASE("Reverse iterators traverse in reverse order", "[iterator]")
{
    SECTION("rbegin() equals rend() for empty list")
    {
        CStringList list;
        REQUIRE(list.rbegin() == list.rend());
    }

    SECTION("rbegin() dereferences to last element")
    {
        CStringList list = MakeList({"a", "b", "c"});
        REQUIRE(*list.rbegin() == "c");
    }

    SECTION("Traversal via rbegin()/rend() gives reversed order")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const std::vector<std::string> result(list.rbegin(), list.rend());
        REQUIRE(result == std::vector<std::string>{ "c", "b", "a" });
    }

    SECTION("crbegin()/crend() work on non-const list")
    {
        CStringList list = MakeList({"a", "b", "c"});
        const std::vector<std::string> result(list.crbegin(), list.crend());
        REQUIRE(result == std::vector<std::string>{ "c", "b", "a" });
    }

    SECTION("const list reverse traversal")
    {
        const CStringList list = MakeList({"a", "b", "c"});
        const std::vector<std::string> result(list.rbegin(), list.rend());
        REQUIRE(result == std::vector<std::string>{ "c", "b", "a" });
    }
}

TEST_CASE("STL algorithm compatibility", "[iterator]")
{
    CStringList list = MakeList({"apple", "banana", "cherry"});

    SECTION("std::find locates existing element")
    {
        const auto it = std::find(list.begin(), list.end(), "banana");
        REQUIRE(it != list.end());
        REQUIRE(*it == "banana");
    }

    SECTION("std::find returns end() for missing element")
    {
        const auto it = std::find(list.begin(), list.end(), "grape");
        REQUIRE(it == list.end());
    }

    SECTION("std::distance returns correct element count")
    {
        REQUIRE(std::distance(list.begin(), list.end()) == 3);
    }

    SECTION("std::copy transfers all elements to vector")
    {
        std::vector<std::string> vec;
        std::copy(list.begin(), list.end(), std::back_inserter(vec));
        REQUIRE(vec == std::vector<std::string>{ "apple", "banana", "cherry" });
    }
}

TEST_CASE("Insert does not invalidate existing iterators", "[iterator]")
{
    CStringList list = MakeList({"a", "c"});
    const auto itA = list.begin();
    const auto itC = std::next(itA);

    list.Insert(itC, "b");

    REQUIRE(*itA == "a");
    REQUIRE(*itC == "c");
    REQUIRE(*std::next(itA) == "b");
}

TEST_CASE("Erase does not invalidate iterators to other elements", "[iterator]")
{
    CStringList list = MakeList({"a", "b", "c"});
    const auto itA = list.begin();
    const auto itC = std::prev(list.end());

    list.Erase(std::next(itA));

    REQUIRE(*itA == "a");
    REQUIRE(*itC == "c");
    REQUIRE(std::next(itA) == itC);
}

TEST_CASE("Mixed operations maintain correct state", "[modifier]")
{
    CStringList list;
    list.PushBack("b");
    list.PushFront("a");
    list.PushBack("d");
    list.Insert(std::prev(list.cend()), "c");
    list.Erase(list.cbegin());

    REQUIRE(ToVector(list) == std::vector<std::string>{ "b", "c", "d" });
    REQUIRE(list.GetSize() == 3);
}

TEST_CASE("GetSize is consistent with actual element count", "[modifier]")
{
    CStringList list;
    REQUIRE(list.GetSize() == static_cast<size_t>(std::distance(list.begin(), list.end())));

    list.PushBack("a");
    list.PushBack("b");
    list.PushBack("c");
    REQUIRE(list.GetSize() == static_cast<size_t>(std::distance(list.begin(), list.end())));

    list.Erase(list.cbegin());
    REQUIRE(list.GetSize() == static_cast<size_t>(std::distance(list.begin(), list.end())));

    list.Clear();
    REQUIRE(list.GetSize() == static_cast<size_t>(std::distance(list.begin(), list.end())));
}

TEST_CASE("Destructor handles large list without stack overflow", "[large]")
{
    constexpr size_t count = 10000000;
    auto* list = new CStringList();
    for (size_t i = 0; i < count; ++i)
    {
        list->PushBack("x");
    }
    REQUIRE_NOTHROW(delete list);
}

TEST_CASE("Clear handles large list without stack overflow", "[large]")
{
    constexpr size_t count = 10000000;
    CStringList list;
    for (size_t i = 0; i < count; ++i)
    {
        list.PushBack("x");
    }
    REQUIRE_NOTHROW(list.Clear());
    REQUIRE(list.IsEmpty());
}
