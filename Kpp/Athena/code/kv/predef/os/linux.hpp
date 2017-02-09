#pragma once


#ifndef KV_OS_LINUX_NAME
#   define KV_OS_LINUX_NAME "Linux"
#   if  !defined(__KV_OS_DETECTED) && ( \
        defined(__linux__) || defined(__linux) || defined(linux) )
#       define KV_OS_LINUX   KV_VERSION_AVAILABLE
#       define KV_OS_NAME    KV_OS_LINUX_NAME
#       define KV_OS_VERSION KV_OS_LINUX
#       include "_detected.hpp"
#   endif
#endif