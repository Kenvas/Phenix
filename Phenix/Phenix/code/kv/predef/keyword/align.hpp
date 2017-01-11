#pragma once

#ifndef align_push
#define align_push(n) __pragma(pack(push, n))
#else
static_assert(false, "macro 'align_push' redefined.");
#endif

#ifndef align_pop
#define align_pop() __pragma(pack(pop))
#else
static_assert(false, "macro 'align_pop' redefined.");
#endif