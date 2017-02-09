#pragma once


#ifndef KV_OS_IOS_NAME
#   define KV_OS_IOS_NAME "iOS"
#   if  !defined(__KV_OS_DETECTED) && ( \
        defined(__APPLE__) && defined(__MACH__) && \
        defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) )
#       define KV_OS_IOS     (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__*1000)
#       define KV_OS_NAME    KV_OS_IOS_NAME
#       define KV_OS_VERSION KV_OS_IOS
#       include "_detected.hpp"
#   endif
#endif