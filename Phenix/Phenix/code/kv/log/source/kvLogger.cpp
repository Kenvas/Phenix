#include "PrecompiledHeader.hpp"

#include "../Logger.hpp"
#include "../LogWrap.hpp"
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

BasicLogger::ConstLoggerRef BasicLogger::endln() const noexcept
{
    std::cout << termcolor::reset << std::endl;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()() const noexcept
{
    return endln();
}

BasicLogger::ConstLoggerRef BasicLogger::time() const noexcept
{
    using namespace std::chrono;
    autox dt = high_resolution_clock::now().time_since_epoch();
    autox us = (duration_cast<microseconds>(dt).count() % microseconds::period::den);
    autox t  = std::time(nullptr);
    autox tm = std::localtime(&t);
    std::cout << fmt::format("{0:%T}.{1:06d} ", *tm, us);
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::color(uint16_t const fore) const noexcept
{
    return color(fore, -1);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(uint16_t const fore) const noexcept
{
    return color(fore, -1);
}

BasicLogger::ConstLoggerRef BasicLogger::color(uint16_t const fore, uint16_t const back) const noexcept
{
    switch (fore)
    {
    case log::color::grey   : cout << termcolor::grey   ; break;
    case log::color::blue   : cout << termcolor::blue   ; break;
    case log::color::green  : cout << termcolor::green  ; break;
    case log::color::cyan   : cout << termcolor::cyan   ; break;
    case log::color::red    : cout << termcolor::red    ; break;
    case log::color::magenta: cout << termcolor::magenta; break;
    case log::color::yellow : cout << termcolor::yellow ; break;
    case log::color::white  : cout << termcolor::white  ; break;
    }
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(uint16_t const fore, uint16_t const back) const noexcept
{
    return color(fore, back);
}

BasicLogger::ConstLoggerRef BasicLogger::write(char const * const content) const noexcept
{
    std::cout << content;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::writeln(char const * const content) const noexcept
{
    return write(content).endln();
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(char const * const content) const noexcept
{
    return write(content);
}

BasicLogger::ConstLoggerRef BasicLogger::write(wchar_t const * const content) const noexcept
{
    std::wcout << content;
    return static_cast<ConstLoggerRef>(*this);
}

BasicLogger::ConstLoggerRef BasicLogger::writeln(wchar_t const * const content) const noexcept
{
    return write(content).endln();
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(wchar_t const * const content) const noexcept
{
    return write(content);
}

BasicLogger::ConstLoggerRef BasicLogger::write(std::string const & content) const noexcept
{
    return write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::writeln(std::string const & content) const noexcept
{
    return writeln(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(std::string const & content) const noexcept
{
    return write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::write(std::wstring const & content) const noexcept
{
    return write(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::writeln(std::wstring const & content) const noexcept
{
    return writeln(content.c_str());
}

BasicLogger::ConstLoggerRef BasicLogger::operator()(std::wstring const & content) const noexcept
{
    return write(content.c_str());
}

#include "../_namespace/end"