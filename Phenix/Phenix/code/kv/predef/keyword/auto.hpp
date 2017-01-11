#pragma once

#ifndef autox
#define autox decltype(auto)
#else
static_assert(false, "macro 'autox' redefined.");
#endif