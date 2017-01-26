#include "PrecompiledHeader.hpp"

#include "kv/entry/IncludeAll.hpp"
#include "kv/log/IncludeAll.hpp"
#include "kv/predef/IncludeAll.hpp"

#include "kv/native/windows/utils.hpp"
#include "kv/native/windows/wgl.hpp"

#include "kv/common/Random.hpp"

#include <random>

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

using namespace kv;

struct TestStatic
{
    char const * s;
    TestStatic(char const * const str)
    {
        s = str;
        log::trace("ctor: ")(s)();
    }

    ~TestStatic()
    {
        log::trace("dtor: ")(s)();
    }
};


template <class T>
struct StaticVar
{
    public: static constexpr T & Get() noexcept
    {
        static char buffer[sizeof(T)];
        return reinterpret_cast<T &>(*buffer);
    }

    public: template<class ... ArgsType> static constexpr void Create(ArgsType&& ... args) noexcept
    {
        autox buffer_ref = StaticVar<T>::Get();
        new (&buffer_ref) T(std::forward<ArgsType>(args) ...);
    }

    public: static constexpr void Destroy() noexcept
    {
        autox buffer_ref = StaticVar<T>::Get();
        buffer_ref.~T();
    }
};

KV_QuickAddEntry
{
    //log::debug.Write("���ã�����");

    std::map<int, int> hist;
    for (int n = 0; n < 10000; ++n) {
        ++hist[int(std::round(kv::Random::Shared.UniformI(3)))];
        //random.UniformI(32);
    }
    for (auto p : hist) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
            << p.first << ' ' << std::string(p.second / 200, '*') << '\n';
    }

    StaticVar<TestStatic>::Create("in func 2");
    StaticVar<TestStatic>::Destroy();

    return 0;
};