#pragma once

namespace kv
{
    template <class TargetType, class SourceType>
    constexpr auto force_cast(SourceType const & value) noexcept
    {
        return *reinterpret_cast<TargetType *>(&const_cast<SourceType &>(value));
    }
}
