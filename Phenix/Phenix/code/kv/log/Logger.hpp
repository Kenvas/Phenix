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
    using RefType = Logger &;

    color ForeColor;
    color BackColor;
    uint32_t Priority;

    BasicLogger() noexcept;
    BasicLogger(color fore_color, color back_color, uint32_t priority) noexcept;

    RefType EndLine   () const noexcept;
    RefType operator()() const noexcept;

    RefType WriteTime() const noexcept;

    RefType SetColor  (color const fore) const noexcept;
    RefType operator()(color const fore) const noexcept;
    RefType SetColor  (color const fore, color const back) const noexcept;
    RefType operator()(color const fore, color const back) const noexcept;

    RefType Write     (char         const * const content) const noexcept;
    RefType WriteLine (char         const * const content) const noexcept;
    RefType operator()(char         const * const content) const noexcept;
    RefType Write     (wchar_t      const * const content) const noexcept;
    RefType WriteLine (wchar_t      const * const content) const noexcept;
    RefType operator()(wchar_t      const * const content) const noexcept;
    RefType Write     (std::string  const &       content) const noexcept;
    RefType WriteLine (std::string  const &       content) const noexcept;
    RefType operator()(std::string  const &       content) const noexcept;
    RefType Write     (std::wstring const &       content) const noexcept;
    RefType WriteLine (std::wstring const &       content) const noexcept;
    RefType operator()(std::wstring const &       content) const noexcept;
};

struct Logger : public BasicLogger
{
    using BasicLogger::BasicLogger;
    using BasicLogger::operator();

    template <class _CharType, class ... _ArgsType>
    inline RefType Write(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<RefType>(BasicLogger::Write(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<RefType>(BasicLogger::Write(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline RefType WriteLine(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return (sizeof...(_ArgsType) > 0)
            ? static_cast<RefType>(BasicLogger::WriteLine(fmt::format(format, std::forward<_ArgsType>(args) ...)))
            : static_cast<RefType>(BasicLogger::WriteLine(format));
    }

    template <class _CharType, class ... _ArgsType>
    inline RefType operator()(_CharType const * const format, _ArgsType && ... args) const noexcept
    {
        return Write(format, std::forward<_ArgsType>(args) ...);
    }
};

#include "_namespace/end"