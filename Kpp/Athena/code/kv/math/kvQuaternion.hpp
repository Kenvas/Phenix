#pragma once

#include "Eigen/Core"

#include "_namespace/begin"

template <class _Scalar = float>
using TQuaternion = Eigen::Quaternion<_Scalar>;

using Quaternionf = TQuaternion<float>;
using Quaternion = Quaternionf;
extern template Quaternionf;

#include "_namespace/end"