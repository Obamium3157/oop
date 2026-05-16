#include "CMyString.h"
#include <stdexcept>

char CMyString::s_emptyBuffer[1] = { '\0' };


CMyString::CMyString()
{
    throw std::logic_error("TODO: implement");
}

CMyString::CMyString(const char* pString)
{
    throw std::logic_error("TODO: implement");
}

CMyString::CMyString(const char* pString, size_t length)
{
    throw std::logic_error("TODO: implement");
}

CMyString::CMyString(CMyString const& other)
{
    throw std::logic_error("TODO: implement");
}

CMyString::CMyString(CMyString&& other) noexcept
{
    throw std::logic_error("TODO: implement");
}

CMyString::CMyString(std::string const& stlString)
{
    throw std::logic_error("TODO: implement");
}

CMyString::~CMyString()
{
    // TODO: implement
}


size_t CMyString::GetLength() const
{
    throw std::logic_error("TODO: implement");
}

const char* CMyString::GetStringData() const
{
    throw std::logic_error("TODO: implement");
}

CMyString CMyString::SubString(size_t start, size_t length) const
{
    throw std::logic_error("TODO: implement");
}

void CMyString::Clear()
{
    throw std::logic_error("TODO: implement");
}

size_t CMyString::GetCapacity() const
{
    throw std::logic_error("TODO: implement");
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
    throw std::logic_error("TODO: implement");
}

void CMyString::Reallocate(size_t newCapacity)
{
    throw std::logic_error("TODO: implement");
}