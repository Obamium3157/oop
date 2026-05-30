#include "CStringList.h"

#include <memory>

CStringList::Node::Node(std::string val)
    : value(std::move(val))
{
}

CStringList::Iterator::Iterator(Node* node)
    : m_node(node)
{
}

CStringList::ConstIterator::ConstIterator(const Iterator it)
    : m_node(it.m_node)
{
}

CStringList::ConstIterator::ConstIterator(Node* node)
    : m_node(node)
{
}

CStringList::CStringList()
    : m_sentinel(new Node())
    , m_size(0)
{
    m_sentinel->next = m_sentinel;
    m_sentinel->prev = m_sentinel;
}

CStringList::CStringList(const CStringList& other)
    : CStringList()
{
    for (const auto& value : other)
    {
        PushBack(value);
    }
}

CStringList::CStringList(CStringList&& other) noexcept
    : m_sentinel(other.m_sentinel)
    , m_size(other.m_size)
{
    other.m_sentinel = nullptr;
    other.m_size = 0;
}

CStringList::~CStringList()
{
    if (m_sentinel == nullptr)
    {
        return;
    }

    Clear();
    delete m_sentinel;
}

CStringList::Iterator::reference CStringList::Iterator::operator*() const
{
    return m_node->value;
}

CStringList::Iterator::pointer CStringList::Iterator::operator->() const
{
    return &m_node->value;
}

CStringList::Iterator& CStringList::Iterator::operator++()
{
    m_node = m_node->next;
    return *this;
}

CStringList::Iterator CStringList::Iterator::operator++(int)
{
    const Iterator previous = *this;
    ++(*this);
    return previous;
}

CStringList::Iterator& CStringList::Iterator::operator--()
{
    m_node = m_node->prev;
    return *this;
}

CStringList::Iterator CStringList::Iterator::operator--(int)
{
    const Iterator previous = *this;
    --(*this);
    return previous;
}

bool CStringList::Iterator::operator==(const Iterator& other) const
{
    return m_node == other.m_node;
}

bool CStringList::Iterator::operator!=(const Iterator& other) const
{
    return m_node != other.m_node;
}

CStringList::ConstIterator::reference CStringList::ConstIterator::operator*() const
{
    return m_node->value;
}

CStringList::ConstIterator::pointer CStringList::ConstIterator::operator->() const
{
    return &m_node->value;
}

CStringList::ConstIterator& CStringList::ConstIterator::operator++()
{
    m_node = m_node->next;
    return *this;
}

CStringList::ConstIterator CStringList::ConstIterator::operator++(int)
{
    const ConstIterator previous = *this;
    ++(*this);
    return previous;
}

CStringList::ConstIterator& CStringList::ConstIterator::operator--()
{
    m_node = m_node->prev;
    return *this;
}

CStringList::ConstIterator CStringList::ConstIterator::operator--(int)
{
    const ConstIterator previous = *this;
    --(*this);
    return previous;
}

bool CStringList::ConstIterator::operator==(const ConstIterator& other) const
{
    return m_node == other.m_node;
}

bool CStringList::ConstIterator::operator!=(const ConstIterator& other) const
{
    return m_node != other.m_node;
}

CStringList& CStringList::operator=(const CStringList& other)
{
    if (this != &other)
    {
        CStringList tmp(other);
        Swap(tmp);
    }

    return *this;
}

CStringList& CStringList::operator=(CStringList&& other) noexcept
{
    if (this != &other)
    {
        Swap(other);
    }

    return *this;
}

void CStringList::InsertNodeBefore(Node* pos, Node* newNode) noexcept
{
    newNode->next = pos;
    newNode->prev = pos->prev;
    pos->prev->next = newNode;
    pos->prev = newNode;
}

void CStringList::UnlinkNode(Node* node) noexcept
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

CStringList::Iterator CStringList::Insert(const ConstIterator pos, const std::string& value)
{
    auto newNode = std::make_unique<Node>(value);
    InsertNodeBefore(pos.m_node, newNode.get());
    ++m_size;
    return Iterator(newNode.release());
}

CStringList::Iterator CStringList::Insert(const ConstIterator pos, std::string&& value)
{
    auto newNode = std::make_unique<Node>(std::move(value));
    InsertNodeBefore(pos.m_node, newNode.get());
    ++m_size;
    return Iterator(newNode.release());
}

CStringList::Iterator CStringList::Erase(const ConstIterator pos)
{
    Node* nodeToErase = pos.m_node;
    Node* nextNode = nodeToErase->next;
    UnlinkNode(nodeToErase);
    delete nodeToErase;
    --m_size;
    return Iterator(nextNode);
}

void CStringList::PushBack(const std::string& value)
{
    Insert(cend(), value);
}

void CStringList::PushBack(std::string&& value)
{
    Insert(cend(), std::move(value));
}

void CStringList::PushFront(const std::string& value)
{
    Insert(cbegin(), value);
}

void CStringList::PushFront(std::string&& value)
{
    Insert(cbegin(), std::move(value));
}

void CStringList::Clear() noexcept
{
    Node* current = m_sentinel->next;
    while (current != m_sentinel)
    {
        Node* next = current->next;
        delete current;
        current = next;
    }

    m_sentinel->next = m_sentinel;
    m_sentinel->prev = m_sentinel;
    m_size = 0;
}

void CStringList::Swap(CStringList& other) noexcept
{
    std::swap(m_sentinel, other.m_sentinel);
    std::swap(m_size, other.m_size);
}

size_t CStringList::GetSize() const noexcept
{
    return m_size;
}

bool CStringList::IsEmpty() const noexcept
{
    return m_size == 0;
}

CStringList::Iterator CStringList::begin()
{
    return Iterator(m_sentinel->next);
}

CStringList::Iterator CStringList::end()
{
    return Iterator(m_sentinel);
}

CStringList::ConstIterator CStringList::begin() const
{
    return ConstIterator(m_sentinel->next);
}

CStringList::ConstIterator CStringList::end() const
{
    return ConstIterator(m_sentinel);
}

CStringList::ConstIterator CStringList::cbegin() const
{
    return begin();
}

CStringList::ConstIterator CStringList::cend() const
{
    return end();
}

CStringList::ReverseIterator CStringList::rbegin()
{
    return ReverseIterator(end());
}

CStringList::ReverseIterator CStringList::rend()
{
    return ReverseIterator(begin());
}

CStringList::ConstReverseIterator CStringList::rbegin() const
{
    return ConstReverseIterator(end());
}

CStringList::ConstReverseIterator CStringList::rend() const
{
    return ConstReverseIterator(begin());
}

CStringList::ConstReverseIterator CStringList::crbegin() const
{
    return rbegin();
}

CStringList::ConstReverseIterator CStringList::crend() const
{
    return rend();
}