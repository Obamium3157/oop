#include "CStringList.h"

CStringList::Node::Node(const std::string& val)
    : value(val)
{
}

CStringList::Iterator::Iterator(Node* node)
    : m_node(node)
{
}

CStringList::ConstIterator::ConstIterator(Iterator it)
    : m_node(it.m_node)
{
}

CStringList::ConstIterator::ConstIterator(Node* node)
    : m_node(node)
{
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
