#pragma once

#include "x86_32.hpp"
#include "x86_64.hpp"

#if defined(KV_ARCH_X86_64)
#   define KV_ARCH_X86         KV_ARCH_X86_64
#   define KV_ARCH_X86_NAME    KV_ARCH_X86_64_NAME
#   define KV_ARCH_X86_VERSION KV_ARCH_X86
#elif defined(KV_ARCH_X86_32)
#   define KV_ARCH_X86         KV_ARCH_X86_32
#   define KV_ARCH_X86_NAME    KV_ARCH_X86_32_NAME
#   define KV_ARCH_X86_VERSION KV_ARCH_X86
#endif

#if !defined(KV_ARCH_NAME)
#   ifdef KV_ARCH_X86_NAME
#       define KV_ARCH_NAME    KV_ARCH_X86_NAME
#       define KV_ARCH_VERSION KV_ARCH_X86_VERSION
#   endif
#else
#   error "multiple architecture has dectected, is it possible?"
#endif