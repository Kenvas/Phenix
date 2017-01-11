#pragma once

#define KV_MAKE_VERSION(version, revision, patch) ( (((version)%100)*10000000) + (((revision)%100)*100000) + ((patch)%100000) )

#define KV_VERSION_MIN       KV_MAKE_VERSION(0, 0, 1)
#define KV_VERSION_MAX       KV_MAKE_VERSION(99, 99, 99999)
#define KV_VERSION_UNKNOWN   KV_MAKE_VERSION(0, 0, 0)
#define KV_VERSION_AVAILABLE KV_VERSION_MIN

#define __KV_POW10_(x) __KV_POW10_ ## x
#define __KV_POW10_0 1
#define __KV_POW10_1 10
#define __KV_POW10_2 100
#define __KV_POW10_3 1000
#define __KV_POW10_4 10000
#define __KV_POW10_5 100000
#define __KV_POW10_6 1000000
#define __KV_POW10_7 10000000
#define __KV_POW10_8 100000000

#define __KV_VERSION_AUX_HEX_MASK(x, l, h) ( ( (x) & ( ((1LL << (h << 2)) - 1) ^ ((1LL << (l << 2)) - 1))  ) >> (l << 2) )
#define __KV_VERSION_AUX_DEC_MASK(x, d, m) ( ( (x) / __KV_POW10_(d) % __KV_POW10_(m) ) )
#define __KV_VERSION_AUX_MAKE_FROM_NORMAL(fn, version, revision, patch) KV_MAKE_VERSION(fn version, fn revision, fn patch)
#define __KV_VERSION_AUX_MAKE_FROM_DATE(fn, year, month, day) KV_MAKE_VERSION((fn year)-1970, (fn month), (fn day))
// #define __KV_VERSION_AUX_KEEP_HEX(x) ((((x)%16)*1) + (((x)/16%16)*10) + (((x)/256%16)*100) + (((x)/4096%16)*1000))
// #define __KV_VERSION_AUX_MAKE_FROM_HEX(fn, version, revision, patch) KV_MAKE_VERSION( \
//     __KV_VERSION_AUX_KEEP_HEX(fn version), __KV_VERSION_AUX_KEEP_HEX(fn revision), __KV_VERSION_AUX_KEEP_HEX(fn patch) )


#define KV_MAKE_VERSION_FROM_0xVRP(x)      __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 2, 3), ((x), 1, 2), ((x), 0, 1))
#define KV_MAKE_VERSION_FROM_0xVRR(x)      __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 2, 3), ((x), 0, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_0xVRPP(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 3, 4), ((x), 2, 3), ((x), 0, 2))
#define KV_MAKE_VERSION_FROM_0xVVRP(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 2, 4), ((x), 1, 2), ((x), 0, 1))
#define KV_MAKE_VERSION_FROM_0xVVRR(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 2, 4), ((x), 0, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_0xVVRRP(x)    __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 3, 5), ((x), 1, 3), ((x), 0, 1))
#define KV_MAKE_VERSION_FROM_0xVVRRPP(x)   __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 4, 6), ((x), 2, 4), ((x), 0, 2))
#define KV_MAKE_VERSION_FROM_0xVVRRPPPP(x) __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 6, 8), ((x), 4, 6), ((x), 0, 4))
#define KV_MAKE_VERSION_FROM_0xVRRPP000(x) __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_HEX_MASK, ((x), 7, 8), ((x), 5, 7), ((x), 3, 5))


#define KV_MAKE_VERSION_FROM_VRR(x)        __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 2, 1), ((x), 0, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_VRP(x)        __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 2, 1), ((x), 1, 1), ((x), 0, 1))
#define KV_MAKE_VERSION_FROM_VRPP(x)       __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 3, 1), ((x), 2, 1), ((x), 0, 2))
#define KV_MAKE_VERSION_FROM_VPPP(x)       __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 3, 1), ((x), 0, 0), ((x), 0, 3))
#define KV_MAKE_VERSION_FROM_VV00(x)       __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 2, 2), ((x), 0, 0), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_VVRR(x)       __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 2, 2), ((x), 0, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_VRRPP(x)      __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 4, 1), ((x), 2, 2), ((x), 0, 2))
#define KV_MAKE_VERSION_FROM_VRR000(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 5, 1), ((x), 3, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_VRP000(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 5, 1), ((x), 4, 1), ((x), 3, 1))
#define KV_MAKE_VERSION_FROM_VVRRPP(x)     __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 4, 2), ((x), 2, 2), ((x), 0, 2))
#define KV_MAKE_VERSION_FROM_VVRR0PP00(x)  __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 7, 2), ((x), 5, 2), ((x), 2, 2))
#define KV_MAKE_VERSION_FROM_VVRR0PPPP(x)  __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 7, 2), ((x), 5, 2), ((x), 0, 4))
#define KV_MAKE_VERSION_FROM_VVRR00PP00(x) __KV_VERSION_AUX_MAKE_FROM_NORMAL(__KV_VERSION_AUX_DEC_MASK, ((x), 8, 2), ((x), 6, 2), ((x), 2, 2))


#define KV_MAKE_VERSION_FROM_YYYY(x)     __KV_VERSION_AUX_MAKE_FROM_DATE(__KV_VERSION_AUX_DEC_MASK, ((x), 0, 4), ((x), 0, 0), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_YYYYMM(x)   __KV_VERSION_AUX_MAKE_FROM_DATE(__KV_VERSION_AUX_DEC_MASK, ((x), 2, 6), ((x), 0, 2), ((x), 0, 0))
#define KV_MAKE_VERSION_FROM_YYYYMMDD(x) __KV_VERSION_AUX_MAKE_FROM_DATE(__KV_VERSION_AUX_DEC_MASK, ((x), 4, 8), ((x), 2, 4), ((x), 0, 2))
