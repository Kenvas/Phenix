#pragma once

#ifndef autox
#define autox decltype(auto)
#else
static_assert(false, "autox has been used at somewhere.");
#endif

#ifndef _alias_
//#define _alias_(name, ...) typedef typename __VA_ARGS__ name
#define _alias_(name, ...) using name = __VA_ARGS__
#else
static_assert(false, "_alias_ has been used at somewhere.");
#endif