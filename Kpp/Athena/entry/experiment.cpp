#include "PrecompiledHeader.hpp"

#include "kv/log/IncludeAll.hpp"
#include "kv/common/kvRandom.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h>

using namespace kv;
using namespace std;

KV_QuickAddEntry
{
    auto count  = 100;
    auto rate   = 0.8f;
    auto data   = vector<int>(count);
    auto avgval = 0.0f;
    auto weight = 0.0f;
    auto limwgt = 1.0f / (1.0f - rate);

    for (auto i = 0; i < count; i++)
    {
        auto val = data[i];
        weight = weight * rate + 1.0f;
        avgval = (avgval * rate + val);
        auto accu = avgval / weight;
        auto prox = avgval / limwgt;
        log::debug("{0:<5}({1:<10}, {2:<10}) ", val, accu, prox)();
    }


    log::debug("mouse enable: {0}", GetSystemMetrics(SM_MOUSEPRESENT))();
    log::debug("mouse wheel enable: {0}", GetSystemMetrics(SM_MOUSEWHEELPRESENT))();
    log::debug("mouse button count: {0}", GetSystemMetrics(SM_CMOUSEBUTTONS))();
    log::debug("mouse button swapped: {0}", GetSystemMetrics(SM_SWAPBUTTON))();

    return 0;
};