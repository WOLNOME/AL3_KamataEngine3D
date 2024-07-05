#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <algorithm>
#include <cmath>
#include <vector>

Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
float Length(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& vs, const Vector3& ve, const float l);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Matrix4x4 Inverse(const Matrix4x4& m);
Vector3 Multiply(const float& s, const Vector3& v);
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, const float t);
