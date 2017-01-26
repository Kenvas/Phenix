#include "PrecompiledHeader.hpp"
#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"
#include "kv/predef/IncludeAll.hpp"

using namespace kv;

#define TestVersionDecMacro(macro, input, expected) log::debug((macro((input)) == (expected)) ? log::color::green : log::color::red); \
    log::debug("{0:<32}({1:<12}): expected = {2:>10}, actual = {3:>10}", #macro, (input), (expected), macro((input)))();
#define TestVersionHexMacro(macro, input, expected) log::debug((macro((input)) == (expected)) ? log::color::green : log::color::red); \
    log::debug("{0:<32}(0x{1:<10x}): expected = {2:>10}, actual = {3:>10}", #macro, (input), (expected), macro((input)))();

#define __MacroInfo(module, member) KV_ ## module ## _ ## member
#define PrintMacroInfo(module, member) \
    log::debug(log::color::cyan)("{0:<8} {1:<10}: {2}", #module, #member, __MacroInfo(module, member))();

KV_QuickAddEntry
{
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VPPP      , 12345678  , 50000678 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VPPP      , 1234      , 10000234 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRP       , 123       , 10200003 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRP000    , 123456    , 10200003 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRPP      , 1234      , 10200034 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRR       , 123       , 12300000 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRR000    , 123400    , 12300000 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VRRPP     , 12340     , 12300040 );
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VV00      , 1234      , 120000000);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VVRR      , 1234      , 123400000);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VVRR00PP00, 1234567890, 123400078);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VVRR0PP00 , 123456789 , 123400067);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VVRR0PPPP , 123456789 , 123406789);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_VVRRPP    , 123456    , 123400056);

    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVRP     , 0x123     , 10200003 );
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVRPP    , 0x1234    , 10200052 );
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVRRPP000, 0x12345678, 13500069 );
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVVRP    , 0x1234    , 180300004);
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVVRR    , 0x1234    , 185200000);
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVVRRP   , 0x12345   , 185200005);
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVVRRPP  , 0x123456  , 185200086);
    TestVersionHexMacro(KV_MAKE_VERSION_FROM_0xVVRRPPPP, 0x12345678, 185222136);

    TestVersionDecMacro(KV_MAKE_VERSION_FROM_YYYY    , 2017    , 470000000);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_YYYYMM  , 201701  , 470100000);
    TestVersionDecMacro(KV_MAKE_VERSION_FROM_YYYYMMDD, 20170108, 470100008);

    cout << endl;
    PrintMacroInfo(ARCH, NAME);
    PrintMacroInfo(ARCH, VERSION);

    cout << endl;
    PrintMacroInfo(COMPILER, NAME);
    PrintMacroInfo(COMPILER, VERSION);
#ifdef KV_COMPILER_NAME_BACK
    PrintMacroInfo(COMPILER, NAME_BACK);
    PrintMacroInfo(COMPILER, VERSION_BACK);
#endif

    cout << endl;
    PrintMacroInfo(OS, NAME);
    PrintMacroInfo(OS, VERSION);

    cout << endl;
    PrintMacroInfo(SIMD, NAME);
    PrintMacroInfo(SIMD, VERSION);

    return 0;
};
