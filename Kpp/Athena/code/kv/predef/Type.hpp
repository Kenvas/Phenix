#pragma once

// #include <stdint.h>
// #include <float.h>

using char8_t   = char              ;

using int8_t    = signed char       ;
using int16_t   = signed short      ;
using int32_t   = signed int        ;
using int64_t   = signed long long  ;
using intmax_t  = signed long long  ;
using uint8_t   = unsigned char     ;
using uint16_t  = unsigned short    ;
using uint32_t  = unsigned int      ;
using uint64_t  = unsigned long long;
using uintmax_t = unsigned long long;

using float32_t = float             ;
using float64_t = double            ;

using size_t    = decltype(sizeof(char));
using ptrdiff_t = decltype(static_cast<char *>(0) - static_cast<char *>(0));

using intptr_t  = ptrdiff_t;
using uintptr_t = size_t;