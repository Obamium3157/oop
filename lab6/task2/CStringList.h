#ifndef OOP_CSTRINGLIST_H
#define OOP_CSTRINGLIST_H

#include <cstddef>
#include <iterator>
#include <string>

class CStringList
{
    struct Node
    {
        std::string value;
        Node* prev = nullptr;
        Node* next = nullptr;

        Node() = default;
        explicit Node(std::string val);
    };

public:
    class Iterator
    {
        friend class CStringList;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        using pointer = std::string*;
        using reference = std::string&;

        Iterator() = default;

        reference operator*() const;
        pointer operator->() const;

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        explicit Iterator(Node* node);
        Node* m_node = nullptr;
    };

    class ConstIterator
    {
        friend class CStringList;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::string*;
        using reference = const std::string&;

        ConstIterator() = default;
        ConstIterator(Iterator it);

        reference operator*() const;
        pointer operator->() const;

        ConstIterator& operator++();
        ConstIterator operator++(int);
        ConstIterator& operator--();
        ConstIterator operator--(int);

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        explicit ConstIterator(Node* node);
        Node* m_node = nullptr;
    };

    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    CStringList();
    CStringList(const CStringList& other);
    CStringList(CStringList&& other) noexcept;
    ~CStringList();

    CStringList& operator=(const CStringList& other);
    CStringList& operator=(CStringList&& other) noexcept;

    void PushBack(const std::string& value);
    void PushBack(std::string&& value);
    void PushFront(const std::string& value);
    void PushFront(std::string&& value);
    Iterator Insert(ConstIterator pos, const std::string& value);
    Iterator Insert(ConstIterator pos, std::string&& value);
    Iterator Erase(ConstIterator pos);
    void Clear() noexcept;
    void Swap(CStringList& other) noexcept;

    size_t GetSize() const noexcept;
    bool IsEmpty() const noexcept;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;

    ReverseIterator rbegin();
    ReverseIterator rend();
    ConstReverseIterator rbegin() const;
    ConstReverseIterator rend() const;
    ConstReverseIterator crbegin() const;
    ConstReverseIterator crend() const;

private:
    void InsertNodeBefore(Node* pos, Node* newNode) noexcept;
    void UnlinkNode(Node* node) noexcept;

    Node* m_sentinel = nullptr;
    size_t m_size = 0;
};


#endif //OOP_CSTRINGLIST_H
