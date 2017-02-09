#pragma once

#include "clang.hpp"
#include "../Version.hpp"

#ifndef KV_COMPILER_GCC_NAME
#   define KV_COMPILER_GCC_NAME "GCC C/C++"
#   if defined(__GNUC__) && !defined(KV_COMPILER_GCC_VERSION)
#      ifdef __GNUC_PATCHLEVEL__
#          define KV_COMPILER_GCC_VERSION KV_MAKE_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#      else
#          define KV_COMPILER_GCC_VERSION KV_MAKE_VERSION(__GNUC__, __GNUC_MINOR__, 0)
#      endif
#      ifdef __KV_COMPILER_DETECTED
#          define KV_COMPILER_GCC_BACK     KV_COMPILER_GCC_VERSION
#          define KV_COMPILER_NAME_BACK    KV_COMPILER_GCC_NAME
#          define KV_COMPILER_VERSION_BACK KV_COMPILER_GCC_BACK
#      else
#          define KV_COMPILER_GCC     KV_COMPILER_GCC_VERSION
#          define KV_COMPILER_NAME    KV_COMPILER_GCC_NAME
#          define KV_COMPILER_VERSION KV_COMPILER_GCC
#      endif
#      include "_detected.hpp"
#   endif
#endif