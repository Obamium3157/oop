#ifndef OOP_CMYLIST_H
#define OOP_CMYLIST_H

#include <cstddef>
#include <iterator>
#include <utility>

template <typename T>
class CMyList
{
private:
    struct NodeBase
    {
        NodeBase* prev = nullptr;
        NodeBase* next = nullptr;
    };

    struct Node : public NodeBase
    {
        T value;

        explicit Node(const T& val) : value(val)
        {
        }

        explicit Node(T&& val) : value(std::move(val))
        {
        }
    };

public:
    class ConstIterator;
    class Iterator;

    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    CMyList()
    {
        InitSentinel();
    }

    CMyList(const CMyList& other)
        : CMyList()
    {
        for (const auto& value : other)
        {
            PushBack(value);
        }
    }

    CMyList(CMyList&& other) noexcept
        : CMyList()
    {
        SwapContents(other);
    }

    ~CMyList() noexcept
    {
        Clear();
    }

    CMyList& operator=(const CMyList& other)
    {
        if (this != &other)
        {
            CMyList temp(other);
            SwapContents(temp);
        }
        return *this;
    }

    CMyList& operator=(CMyList&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            SwapContents(other);
        }
        return *this;
    }

    void PushFront(const T& value)
    {
        Insert(begin(), value);
    }

    void PushFront(T&& value)
    {
        Insert(begin(), std::move(value));
    }

    void PushBack(const T& value)
    {
        Insert(end(), value);
    }

    void PushBack(T&& value)
    {
        Insert(end(), std::move(value));
    }

    Iterator Insert(Iterator pos, const T& value)
    {
        return CreateAndLinkNode(pos, value);
    }

    Iterator Insert(Iterator pos, T&& value)
    {
        return CreateAndLinkNode(pos, std::move(value));
    }

    Iterator Erase(Iterator pos)
    {
        NodeBase* nodeToDelete = pos.m_current;
        NodeBase* successor = nodeToDelete->next;

        UnlinkNode(nodeToDelete);
        delete static_cast<Node*>(nodeToDelete);
        --m_size;

        return Iterator(successor);
    }

    void Clear() noexcept
    {
        while (!IsEmpty())
        {
            Erase(begin());
        }
    }

    Iterator begin()
    {
        return Iterator(m_sentinel.next);
    }

    Iterator end()
    {
        return Iterator(&m_sentinel);
    }

    ConstIterator begin() const
    {
        return ConstIterator(m_sentinel.next);
    }

    ConstIterator end() const
    {
        return ConstIterator(&m_sentinel);
    }

    ConstIterator cbegin() const
    {
        return begin();
    }

    ConstIterator cend() const
    {
        return end();
    }

    ReverseIterator rbegin()
    {
        return ReverseIterator(end());
    }

    ReverseIterator rend()
    {
        return ReverseIterator(begin());
    }

    ConstReverseIterator rbegin() const
    {
        return ConstReverseIterator(end());
    }

    ConstReverseIterator rend() const
    {
        return ConstReverseIterator(begin());
    }

    std::size_t GetSize() const
    {
        return m_size;
    }

    bool IsEmpty() const
    {
        return m_size == 0;
    }

    class ConstIterator
    {
        friend class CMyList;
        friend class Iterator;

    public:
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        ConstIterator() = default;

        ConstIterator(const Iterator& it)
            : m_current(it.m_current)
        {
        }

        reference operator*() const
        {
            return static_cast<const Node*>(m_current)->value;
        }

        pointer operator->() const
        {
            return &static_cast<const Node*>(m_current)->value;
        }

        ConstIterator& operator++()
        {
            m_current = m_current->next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        ConstIterator& operator--()
        {
            m_current = m_current->prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            auto copy = *this;
            --(*this);
            return copy;
        }

        bool operator==(const ConstIterator& other) const
        {
            return m_current == other.m_current;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return !(*this == other);
        }

    private:
        explicit ConstIterator(const NodeBase* node)
            : m_current(node)
        {
        }

        const NodeBase* m_current = nullptr;
    };

    class Iterator
    {
        friend class CMyList;
        friend class ConstIterator;

    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        Iterator() = default;

        T& operator*() const
        {
            return static_cast<Node*>(m_current)->value;
        }

        T* operator->() const
        {
            return &static_cast<Node*>(m_current)->value;
        }

        Iterator& operator++()
        {
            m_current = m_current->next;
            return *this;
        }

        Iterator operator++(int)
        {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        Iterator& operator--()
        {
            m_current = m_current->prev;
            return *this;
        }

        Iterator operator--(int)
        {
            auto copy = *this;
            --(*this);
            return copy;
        }

        bool operator==(const Iterator& other) const
        {
            return m_current == other.m_current;
        }

        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;

    private:
        explicit Iterator(NodeBase* node) : m_current(node)
        {
        }

        NodeBase* m_current = nullptr;
    };

private:
    NodeBase m_sentinel;
    std::size_t m_size = 0;

    void InitSentinel() noexcept
    {
        m_sentinel.next = &m_sentinel;
        m_sentinel.prev = &m_sentinel;
    }

    template <typename U>
    Iterator CreateAndLinkNode(Iterator pos, U&& value)
    {
        Node* newNode = new Node(std::forward<U>(value));
        LinkNodeBefore(pos.m_current, newNode);
        ++m_size;
        return Iterator(newNode);
    }

    static void LinkNodeBefore(NodeBase* position, NodeBase* newNode) noexcept
    {
        newNode->prev = position->prev;
        newNode->next = position;
        position->prev->next = newNode;
        position->prev = newNode;
    }

    static void UnlinkNode(NodeBase* node) noexcept
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void SwapContents(CMyList& other) noexcept
    {
        std::swap(m_size, other.m_size);
        TransferSentinelLinks(m_sentinel, other.m_sentinel);
    }

    static void TransferSentinelLinks(NodeBase& a, NodeBase& b) noexcept
    {
        const bool aIsEmpty = (a.next == &a);
        const bool bIsEmpty = (b.next == &b);

        if (!aIsEmpty && !bIsEmpty)
        {
            std::swap(a.next, b.next);
            std::swap(a.prev, b.prev);
            a.next->prev = &a;
            a.prev->next = &a;
            b.next->prev = &b;
            b.prev->next = &b;
        }
        else if (!aIsEmpty)
        {
            b.next = a.next;
            b.prev = a.prev;
            b.next->prev = &b;
            b.prev->next = &b;
            a.next = &a;
            a.prev = &a;
        }
        else if (!bIsEmpty)
        {
            a.next = b.next;
            a.prev = b.prev;
            a.next->prev = &a;
            a.prev->next = &a;
            b.next = &b;
            b.prev = &b;
        }
    }
};


template <typename T>
bool CMyList<T>::Iterator::operator==(const CMyList<T>::ConstIterator& other) const
{
    return m_current == other.m_current;
}

template <typename T>
bool CMyList<T>::Iterator::operator!=(const CMyList<T>::ConstIterator& other) const
{
    return !(*this == other);
}

#endif //OOP_CMYLIST_H
