#include "CMyString.h"

#include <cstring>
#include <stdexcept>

CMyString::CMyString()
    : m_data(s_emptyBuffer)
    , m_length(0)
    , m_capacity(0)
{
}

CMyString::CMyString(const char* pString)
    : CMyString(pString, pString ? std::strlen(pString) : 0)
{
}

CMyString::CMyString(const char* pString, const size_t length)
    : m_data(s_emptyBuffer)
    , m_length(0)
    , m_capacity(0)
{
    if (length == 0)
    {
        return;
    }

    m_data = new char[length + 1];
    m_length = length;
    m_capacity = length;

    std::memcpy(m_data, pString, length);
    m_data[length] = '\0';
}

CMyString::CMyString(CMyString const& other)
    : CMyString(other.m_data, other.m_length)
{
}

CMyString::CMyString(CMyString&& other) noexcept
    : m_data(other.m_data)
    , m_length(other.m_length)
    , m_capacity(other.m_capacity)
{
    other.m_data = s_emptyBuffer;
    other.m_length = 0;
    other.m_capacity = 0;
}

CMyString::CMyString(std::string const& stlString)
    : CMyString(stlString.data(), stlString.size())
{
}

CMyString::~CMyString()
{
    if (!IsUsingStaticBuffer())
    {
        delete[] m_data;
    }
}


size_t CMyString::GetLength() const
{
    return m_length;
}

const char* CMyString::GetStringData() const
{
    return m_data;
}

size_t CMyString::GetCapacity() const
{
    return m_capacity;
}

CMyString CMyString::SubString(const size_t start, const size_t length) const
{
    if (start > m_length)
    {
        throw std::out_of_range("SubString: start index out of range");
    }

    const size_t availableLength = m_length - start;
    const size_t actualLength = std::min(length, availableLength);

    return {m_data + start, actualLength};
}

void CMyString::Clear()
{
    if (!IsUsingStaticBuffer())
    {
        delete[] m_data;
    }

    m_data = s_emptyBuffer;
    m_length = 0;
    m_capacity = 0;
}


CMyString& CMyString::operator=(CMyString const& other)
{
    throw std::logic_error("TODO: implement");
}

CMyString& CMyString::operator=(CMyString&& other) noexcept
{
    throw std::logic_error("TODO: implement");
}

CMyString& CMyString::operator+=(CMyString const& other)
{
    throw std::logic_error("TODO: implement");
}


char& CMyString::operator[](size_t index)
{
    throw std::logic_error("TODO: implement");
}

const char& CMyString::operator[](size_t index) const
{
    throw std::logic_error("TODO: implement");
}

CMyString operator+(CMyString lhs, CMyString const& rhs)
{
    throw std::logic_error("TODO: implement");
}

CMyString operator+(std::string const& lhs, CMyString const& rhs)
{
    throw std::logic_error("TODO: implement");
}

CMyString operator+(const char* lhs, CMyString const& rhs)
{
    throw std::logic_error("TODO: implement");
}


bool operator==(CMyString const& lhs, CMyString const& rhs)
{
    throw std::logic_error("TODO: implement");
}

std::strong_ordering operator<=>(CMyString const& lhs, CMyString const& rhs)
{
    throw std::logic_error("TODO: implement");
}

std::ostream& operator<<(std::ostream& stream, CMyString const& str)
{
    throw std::logic_error("TODO: implement");
}

std::istream& operator>>(std::istream& stream, CMyString& str)
{
    throw std::logic_error("TODO: implement");
}


bool CMyString::IsUsingStaticBuffer() const noexcept
{
    return m_data == s_emptyBuffer;
}

void CMyString::Reallocate(size_t newCapacity)
{
    throw std::logic_error("TODO: implement");
}