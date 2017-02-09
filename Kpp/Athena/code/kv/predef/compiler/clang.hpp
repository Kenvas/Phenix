#pragma once

#include "../Version.hpp"

#ifndef KV_COMPILER_CLANG_NAME
#   define KV_COMPILER_CLANG_NAME "Clang"
#   if defined(__clang__) && !defined(KV_COMPILER_CLANG_VERSION)
#      define KV_COMPILER_CLANG_VERSION KV_MAKE_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#      ifdef __KV_COMPILER_DETECTED
#          define KV_COMPILER_CLANG_BACK   KV_COMPILER_CLANG_VERSION
#          define KV_COMPILER_NAME_BACK    KV_COMPILER_CLANG_NAME
#          define KV_COMPILER_VERSION_BACK KV_COMPILER_CLANG_BACK
#      else
#          define KV_COMPILER_CLANG   KV_COMPILER_CLANG_VERSION
#          define KV_COMPILER_NAME    KV_COMPILER_CLANG_NAME
#          define KV_COMPILER_VERSION KV_COMPILER_CLANG
#      endif
#      include "_detected.hpp"
#   endif
#endif