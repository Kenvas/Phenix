#include "PrecompiledHeader.hpp"

#include "../Logger.hpp"
#include "../LogColor.hpp"
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
Logger const info    {};
Logger const warn    {};
Logger const error   {};
Logger const critical{};

BasicLogger::ConstLoggerRef BasicLogger::EndLine() const noexcept
{
    std::cout << termcolor::reset << std::endl;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()() const noexcept
{
    return EndLine();
}

BasicLogger::ConstLoggerRef BasicLogger::WriteTime() const noexcept
{
    using namespace std::chrono;
    autox dt = high_resolution_clock::now().time_since_epoch();
    autox us = (duration_cast<microseconds>(dt).count() % microseconds::period::den);
    autox t  = std::time(nullptr);
    autox tm = std::localtime(&t);
    std::cout << fmt::format("{0:%T}.{1:06d} ", *tm, us);
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::SetColor(color const fore) const noexcept
{
    if (fore == color::reset)
    {
        std::cout << termcolor::reset;
        return static_cast<ConstLoggerRef>(*this);
    }
    return SetColor(fore, color::grey);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(color const fore) const noexcept
{
    return SetColor(fore);
}

BasicLogger::ConstLoggerRef BasicLogger::SetColor(color const fore, color const back) const noexcept
{
    cout << (((int(fore) & 0x8) != 0) ? termcolor::boldfore : termcolor::darkfore);
    switch (int(fore) & 0x7)
    {
    default                 :                             break;
    case int(color::grey   ): cout << termcolor::grey   ; break;
    case int(color::blue   ): cout << termcolor::blue   ; break;
    case int(color::green  ): cout << termcolor::green  ; break;
    case int(color::cyan   ): cout << termcolor::cyan   ; break;
    case int(color::red    ): cout << termcolor::red    ; break;
    case int(color::magenta): cout << termcolor::magenta; break;
    case int(color::yellow ): cout << termcolor::yellow ; break;
    case int(color::white  ): cout << termcolor::white  ; break;
    }

    cout << (((int(back) & 0x8) != 0) ? termcolor::boldback : termcolor::darkback);
    switch (int(back) & 0x7)
    {
    default                 :                                break;
    case int(color::grey   ): cout << termcolor::on_grey   ; break;
    case int(color::blue   ): cout << termcolor::on_blue   ; break;
    case int(color::green  ): cout << termcolor::on_green  ; break;
    case int(color::cyan   ): cout << termcolor::on_cyan   ; break;
    case int(color::red    ): cout << termcolor::on_red    ; break;
    case int(color::magenta): cout << termcolor::on_magenta; break;
    case int(color::yellow ): cout << termcolor::on_yellow ; break;
    case int(color::white  ): cout << termcolor::on_white  ; break;
    }

    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(color const fore, color const back) const noexcept
{
    return SetColor(fore, back);
}

BasicLogger::ConstLoggerRef BasicLogger::Write(char const * const content) const noexcept
{
    std::cout << content;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::WriteLine(char const * const content) const noexcept
{
    return Write(content).EndLine();
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(char const * const content) const noexcept
{
    return Write(content);
}

BasicLogger::ConstLoggerRef BasicLogger::Write(wchar_t const * const content) const noexcept
{
    std::wcout << content;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::WriteLine(wchar_t const * const content) const noexcept
{
    return Write(content).EndLine();
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(wchar_t const * const content) const noexcept
{
    return Write(content);
}

BasicLogger::ConstLoggerRef BasicLogger::Write(std::string const & content) const noexcept
{
    return Write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::WriteLine(std::string const & content) const noexcept
{
    return WriteLine(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(std::string const & content) const noexcept
{
    return Write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::Write(std::wstring const & content) const noexcept
{
    return Write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::WriteLine(std::wstring const & content) const noexcept
{
    return WriteLine(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(std::wstring const & content) const noexcept
{
    return Write(content.c_str());
}

#include "../_namespace/end"