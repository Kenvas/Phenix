#pragma once

#include "kv/predef/Type.hpp"
#include "kv/predef/Keyword.hpp"

#include <memory>
#include <random>

#include "_namespace/begin"

class Random final
{
public:

    using IntegralType  = int64_t;
    using FloatingType  = float64_t;
    using DeviceType    = std::random_device;
    using EngineType    = std::mt19937_64;
    using EnginePtrType = std::shared_ptr<EngineType>;

    static Random const Shared;

private:

    EnginePtrType EnginePtr_;

    Random(EnginePtrType const engine_ptr) noexcept;

public:

    static DeviceType::result_type Generate() noexcept;

public:

    Random() noexcept;

    IntegralType UniformI(IntegralType const n) const noexcept;
    IntegralType UniformI(IntegralType const min, IntegralType const max) const noexcept;

    FloatingType UniformF(FloatingType n) const noexcept;
    FloatingType UniformF(FloatingType const min, FloatingType const max) const noexcept;

    FloatingType Normal(FloatingType const mean, FloatingType const sigma) const noexcept;

    IntegralType Poisson(FloatingType const mean) const noexcept;

    bool Bernoulli(FloatingType const p) const noexcept;

};

#include "_namespace/end"