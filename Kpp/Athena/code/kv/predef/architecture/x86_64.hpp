#pragma once

#include "../Version.hpp"

#ifndef KV_ARCH_X86_64_NAME
#   define KV_ARCH_X86_64_NAME "Intel x86-64"
#   if !defined(KV_ARCH_X86_64_VERSION) && ( \
        defined(__x86_64) || defined(__x86_64__) || \
        defined(__amd64__) || defined(__amd64) || \
        defined(_M_X64) )
#      define KV_ARCH_X86_64_VERSION KV_MAKE_VERSION(1, 0, 0)
#   endif
#   ifdef KV_ARCH_X86_64_VERSION
#       define KV_ARCH_X86_64 KV_ARCH_X86_64_VERSION
#   endif
#endif