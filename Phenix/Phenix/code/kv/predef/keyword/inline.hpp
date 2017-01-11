#pragma once

#ifndef kvinline
#define kvinline(x) __kvinline_ ## x
#else
static_assert(false, "macro 'kvinline' redefined.");
#endif

#ifndef __kvinline_on
#define __kvinline_on inline
#else
static_assert(false, "macro '__kvinline_on' redefined.");
#endif

#ifndef __kvinline_off
#define __kvinline_off __declspec(noinline)
#else
static_assert(false, "macro '__kvinline_off' redefined.");
#endif

