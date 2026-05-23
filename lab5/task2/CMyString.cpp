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
    : m_data(s_emptyBuffer)
    , m_length(0)
    , m_capacity(0)
{
    // TODO: эта строка много где дублируется
    if (pString == nullptr)
    {
        throw std::invalid_argument("CMyString: pString must be non-null");
    }

    const size_t length = std::strlen(pString);
    *this = CMyString(pString, length);
}

CMyString::CMyString(const char* pString, const size_t length)
    : m_data(s_emptyBuffer)
    , m_length(0)
    , m_capacity(0)
{
    if (pString == nullptr)
    {
        throw std::invalid_argument("CMyString: pString must be non-null");
    }

    if (length == 0)
    {
        return;
    }

    m_data = new char[length + 1];
    m_length = length;
    m_capacity = length;

    // TODO: посмотреть, кидает ли исключения
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
    if (!IsEmpty())
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
    if (!IsEmpty())
    {
        delete[] m_data;
    }

    m_data = s_emptyBuffer;
    m_length = 0;
    m_capacity = 0;
}


CMyString& CMyString::operator=(CMyString const& other)
{
    if (this == &other)
    {
        return *this;
    }

    CMyString temp(other);
    std::swap(m_data, temp.m_data);
    std::swap(m_length, temp.m_length);
    std::swap(m_capacity, temp.m_capacity);

    return *this;
}

CMyString& CMyString::operator=(CMyString&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (!IsEmpty())
    {
        delete[] m_data;
    }

    m_data = other.m_data;
    m_length = other.m_length;
    m_capacity = other.m_capacity;

    other.m_data = s_emptyBuffer;
    other.m_length = 0;
    other.m_capacity = 0;

    return *this;
}

CMyString& CMyString::operator+=(CMyString const& other)
{
    if (other.m_length == 0)
    {
        return *this;
    }

    const size_t newLength = m_length + other.m_length;

    if (newLength > m_capacity)
    {
        const size_t newCapacity = std::max(newLength, m_capacity * 2);
        Reallocate(newCapacity);
    }

    std::memcpy(m_data + m_length, other.m_data, other.m_length);
    m_length = newLength;
    m_data[m_length] = '\0';

    return *this;
}


char& CMyString::operator[](const size_t index)
{
    if (index >= m_length)
    {
        throw std::out_of_range("CMyString: index out of range");
    }

    return m_data[index];
}

const char& CMyString::operator[](const size_t index) const
{
    if (index >= m_length)
    {
        throw std::out_of_range("CMyString: index out of range");
    }

    return m_data[index];
}

CMyString operator+(CMyString lhs, CMyString const& rhs)
{
    lhs += rhs;

    return lhs;
}

CMyString operator+(std::string const& lhs, CMyString const& rhs)
{
    return CMyString(lhs) + rhs;
}

CMyString operator+(const char* lhs, CMyString const& rhs)
{
    return CMyString(lhs) + rhs;
}


bool operator==(CMyString const& lhs, CMyString const& rhs)
{
    if (lhs.m_length != rhs.m_length)
    {
        return false;
    }

    return std::memcmp(lhs.m_data, rhs.m_data, lhs.m_length) == 0;
}

std::strong_ordering operator<=>(CMyString const& lhs, CMyString const& rhs)
{
    const size_t commonLength = std::min(lhs.m_length, rhs.m_length);

    if (const int result = std::memcmp(lhs.m_data, rhs.m_data, commonLength);
        result != 0)
    {
        return result < 0
            ? std::strong_ordering::less
            : std::strong_ordering::greater;
    }

    return lhs.m_length <=> rhs.m_length;
}

std::ostream& operator<<(std::ostream& stream, CMyString const& str)
{
    stream.write(str.GetStringData(), static_cast<std::streamsize>(str.GetLength()));
    return stream;
}

std::istream& operator>>(std::istream& stream, CMyString& str)
{
    str.Clear();

    char ch;
    while (stream.get(ch) && !std::isspace(static_cast<unsigned char>(ch)))
    {
        str += CMyString(&ch, 1);
    }

    return stream;
}


bool CMyString::IsEmpty() const noexcept
{
    return m_data == s_emptyBuffer;
}

void CMyString::Reallocate(const size_t newCapacity)
{
    char* newData = new char[newCapacity + 1];

    std::memcpy(newData, m_data, m_length);
    newData[m_length] = '\0';

    if (!IsEmpty())
    {
        delete[] m_data;
    }

    m_data = newData;
    m_capacity = newCapacity;
}
