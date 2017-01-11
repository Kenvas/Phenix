#pragma once

#include "../Version.hpp"

#ifndef KV_OS_WINDOWS_NAME
#   define KV_OS_WINDOWS_NAME "Microsoft Windows"
#   if  !defined(__KV_OS_DETECTED) && ( \
        defined(_WIN32) || defined(_WIN64) || \
        defined(__WIN32__) || defined(__TOS_WIN__) || \
        defined(__WINDOWS__) )
#       ifdef _WIN32_WINNT
#           define KV_OS_WINDOWS KV_MAKE_VERSION_FROM_0xVRR(_WIN32_WINNT)
#       else
#           define KV_OS_WINDOWS KV_VERSION_AVAILABLE
#       endif
#       define KV_OS_NAME    KV_OS_WINDOWS_NAME
#       define KV_OS_VERSION KV_OS_WINDOWS
#       include "_detected.hpp"
#   endif
#endif