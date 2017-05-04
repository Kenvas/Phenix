#pragma once

#include "_namespace/begin"
enum class color
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

    reset       = -1,
};
#include "_namespace/end"