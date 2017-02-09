#pragma once

#include "clang.hpp"
#include "../Version.hpp"


#ifndef KV_COMPILER_MSVC_NAME
#   define KV_COMPILER_MSVC_NAME "Microsoft Visual C/C++"
#   define KV_COMPILER_MSVC2017 KV_MAKE_VERSION(15, 0, 0)
#   define KV_COMPILER_MSVC2015 KV_MAKE_VERSION(14, 0, 0)
// #   define KV_COMPILER_MSVC2013 KV_MAKE_VERSION(12, 0, 0)
// #   define KV_COMPILER_MSVC2012 KV_MAKE_VERSION(11, 0, 0)
// #   define KV_COMPILER_MSVC2010 KV_MAKE_VERSION(10, 0, 0)
// #   define KV_COMPILER_MSVC2008 KV_MAKE_VERSION( 9, 0, 0)
#   if defined(_MSC_VER) && !defined(KV_COMPILER_MSVC_VERSION)
#       if (_MSC_VER >= 1900) // MSVC2015
#           define KV_COMPILER_MSVC_VERSION KV_MAKE_VERSION(_MSC_VER/100-5, _MSC_VER%100, _MSC_BUILD)
#       else
#           define KV_COMPILER_MSVC_VERSION KV_MAKE_VERSION(_MSC_VER/100-6, _MSC_VER%100, _MSC_BUILD)
#       endif
#       ifdef __KV_COMPILER_DETECTED
#           define KV_COMPILER_MSVC_BACK    KV_COMPILER_MSVC_VERSION
#           define KV_COMPILER_NAME_BACK    KV_COMPILER_MSVC_NAME
#           define KV_COMPILER_VERSION_BACK KV_COMPILER_MSVC_BACK
#       else
#           define KV_COMPILER_MSVC    KV_COMPILER_MSVC_VERSION
#           define KV_COMPILER_NAME    KV_COMPILER_MSVC_NAME
#           define KV_COMPILER_VERSION KV_COMPILER_MSVC
#       endif
#       include "_detected.hpp"
#   endif
#endif
