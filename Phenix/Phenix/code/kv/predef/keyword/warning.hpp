#pragma once

#ifndef warning_disable
#define warning_disable(codes) __pragma(warning(push)) __pragma(warning(disable: codes))
#else
static_assert(false, "macro 'warning_disable' redefined.");
#endif

#ifndef warning_enable
#define warning_enable() __pragma(warning(pop))
#else
static_assert(false, "macro 'warning_enable' redefined.");
#endif