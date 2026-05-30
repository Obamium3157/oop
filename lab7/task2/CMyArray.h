#ifndef OOP_CMYARRAY_H
#define OOP_CMYARRAY_H

#include <iterator>
#include <utility>

template <typename T>
class CMyArray
{
public:
    using Iterator = T*;
    using ConstIterator = const T*;
    using ReverseIterator = std::reverse_iterator<Iterator>;
    using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

    CMyArray() = default;

    CMyArray(const CMyArray& other)
        : m_buffer(other.m_size)
        , m_size(0)
    {
        CopyElementsFrom(other);
    }

    CMyArray(CMyArray&& other) noexcept
        : m_buffer(std::move(other.m_buffer))
        , m_size(std::exchange(other.m_size, 0))
    {
    }

    ~CMyArray() noexcept
    {
        DestroyElements();
    }

    CMyArray& operator=(const CMyArray& other)
    {
        if (this != &other)
        {
            CMyArray temp(other);
            Swap(temp);
        }

        return *this;
    }

    CMyArray& operator=(CMyArray&& other) noexcept
    {
        if (this != &other)
        {
            Swap(other);
        }

        return *this;
    }

    void PushBack(const T& value)
    {
        InsertAtEnd(value);
    }

    void PushBack(T&& value)
    {
        InsertAtEnd(std::move(value));
    }

    size_t GetSize() const noexcept
    {
        return m_size;
    }

    T& operator[](size_t index)
    {
        ThrowIfOutOfRange(index);
        return m_buffer.data[index];
    }

    const T& operator[](size_t index) const
    {
        ThrowIfOutOfRange(index);
        return m_buffer.data[index];
    }

    void Resize(size_t newSize)
    {
        if (newSize > m_size)
        {
            GrowTo(newSize);
        }
        else if (newSize < m_size)
        {
            ShrinkTo(newSize);
        }
    }

    void Clear() noexcept
    {
        DestroyElements();
        m_size = 0;
    }

    Iterator begin() noexcept { return m_buffer.data; }
    Iterator end() noexcept { return m_buffer.data + m_size; }
    ConstIterator begin() const noexcept { return m_buffer.data; }
    ConstIterator end() const noexcept { return m_buffer.data + m_size; }
    ConstIterator cbegin() const noexcept { return m_buffer.data; }
    ConstIterator cend() const noexcept { return m_buffer.data + m_size; }

    ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
    ReverseIterator rend() noexcept { return ReverseIterator(begin()); }
    ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(end()); }
    ConstReverseIterator rend() const noexcept { return ConstReverseIterator(begin()); }
    ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(cend()); }
    ConstReverseIterator crend() const noexcept { return ConstReverseIterator(cbegin()); }


private:
    struct RawBuffer
    {
        T* data = nullptr;
        size_t capacity = 0;

        RawBuffer() = default;

        explicit RawBuffer(const size_t requiredCapacity)
            : data(requiredCapacity > 0
                ? static_cast<T*>(::operator new(requiredCapacity * sizeof(T)))
                : nullptr)
            , capacity(requiredCapacity)
        {
        }

        RawBuffer(RawBuffer&& other) noexcept
            : data(std::exchange(other.data, nullptr))
            , capacity(std::exchange(other.capacity, 0))
        {
        }

        ~RawBuffer()
        {
            ::operator delete(data);
        }

        RawBuffer(const RawBuffer&) = delete;
        RawBuffer& operator=(const RawBuffer&) = delete;

        void Swap(RawBuffer& other) noexcept
        {
            std::swap(data, other.data);
            std::swap(capacity, other.capacity);
        }
    };

    template <typename TArg>
    void InsertAtEnd(TArg&& value)
    {
        if (m_size == m_buffer.capacity)
        {
            const size_t newCapacity = (m_buffer.capacity == 0)
                ? 1
                : m_buffer.capacity * 2;
            ReallocateBuffer(newCapacity);
        }

        new (m_buffer.data + m_size) T(std::forward<TArg>(value));
        ++m_size;
    }

    void GrowTo(size_t newSize)
    {
        if (newSize > m_buffer.capacity)
        {
            ReallocateBuffer(newSize);
        }

        while (m_size < newSize)
        {
            new (m_buffer.data + m_size) T();
            ++m_size;
        }
    }

    void ShrinkTo(const size_t newSize) noexcept
    {
        while (m_size > newSize)
        {
            m_buffer.data[--m_size].~T();
        }
    }

    void ReallocateBuffer(const size_t newCapacity)
    {
        RawBuffer newBuffer(newCapacity);
        size_t movedCount = 0;
        try
        {
            for (; movedCount < m_size; ++movedCount)
            {
                new(newBuffer.data + movedCount) T(
                    std::move_if_noexcept(m_buffer.data[movedCount]));
            }
        }
        catch (...)
        {
            DestroyRange(newBuffer.data, movedCount);
            throw;
        }
        DestroyElements();
        m_buffer.Swap(newBuffer);
    }

    void CopyElementsFrom(const CMyArray& other)
    {
        try
        {
            while (m_size < other.m_size)
            {
                new (m_buffer.data + m_size) T(other.m_buffer.data[m_size]);;
                ++m_size;
            }
        }
        catch (...)
        {
            DestroyElements();
            m_size = 0;
            throw;
        }
    }

    void DestroyElements() noexcept
    {
        DestroyRange(m_buffer.data, m_size);
    }

    static void DestroyRange(T* data, const size_t count) noexcept
    {
        for (size_t i = 0; i < count; ++i)
        {
            data[i].~T();
        }
    }

    void ThrowIfOutOfRange(const size_t index) const
    {
        if (index >= m_size)
        {
            throw std::out_of_range(
                "Index " + std::to_string(index)
                + " is out of range [0, " + std::to_string(m_size) + "]");
        }
    }

    void Swap(CMyArray& other) noexcept
    {
        m_buffer.Swap(other.m_buffer);
        std::swap(m_size, other.m_size);
    }


    RawBuffer m_buffer;
    size_t m_size = 0;
};


#endif //OOP_CMYARRAY_H
