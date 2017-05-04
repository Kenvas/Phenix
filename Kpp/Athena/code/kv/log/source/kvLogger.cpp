#include "PrecompiledHeader.hpp"

#include "../IncludeAll.hpp"
#include "kv/predef/Keyword.hpp"

#include "termcolor/termcolor.hpp"
#include "fmt/format.h"
#include "fmt/time.h"

#include <ctime>
#include <chrono>
#include <iostream>

#include "../_namespace/begin"

Logger const trace   {};
Logger const debug   {};
Logger const info    { color::white , color::grey, 3 };
Logger const warn    { color::yellow, color::grey, 4 };
Logger const error   { color::red   , color::grey, 5 };
Logger const critical{ color::white , color::red , 6 };
TimeTag time;

inline BasicLogger::RefType const logger_cast(BasicLogger const * const value) noexcept
{
    return static_cast<BasicLogger::RefType>(const_cast<BasicLogger &>(*value));
}

BasicLogger::BasicLogger() noexcept
    : BasicLogger(color::white, color::grey, 0)
{}

BasicLogger::BasicLogger(
    color    fore_color,
    color    back_color,
    uint32_t priority
    ) noexcept
    : ForeColor(fore_color)
    , BackColor(back_color)
    , Priority(priority)
{}

BasicLogger::RefType BasicLogger::EndLine() const noexcept
{
    std::cout << std::endl;
    return SetColor(color::reset);
}

BasicLogger::RefType BasicLogger::operator()() const noexcept
{
    return EndLine();
}

BasicLogger::RefType BasicLogger::WriteTime() const noexcept
{
    using namespace std::chrono;
    autox dt = high_resolution_clock::now().time_since_epoch();
    autox us = (duration_cast<microseconds>(dt).count() % microseconds::period::den);
    autox tm = std::time(nullptr);
    autox lt = std::localtime(&tm);
    std::cout << fmt::format("{0:%T}.{1:06d} ", *lt, us);
    return logger_cast(this);
}

BasicLogger::RefType BasicLogger::operator()(TimeTag unused) const noexcept
{
    return WriteTime();
}

BasicLogger::RefType BasicLogger::SetColor(color const fore) const noexcept
{
    if (fore == color::reset)
    {
        std::cout << termcolor::reset;
        return logger_cast(this);
    }
    return SetColor(fore, color::grey);
}

BasicLogger::RefType BasicLogger::operator()(color const fore) const noexcept
{
    return SetColor(fore);
}

BasicLogger::RefType BasicLogger::SetColor(color const fore, color const back) const noexcept
{
    std::cout << (((int(fore) & 0x8) != 0) ? termcolor::boldfore : termcolor::darkfore);
    switch (int(fore) & 0x7)
    {
    default                 :                                  break;
    case int(color::grey   ): std::cout << termcolor::grey   ; break;
    case int(color::blue   ): std::cout << termcolor::blue   ; break;
    case int(color::green  ): std::cout << termcolor::green  ; break;
    case int(color::cyan   ): std::cout << termcolor::cyan   ; break;
    case int(color::red    ): std::cout << termcolor::red    ; break;
    case int(color::magenta): std::cout << termcolor::magenta; break;
    case int(color::yellow ): std::cout << termcolor::yellow ; break;
    case int(color::white  ): std::cout << termcolor::white  ; break;
    }

    std::cout << (((int(back) & 0x8) != 0) ? termcolor::boldback : termcolor::darkback);
    switch (int(back) & 0x7)
    {
    default                 :                                     break;
    case int(color::grey   ): std::cout << termcolor::on_grey   ; break;
    case int(color::blue   ): std::cout << termcolor::on_blue   ; break;
    case int(color::green  ): std::cout << termcolor::on_green  ; break;
    case int(color::cyan   ): std::cout << termcolor::on_cyan   ; break;
    case int(color::red    ): std::cout << termcolor::on_red    ; break;
    case int(color::magenta): std::cout << termcolor::on_magenta; break;
    case int(color::yellow ): std::cout << termcolor::on_yellow ; break;
    case int(color::white  ): std::cout << termcolor::on_white  ; break;
    }

    return logger_cast(this);
}

BasicLogger::RefType BasicLogger::operator()(color const fore, color const back) const noexcept
{
    return SetColor(fore, back);
}

BasicLogger::RefType BasicLogger::Write(char const * const content) const noexcept
{
    std::cout << content;
    return logger_cast(this);
}

BasicLogger::RefType BasicLogger::WriteLine(char const * const content) const noexcept
{
    return Write(content).EndLine();
}

BasicLogger::RefType BasicLogger::operator()(char const * const content) const noexcept
{
    return Write(content);
}

BasicLogger::RefType BasicLogger::Write(wchar_t const * const content) const noexcept
{
    std::wcout << content;
    return logger_cast(this);
}

BasicLogger::RefType BasicLogger::WriteLine(wchar_t const * const content) const noexcept
{
    return Write(content).EndLine();
}

BasicLogger::RefType BasicLogger::operator()(wchar_t const * const content) const noexcept
{
    return Write(content);
}

BasicLogger::RefType BasicLogger::Write(std::string const & content) const noexcept
{
    return Write(content.data());
}

BasicLogger::RefType BasicLogger::WriteLine(std::string const & content) const noexcept
{
    return WriteLine(content.data());
}

BasicLogger::RefType BasicLogger::operator()(std::string const & content) const noexcept
{
    return Write(content.data());
}

BasicLogger::RefType BasicLogger::Write(std::wstring const & content) const noexcept
{
    return Write(content.data());
}

BasicLogger::RefType BasicLogger::WriteLine(std::wstring const & content) const noexcept
{
    return WriteLine(content.data());
}

BasicLogger::RefType BasicLogger::operator()(std::wstring const & content) const noexcept
{
    return Write(content.data());
}

#include "../_namespace/end"