#pragma once

namespace fnx
{
    template <class TargetType, class SourceType>
    constexpr auto force_cast(SourceType const & value) noexcept
    {
        return *reinterpret_cast<TargetType *>(&value);
    }
}
