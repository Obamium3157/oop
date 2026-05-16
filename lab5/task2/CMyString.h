#ifndef OOP_CSTRING_H
#define OOP_CSTRING_H

#include <cstddef>
#include <iostream>
#include <string>

class CMyString
{
public:
    CMyString();
    CMyString(const char* pString);
    CMyString(const char* pString, size_t length);
    CMyString(CMyString const& other);
    CMyString(CMyString&& other) noexcept;
    CMyString(std::string const& stlString);
    ~CMyString();

    size_t GetLength() const;
    const char* GetStringData() const;
    CMyString SubString(size_t start, size_t length = SIZE_MAX) const;
    void Clear();
    size_t GetCapacity() const;

    CMyString& operator=(CMyString const& other);
    CMyString& operator=(CMyString&& other) noexcept;
    CMyString& operator+=(CMyString const& other);

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    friend CMyString operator+(CMyString lhs, CMyString const& rhs);
    friend CMyString operator+(std::string const& lhs, CMyString const& rhs);
    friend CMyString operator+(const char* lhs, CMyString const& rhs);

    friend bool operator==(const CMyString& lhs, const CMyString& rhs);
    friend std::strong_ordering operator<=>(CMyString const& lhs, CMyString const& rhs);

    friend std::ostream& operator<<(std::ostream& stream, CMyString const& str);
    friend std::istream& operator>>(std::istream& stream, CMyString& str);

private:
    static char s_emptyBuffer[1];

    char* m_data;
    size_t m_length;
    size_t m_capacity;

    bool IsUsingStaticBuffer() const noexcept;
    void Reallocate(size_t newCapacity);
};


#endif //OOP_CSTRING_H
