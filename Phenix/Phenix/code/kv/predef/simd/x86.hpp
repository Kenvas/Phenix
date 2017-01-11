#pragma once

#include "../Version.hpp"

#define KV_SIMD_X86_MMX       KV_MAKE_VERSION(0, 9, 0)
#define KV_SIMD_X86_SSE       KV_MAKE_VERSION(1, 0, 0)
#define KV_SIMD_X86_SSE2      KV_MAKE_VERSION(2, 0, 0)
#define KV_SIMD_X86_SSE3      KV_MAKE_VERSION(3, 0, 0)
#define KV_SIMD_X86_SSSE3     KV_MAKE_VERSION(3, 1, 0)
#define KV_SIMD_X86_SSE4_1    KV_MAKE_VERSION(4, 1, 0)
#define KV_SIMD_X86_SSE4_2    KV_MAKE_VERSION(4, 2, 0)
#define KV_SIMD_X86_AVX       KV_MAKE_VERSION(5, 0, 0)
#define KV_SIMD_X86_FMA3      KV_MAKE_VERSION(5, 2, 0)
#define KV_SIMD_X86_AVX2      KV_MAKE_VERSION(5, 3, 0)
#define KV_SIMD_X86_MIC       KV_MAKE_VERSION(9, 0, 0)

#define KV_SIMD_X86_AMD_SSE4A KV_MAKE_VERSION(4, 0, 0)
#define KV_SIMD_X86_AMD_FMA4  KV_MAKE_VERSION(5, 1, 0)
#define KV_SIMD_X86_AMD_XOP   KV_MAKE_VERSION(5, 1, 1)


#ifndef KV_SIMD_X86_NAME
#   define KV_SIMD_X86_NAME "SIMD x86"
#   if !defined(KV_SIMD_X86) && defined(__MIC__)
#      define KV_SIMD_X86 KV_SIMD_X86_MIC
#   endif
#   if !defined(KV_SIMD_X86) && defined(__AVX2__)
#      define KV_SIMD_X86 KV_SIMD_X86_AVX2
#   endif
#   if !defined(KV_SIMD_X86) && defined(__AVX__)
#      define KV_SIMD_X86 KV_SIMD_X86_AVX
#   endif
#   if !defined(KV_SIMD_X86) && defined(__FMA__)
#      define KV_SIMD_X86 KV_SIMD_X86_FMA
#   endif
#   if !defined(KV_SIMD_X86) && defined(__SSE4_2__)
#      define KV_SIMD_X86 KV_SIMD_X86_SSE4_2
#   endif
#   if !defined(KV_SIMD_X86) && defined(__SSE4_1__)
#      define KV_SIMD_X86 KV_SIMD_X86_SSE4_1
#   endif
#   if !defined(KV_SIMD_X86) && defined(__SSSE3__)
#      define KV_SIMD_X86 KV_SIMD_X86_SSSE3
#   endif
#   if !defined(KV_SIMD_X86) && defined(__SSE3__)
#      define KV_SIMD_X86 KV_SIMD_X86_SSE3
#   endif
#   if !defined(KV_SIMD_X86) && (defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2))
#      define KV_SIMD_X86 KV_SIMD_X86_SSE2
#   endif
#   if !defined(KV_SIMD_X86) && (defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1))
#      define KV_SIMD_X86 KV_SIMD_X86_SSE
#   endif
#   if !defined(KV_SIMD_X86) && defined(__MMX__)
#      define KV_SIMD_X86 KV_SIMD_X86_MMX
#   endif
#   if defined(KV_SIMD_X86)
#      if !defined(KV_SIMD_VERSION) && !defined(KV_SIMD_NAME)
#           define KV_SIMD_NAME    KV_SIMD_X86_NAME
#           define KV_SIMD_VERSION KV_SIMD_X86
#      else
#           error "multiple simd instructions detected, is it possible?"
#      endif
#   endif
#endif


#ifndef KV_SIMD_X86_AMD_NAME
#   define KV_SIMD_X86_AMD_NAME "SIMD x86 (AMD)"
#   if !defined(KV_SIMD_X86_AMD) && defined(__XOP__)
#      define KV_SIMD_X86_AMD KV_SIMD_X86_AMD_XOP_VERSION
#   endif
#   if !defined(KV_SIMD_X86_AMD) && defined(__FMA4__)
#      define KV_SIMD_X86_AMD KV_SIMD_X86_AMD_FMA4_VERSION
#   endif
#   if !defined(KV_SIMD_X86_AMD) && defined(__SSE4A__)
#      define KV_SIMD_X86_AMD KV_SIMD_X86_AMD_SSE4A_VERSION
#   endif
#   if defined(KV_SIMD_X86_AMD) && (KV_SIMD_X86_AMD > KV_SIMD_X86)
#      undef KV_SIMD_NAME
#      undef KV_SIMD_VERSION
#      define KV_SIMD_NAME    KV_SIMD_X86_NAME
#      define KV_SIMD_VERSION KV_SIMD_X86
#   endif
#endif