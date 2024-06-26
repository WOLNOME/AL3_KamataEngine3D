#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Transform(const Vector3& vecotor, const Matrix4x4& matrix);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
float Length(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& vs, const Vector3& ve,const float l);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Vector3 Multiply(const float& s, const Vector3& v);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);