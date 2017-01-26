#pragma once

#include "kv/predef/Type.hpp"
#include "LogColor.hpp"
#include <iostream>

#include "_namespace/begin"

struct BasicLogger;
struct Logger;

extern Logger const trace;
extern Logger const debug;
extern Logger const info;
extern Logger const warn;
extern Logger const error;
extern Logger const critical;

struct BasicLogger
{
public:
    using ConstLoggerRef = Logger const &;

    color fore_color;
    color back_color;
    uint32_t level;

    ConstLoggerRef EndLine   () const noexcept;
    ConstLoggerRef operator()() const noexcept;

    ConstLoggerRef WriteTime() const noexcept;

    ConstLoggerRef SetColor  (color const fore) const noexcept;
    ConstLoggerRef operator()(color const fore) const noexcept;
    ConstLoggerRef SetColor  (color const fore, color const back) const noexcept;
    ConstLoggerRef operator()(color const fore, color const back) const noexcept;

    ConstLoggerRef Write     (char         const * const content) const noexcept;
    ConstLoggerRef WriteLine (char         const * const content) const noexcept;
    ConstLoggerRef operator()(char         const * const content) const noexcept;
    ConstLoggerRef Write     (wchar_t      const * const content) const noexcept;
    ConstLoggerRef WriteLine (wchar_t      const * const content) const noexcept;
    ConstLoggerRef operator()(wchar_t      const * const content) const noexcept;
    ConstLoggerRef Write     (std::string  const &       content) const noexcept;
    ConstLoggerRef WriteLine (std::string  const &       content) const noexcept;
    ConstLoggerRef operator()(std::string  const &       content) const noexcept;
    ConstLoggerRef Write     (std::wstring const &       content) const noexcept;
    ConstLoggerRef WriteLine (std::wstring const &       content) const noexcept;
    ConstLoggerRef operator()(std::wstring const &       content) const noexcept;
};

struct Logger : public BasicLogger
{
    using BasicLogger::BasicLogger;
    using BasicLogger::operator();

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef Write(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<Logger const &>(BasicLogger::Write(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<Logger const &>(BasicLogger::Write(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef WriteLine(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<Logger const &>(BasicLogger::WriteLine(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<Logger const &>(BasicLogger::WriteLine(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef operator()(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return Write(format, std::forward<_ArgsType>(args) ...);
    }
};

#include "_namespace/end"