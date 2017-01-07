#pragma once

namespace kv
{
    struct Size final
    {
    public:
        int Width;
        int Height;
    public:
        constexpr Size(int width, int height) noexcept
            : Width(width)
            , Height(height)
        {}
    };
}
