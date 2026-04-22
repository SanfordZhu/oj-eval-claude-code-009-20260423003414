#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
template<typename T>
class vector
{
public:
    class const_iterator;
    class iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

    private:
        T* ptr;
        const vector* vec;

    public:
        iterator(T* p = nullptr, const vector* v = nullptr) : ptr(p), vec(v) {}

        iterator operator+(const int &n) const
        {
            return iterator(ptr + n, vec);
        }
        iterator operator-(const int &n) const
        {
            return iterator(ptr - n, vec);
        }
        int operator-(const iterator &rhs) const
        {
            if (vec != rhs.vec) {
                throw invalid_iterator();
            }
            return ptr - rhs.ptr;
        }
        iterator& operator+=(const int &n)
        {
            ptr += n;
            return *this;
        }
        iterator& operator-=(const int &n)
        {
            ptr -= n;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        iterator& operator++()
        {
            ++ptr;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp = *this;
            --ptr;
            return tmp;
        }
        iterator& operator--()
        {
            --ptr;
            return *this;
        }
        T& operator*() const
        {
            return *ptr;
        }
        bool operator==(const iterator &rhs) const
        {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const
        {
            return ptr == rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const
        {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const
        {
            return ptr != rhs.ptr;
        }
    };

    class const_iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

    private:
        const T* ptr;
        const vector* vec;

    public:
        const_iterator(const T* p = nullptr, const vector* v = nullptr) : ptr(p), vec(v) {}

        const_iterator(const iterator& it) : ptr(it.ptr), vec(it.vec) {}

        const_iterator operator+(const int &n) const
        {
            return const_iterator(ptr + n, vec);
        }
        const_iterator operator-(const int &n) const
        {
            return const_iterator(ptr - n, vec);
        }
        int operator-(const const_iterator &rhs) const
        {
            if (vec != rhs.vec) {
                throw invalid_iterator();
            }
            return ptr - rhs.ptr;
        }
        const_iterator& operator+=(const int &n)
        {
            ptr += n;
            return *this;
        }
        const_iterator& operator-=(const int &n)
        {
            ptr -= n;
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++ptr;
            return tmp;
        }
        const_iterator& operator++()
        {
            ++ptr;
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator tmp = *this;
            --ptr;
            return tmp;
        }
        const_iterator& operator--()
        {
            --ptr;
            return *this;
        }
        const T& operator*() const
        {
            return *ptr;
        }
        bool operator==(const iterator &rhs) const
        {
            return ptr == rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const
        {
            return ptr == rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const
        {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const
        {
            return ptr != rhs.ptr;
        }
    };

private:
    T* data;
    size_t len;
    size_t cap;

    void expand()
    {
        size_t new_cap = cap == 0 ? 1 : cap * 2;
        T* new_data = (T*)::operator new(new_cap * sizeof(T));
        for (size_t i = 0; i < len; ++i) {
            new(new_data + i) T(data[i]);
            data[i].~T();
        }
        ::operator delete(data);
        data = new_data;
        cap = new_cap;
    }

public:
    vector() : data(nullptr), len(0), cap(0) {}

    vector(const vector &other) : data(nullptr), len(0), cap(0)
    {
        if (other.len > 0) {
            data = (T*)::operator new(other.len * sizeof(T));
            cap = other.len;
            for (size_t i = 0; i < other.len; ++i) {
                new(data + i) T(other.data[i]);
            }
            len = other.len;
        }
    }

    ~vector()
    {
        for (size_t i = 0; i < len; ++i) {
            data[i].~T();
        }
        ::operator delete(data);
    }

    vector &operator=(const vector &other)
    {
        if (this == &other) {
            return *this;
        }
        for (size_t i = 0; i < len; ++i) {
            data[i].~T();
        }
        ::operator delete(data);
        len = 0;
        cap = 0;
        data = nullptr;
        if (other.len > 0) {
            data = (T*)::operator new(other.len * sizeof(T));
            cap = other.len;
            for (size_t i = 0; i < other.len; ++i) {
                new(data + i) T(other.data[i]);
            }
            len = other.len;
        }
        return *this;
    }

    T & at(const size_t &pos)
    {
        if (pos >= len) {
            throw index_out_of_bound();
        }
        return data[pos];
    }

    const T & at(const size_t &pos) const
    {
        if (pos >= len) {
            throw index_out_of_bound();
        }
        return data[pos];
    }

    T & operator[](const size_t &pos)
    {
        if (pos >= len) {
            throw index_out_of_bound();
        }
        return data[pos];
    }

    const T & operator[](const size_t &pos) const
    {
        if (pos >= len) {
            throw index_out_of_bound();
        }
        return data[pos];
    }

    const T & front() const
    {
        if (len == 0) {
            throw container_is_empty();
        }
        return data[0];
    }

    const T & back() const
    {
        if (len == 0) {
            throw container_is_empty();
        }
        return data[len - 1];
    }

    iterator begin()
    {
        return iterator(data, this);
    }

    const_iterator begin() const
    {
        return const_iterator(data, this);
    }

    const_iterator cbegin() const
    {
        return const_iterator(data, this);
    }

    iterator end()
    {
        return iterator(data + len, this);
    }

    const_iterator end() const
    {
        return const_iterator(data + len, this);
    }

    const_iterator cend() const
    {
        return const_iterator(data + len, this);
    }

    bool empty() const
    {
        return len == 0;
    }

    size_t size() const
    {
        return len;
    }

    void clear()
    {
        for (size_t i = 0; i < len; ++i) {
            data[i].~T();
        }
        len = 0;
    }

    iterator insert(iterator pos, const T &value)
    {
        size_t idx = pos - begin();
        if (idx > len) {
            throw index_out_of_bound();
        }
        if (len == cap) {
            expand();
        }
        for (size_t i = len; i > idx; --i) {
            new(data + i) T(data[i - 1]);
            data[i - 1].~T();
        }
        new(data + idx) T(value);
        ++len;
        return iterator(data + idx, this);
    }

    iterator insert(const size_t &ind, const T &value)
    {
        if (ind > len) {
            throw index_out_of_bound();
        }
        if (len == cap) {
            expand();
        }
        for (size_t i = len; i > ind; --i) {
            new(data + i) T(data[i - 1]);
            data[i - 1].~T();
        }
        new(data + ind) T(value);
        ++len;
        return iterator(data + ind, this);
    }

    iterator erase(iterator pos)
    {
        size_t idx = pos - begin();
        if (idx >= len) {
            throw index_out_of_bound();
        }
        data[idx].~T();
        for (size_t i = idx; i < len - 1; ++i) {
            new(data + i) T(data[i + 1]);
            data[i + 1].~T();
        }
        --len;
        return iterator(data + idx, this);
    }

    iterator erase(const size_t &ind)
    {
        if (ind >= len) {
            throw index_out_of_bound();
        }
        data[ind].~T();
        for (size_t i = ind; i < len - 1; ++i) {
            new(data + i) T(data[i + 1]);
            data[i + 1].~T();
        }
        --len;
        return iterator(data + ind, this);
    }

    void push_back(const T &value)
    {
        if (len == cap) {
            expand();
        }
        new(data + len) T(value);
        ++len;
    }

    void pop_back()
    {
        if (len == 0) {
            throw container_is_empty();
        }
        --len;
        data[len].~T();
    }
};

}

#endif
