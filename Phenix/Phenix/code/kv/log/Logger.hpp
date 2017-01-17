#pragma once

#include "kv/predef/Type.hpp"
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

    uint16_t fore_color;
    uint16_t back_color;
    uint32_t level;

    ConstLoggerRef endln     () const noexcept;
    ConstLoggerRef operator()() const noexcept;

    ConstLoggerRef time() const noexcept;

    ConstLoggerRef color     (uint16_t const fore) const noexcept;
    ConstLoggerRef operator()(uint16_t const fore) const noexcept;
    ConstLoggerRef color     (uint16_t const fore, uint16_t const back) const noexcept;
    ConstLoggerRef operator()(uint16_t const fore, uint16_t const back) const noexcept;

    ConstLoggerRef write     (char const * const content   ) const noexcept;
    ConstLoggerRef writeln   (char const * const content   ) const noexcept;
    ConstLoggerRef operator()(char const * const content   ) const noexcept;
    ConstLoggerRef write     (wchar_t const * const content) const noexcept;
    ConstLoggerRef writeln   (wchar_t const * const content) const noexcept;
    ConstLoggerRef operator()(wchar_t const * const content) const noexcept;
    ConstLoggerRef write     (std::string const & content  ) const noexcept;
    ConstLoggerRef writeln   (std::string const & content  ) const noexcept;
    ConstLoggerRef operator()(std::string const & content  ) const noexcept;
    ConstLoggerRef write     (std::wstring const & content ) const noexcept;
    ConstLoggerRef writeln   (std::wstring const & content ) const noexcept;
    ConstLoggerRef operator()(std::wstring const & content ) const noexcept;
};

struct Logger : public BasicLogger
{
    using BasicLogger::BasicLogger;
    using BasicLogger::operator();

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef write(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<Logger const &>(BasicLogger::write(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<Logger const &>(BasicLogger::write(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef writeln(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<Logger const &>(BasicLogger::writeln(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<Logger const &>(BasicLogger::writeln(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline ConstLoggerRef operator()(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return write(format, std::forward<_ArgsType>(args) ...);
    }
};


#include "_namespace/end"