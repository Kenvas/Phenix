#include "PrecompiledHeader.hpp"

#include "../kvRandom.hpp"
#include "kv/predef/Keyword.hpp"

#include <array>

using namespace std;
using namespace kv;

#include "../_namespace/begin"

Random::EnginePtrType CreateEngine() noexcept
{
#if 0
    char seeds[] = "make world full of love & peace";
    return make_shared<Random::EngineType>(seed_seq(seeds, seeds + sizeof(seeds)));
#else
    using EngineType = Random::EngineType;
    using DataType   = EngineType::result_type;
    using ArrayType  = array<DataType, EngineType::state_size>;
    autox states = ArrayType{ Random::Generate(), Random::Generate() };
    //generate_n(states.data(), EngineType::state_size, Random::Generate);
    seed_seq seq(states.begin(), states.end());
    return make_shared<EngineType>(seq);
#endif
}

Random const Random::Shared = Random::Random(CreateEngine());

Random::DeviceType::result_type Random::Generate() noexcept
{
    return DeviceType()();
}

Random::Random() noexcept
    : EnginePtr_(Shared.EnginePtr_)
{
}

Random::Random(EnginePtrType const engine_ptr) noexcept
    : EnginePtr_(engine_ptr)
{
}

Random::IntegralType Random::UniformI(IntegralType const n) const noexcept
{
    return UniformI(0, n - 1);
}

Random::IntegralType Random::UniformI(IntegralType const min, IntegralType const max) const noexcept
{
    using distribution = uniform_int_distribution<IntegralType>;
    return distribution(min, max)(*EnginePtr_);
}

Random::FloatingType Random::UniformF(FloatingType const n) const noexcept
{
    return UniformF(0, n);
}

Random::FloatingType Random::UniformF(FloatingType const min, FloatingType const max) const noexcept
{
    using distribution = uniform_real_distribution<FloatingType>;
    return distribution(min, max)(*EnginePtr_);
}

Random::FloatingType Random::Normal(FloatingType const mean, FloatingType const sigma) const noexcept
{
    using distribution = normal_distribution<FloatingType>;
    return distribution(mean, sigma)(*EnginePtr_);
}

Random::IntegralType Random::Poisson(FloatingType const mean) const noexcept
{
    using distribution = poisson_distribution<IntegralType>;
    return distribution(mean)(*EnginePtr_);
}

bool Random::Bernoulli(FloatingType const p) const noexcept
{
    using distribution = bernoulli_distribution;
    return distribution(p)(*EnginePtr_);
}

#include "../_namespace/end"