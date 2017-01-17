#include "PrecompiledHeader.hpp"

#include "kv/entry/IncludeAll.hpp"
#include "kv/predef/IncludeAll.hpp"

#include "kv/native/windows/utils.hpp"
#include "kv/native/windows/wgl.hpp"

#include "kv/log/IncludeAll.hpp"
#include "kv/log/Logger.hpp"

#include "spdlog/spdlog.h"

using namespace kv;

KV_QuickAddEntry
{
    log::debug.write("���ã�����");
    log::debug.write("{0}", "hello world");
    log::debug.writeln("hello world");
    log::debug.writeln("{0}", "hello world");
    log::debug("hello world")();
    log::debug("{0}", "hello world")();
    log::debug(log::color::red, log::color::on_blue)("{0}", "hello world")();


    return 0;
};