#pragma once

#include "../Version.hpp"

#ifndef KV_ARCH_X86_32_NAME
#   define KV_ARCH_X86_32_NAME "Intel x86-32"
#   if !defined(KV_ARCH_X86_32_VERSION) && defined(__I86__)
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION(__I86__,0,0)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && defined(_M_IX86)
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION_FROM_VV00(_M_IX86)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && defined(__i686__)
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION(6,0,0)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && defined(__i586__)
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION(5,0,0)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && defined(__i486__)
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION(4,0,0)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && (defined(__i386__) || defined(i386__) || defined(i386))
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION(3,0,0)
#   endif
#   if !defined(KV_ARCH_X86_32_VERSION) && ( \
        defined(_X86_) || \
        defined(__INTEL__) || defined(__THW_INTEL__) )
#       define KV_ARCH_X86_32_VERSION KV_MAKE_VERSION_AVAILABLE
#   endif
#   ifdef KV_ARCH_X86_32_VERSION
#       define KV_ARCH_X86_32 KV_ARCH_X86_32_VERSION
#   endif
#endif