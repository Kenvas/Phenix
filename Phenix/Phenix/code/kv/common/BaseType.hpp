#pragma

#include "_namespace/begin"

template <class T>
struct IntegerProxy final
{
    using ValueType = T;

    ValueType x;

    constexpr IntegerProxy() noexcept = default;
    constexpr IntegerProxy(IntegerProxy const &) noexcept = default;

    constexpr IntegerProxy(ValueType const value) noexcept
        : x(value)
    {}

    constexpr operator ValueType () noexcept { return x; }
    constexpr operator ValueType const () const noexcept { return x; }

    constexpr ValueType & operator=(ValueType const value) noexcept { x = value; return x; }
    constexpr ValueType const & operator=(ValueType const value) const noexcept { x = value; return x; }
};

#include "_namespace/end"