#pragma once


#ifndef KV_OS_ANDROID_NAME
#   define KV_OS_ANDROID_NAME "Android"
#   if  !defined(__KV_OS_DETECTED) && ( \
        defined(__ANDROID__) )
#       if   (__ANDROID_API__  == 25)
#           define KV_OS_ANDROID KV_MAKE_VERSION(7, 1, 0)
#       elif (__ANDROID_API__  == 24)
#           define KV_OS_ANDROID KV_MAKE_VERSION(7, 0, 0)
#       elif (__ANDROID_API__  == 23)
#           define KV_OS_ANDROID KV_MAKE_VERSION(6, 0, 0)
#       elif (__ANDROID_API__  == 22)
#           define KV_OS_ANDROID KV_MAKE_VERSION(5, 1, 0)
#       elif (__ANDROID_API__  == 21)
#           define KV_OS_ANDROID KV_MAKE_VERSION(5, 0, 0)
#       elif (__ANDROID_API__  == 19)
#           define KV_OS_ANDROID KV_MAKE_VERSION(4, 4, 0)
#       elif (__ANDROID_API__  == 18)
#           define KV_OS_ANDROID KV_MAKE_VERSION(4, 3, 0)
#       elif (__ANDROID_API__  == 17)
#           define KV_OS_ANDROID KV_MAKE_VERSION(4, 2, 0)
#       elif (__ANDROID_API__  == 16)
#           define KV_OS_ANDROID KV_MAKE_VERSION(4, 1, 0)
#       elif (__ANDROID_API__  == 15)
#           define KV_OS_ANDROID KV_MAKE_VERSION(4, 0, 0)
#       elif (__ANDROID_API__  == 10)
#           define KV_OS_ANDROID KV_MAKE_VERSION(2, 3, 3)
#       elif (__ANDROID_API__  ==  9)
#           define KV_OS_ANDROID KV_MAKE_VERSION(2, 3, 0)
#       elif (__ANDROID_API__  ==  8)
#           define KV_OS_ANDROID KV_MAKE_VERSION(2, 2, 0)
#       else
            error "KV_OS_ANDROID not handled"
#       endif
#       define KV_OS_NAME    KV_OS_ANDROID_NAME
#       define KV_OS_VERSION KV_OS_ANDROID
#       include "_detected.hpp"
#   endif
#endif