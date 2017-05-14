#pragma once

#include "Eigen/Core"

#include "_namespace/begin"

template <int _Rows, int _Columns, class _Scalar = float>
using TMatrix = Eigen::Matrix<_Scalar, _Rows, _Columns>;

using Matrix3f   = TMatrix<3, 3>;
using Matrix2x3f = TMatrix<2, 3>;
using Matrix3    = Matrix3f;
using Matrix2x3  = Matrix2x3f;
extern template Matrix3f;
extern template Matrix2x3f;

using Matrix4f   = TMatrix<4, 4>;
using Matrix3x4f = TMatrix<3, 4>;
using Matrix4    = Matrix4f;
using Matrix3x4  = Matrix3x4f;
extern template Matrix3f;
extern template Matrix3x4f;

#include "_namespace/end"