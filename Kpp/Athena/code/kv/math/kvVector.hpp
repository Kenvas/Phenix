#pragma once

#include "Eigen/Core"

#include "_namespace/begin"

template <int _Dimensions, class _Scalar = float, bool _IsColumnMajor = true>
using TVector = Eigen::Matrix<_Scalar, (_IsColumnMajor ? _Dimensions : 1), (_IsColumnMajor ? 1 : _Dimensions)>;

using Vector2f = TVector<2, float>;
using Vector2i = TVector<2, int32_t>;
using Vector2u = TVector<2, uint32_t>;
using Vector2  = Vector2f;
using RowVector2f = TVector<2, float, false>;
using RowVector2i = TVector<2, int32_t, false>;
using RowVector2u = TVector<2, uint32_t, false>;
using RowVector2 = RowVector2f;
extern template Vector2f;
extern template Vector2i;
extern template Vector2u;
extern template RowVector2f;
extern template RowVector2i;
extern template RowVector2u;

using Vector3f = TVector<3, float>;
using Vector3i = TVector<3, int32_t>;
using Vector3u = TVector<3, uint32_t>;
using Vector3  = Vector3f;
using RowVector3f = TVector<3, float, false>;
using RowVector3i = TVector<3, int32_t, false>;
using RowVector3u = TVector<3, uint32_t, false>;
using RowVector3 = RowVector3f;
extern template Vector3f;
extern template Vector3i;
extern template Vector3u;
extern template RowVector3f;
extern template RowVector3i;
extern template RowVector3u;

using Vector4f = TVector<4, float>;
using Vector4i = TVector<4, int32_t>;
using Vector4u = TVector<4, uint32_t>;
using Vector4  = Vector4f;
using RowVector4f = TVector<4, float, false>;
using RowVector4i = TVector<4, int32_t, false>;
using RowVector4u = TVector<4, uint32_t, false>;
using RowVector4 = RowVector4f;
extern template Vector4f;
extern template Vector4i;
extern template Vector4u;
extern template RowVector4f;
extern template RowVector4i;
extern template RowVector4u;

#include "_namespace/end"