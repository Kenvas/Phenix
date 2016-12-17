#pragma once

namespace fnx
{
    template <class TargetType, class SourceType>
    constexpr auto force_cast(SourceType const & value) noexcept
    {
        return *reinterpret_cast<TargetType *>(&value);
    }

    bool const TryParseInt(char const * const input, int * value) noexcept
    {
        if (value == nullptr) return false;
        auto p = input;
        auto v = 0;
        auto s = (*p == '-');
        p += (*p == '+') ? 1 : 0;
        p += (*p == '-') ? 1 : 0;
        while (*p != '\0')
        {
            auto c = *p;
            if (c < '0' || c > '9') return false;
            v *= 10;
            v += int(c - '0');
            ++p;
        }
        *value = s ? -v : v;
        return true;
    }
}
