/*
 Formatting library for C++ - time formatting

 Copyright (c) 2012 - 2016, Victor Zverovich
 All rights reserved.

 For the license information refer to format.h.
 */

#ifndef FMT_TIME_H_
#define FMT_TIME_H_

#include "format.h"
#include <ctime>

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4702)  // unreachable code
# pragma warning(disable: 4996)  // "deprecated" functions
#endif

namespace fmt {
template <typename ArgFormatter>
void format_arg(BasicFormatter<char, ArgFormatter> &f,
                const char *&format_str, const std::tm &tm) {
  if (*format_str == ':')
    ++format_str;
  const char *end = format_str;
  while (*end && *end != '}')
    ++end;
  if (*end != '}')
    FMT_THROW(FormatError("missing '}' in format string"));
  internal::MemoryBuffer<char, internal::INLINE_BUFFER_SIZE> format;
  format.append(format_str, end + 1);
  format[format.size() - 1] = '\0';
  Buffer<char> &buffer = f.writer().buffer();
  std::size_t start = buffer.size();
  for (;;) {
    std::size_t size = buffer.capacity() - start;
    std::size_t count = std::strftime(&buffer[start], size, &format[0], &tm);
    if (count != 0) {
      buffer.resize(start + count);
      break;
    }
    if (size >= format.size() * 256) {
      // If the buffer is 256 times larger than the format string, assume
      // that `strftime` gives an empty result. There doesn't seem to be a
      // better way to distinguish the two cases:
      // https://github.com/fmtlib/fmt/issues/367
      break;
    }
    const std::size_t MIN_GROWTH = 10;
    buffer.reserve(buffer.capacity() + (size > MIN_GROWTH ? size : MIN_GROWTH));
  }
  format_str = end + 1;
}


// Thread-safe replacement for std::localtime
std::tm localtime(std::time_t time);

// Thread-safe replacement for std::gmtime
std::tm gmtime(std::time_t time);
} //namespace fmt

#ifdef _MSC_VER
# pragma warning(pop)
#endif

#endif  // FMT_TIME_H_
