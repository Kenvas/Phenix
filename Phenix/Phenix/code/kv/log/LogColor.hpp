#pragma once

#include "_namespace/begin"
namespace color
{
    enum color_enum
    {
        grey        = 0,

        blue        = 1,
        green       = 2,
        cyan        = blue  | green,
        red         = 4,
        yellow      = green | red,
        magenta     = blue  | red,
        white       = blue  | green | red,
        bold        = 8,
        BLUE        = bold | blue,
        GREEN       = bold | green,
        CYAN        = bold | cyan,
        RED         = bold | red,
        YELLOW      = bold | yellow,
        MAGENTA     = bold | magenta,
        WHITE       = bold | white,

        on_blue     = blue    << 4,
        on_green    = green   << 4,
        on_cyan     = cyan    << 4,
        on_red      = red     << 4,
        on_yellow   = yellow  << 4,
        on_magenta  = magenta << 4,
        on_white    = white   << 4,
        on_bold     = bold    << 4,
        ON_BLUE     = on_bold | on_blue,
        ON_GREEN    = on_bold | on_green,
        ON_CYAN     = on_bold | on_cyan,
        ON_RED      = on_bold | on_red ,
        ON_YELLOW   = on_bold | on_yellow,
        ON_MAGENTA  = on_bold | on_magenta,
        ON_WHITE    = on_bold | on_white,

    };
}
#include "_namespace/end"