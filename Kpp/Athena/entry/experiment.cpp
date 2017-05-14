#include "PrecompiledHeader.hpp"

#include "kv/log/IncludeAll.hpp"
#include "kv/common/kvRandom.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h>

#include "Eigen/Geometry"
#include "kv/math/IncludeAll.hpp"

using namespace kv;
using namespace std;
using namespace kv::math;

KV_QuickAddEntry
{

    auto x = Vector3f(1, 2, 3);
    auto y = Vector3f(1, 2, 3);

    auto q = Vector3f::Identity();

    Eigen::Affine3f t;
    t = Eigen::AngleAxisf(45.0f * 3.14f / 180, Vector3f::Zero());

    auto xx = t * x.homogeneous();

    cout << sizeof(Eigen::Affine3f) << endl;
    cout << t.matrix() << endl;
    cout << x << endl;
    cout << xx << endl;

    return 0;
};