#pragma once

#include <iterator>

namespace kv
{
    template <class T>
    class ArrayView
    {
    public:
        using value_type      = T;
        using pointer         = value_type *;
        using const_pointer   = value_type const *;
        using iterator        = value_type *;
        using const_iterator  = value_type const *;
        using reference       = value_type &;
        using const_reference = value_type const &;
        using size_type       = size_t;
        using difference_type = ptrdiff_t;

    private:
        iterator  data_;
        size_type length_;

    public:

        template <int nLength>
        constexpr ArrayView(value_type const (&data)[nLength]) noexcept
            : data_(const_cast<value_type (&)[nLength]>(data)), length_(nLength) { }

        constexpr ArrayView(pointer const data, size_type const length) noexcept
            : data_(data), length_(length) { }

        constexpr const_iterator const data() const noexcept
        {
            return data_;
        }

        constexpr size_type const length() const noexcept
        {
            return length_;
        }

        constexpr const_iterator const begin() const noexcept
        {
            return data();
        }

        constexpr const_iterator const end() const noexcept
        {
            return data() + length();
        }

    };
}