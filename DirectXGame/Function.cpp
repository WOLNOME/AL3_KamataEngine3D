#include "Function.h"
#include <cassert>

Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 c;
	c.x = v1.x + v2.x;
	c.y = v1.y + v2.y;
	c.z = v1.z + v2.z;
	return c;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 c;
	c.x = v1.x - v2.x;
	c.y = v1.y - v2.y;
	c.z = v1.z - v2.z;
	return c;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

float Length(const Vector3& v1, const Vector3& v2) {
	float c;
	c = sqrtf(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2) + powf(v1.z - v2.z, 2));
	return c;
}

float Length2(const Vector3& v) {
	float c;
	c = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
	return c;
}

float Length2(const Vector2& v) { 
	float c;
	c = sqrtf(powf(v.x, 2) + powf(v.y, 2));
	return c;
}

Vector3 Normalize(const Vector3& vs, const Vector3& ve, const float l) {
	Vector3 c;
	Vector3 sv;
	float length = Length(vs, ve);
	sv = Subtract(ve, vs);
	if (length != 0) {
		c.x = sv.x / length;
		c.y = sv.y / length;
		c.z = sv.z / length;
	} else {
		assert("正規化できません");
	}
	c.x *= l;
	c.y *= l;
	c.z *= l;
	return c;
}

Vector3 Normalize2(const Vector3& v) {
	Vector3 c;
	// 長さを求める
	float length = Length2(v);
	// length=0で無ければ正規化
	if (length != 0) {
		c.x = v.x / length;
		c.y = v.y / length;
		c.z = v.z / length;
	} else {
		assert("正規化できません");
	}
	return c;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 c;
	c.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	c.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	c.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	c.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
	c.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	c.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	c.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	c.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
	c.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	c.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	c.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	c.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
	c.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	c.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	c.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	c.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return c;
}
// アフィン変換関数
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 c;
	c.m[0][0] = 1;
	c.m[0][1] = 0;
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = std::cos(radian);
	c.m[1][2] = std::sin(radian);
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = -std::sin(radian);
	c.m[2][2] = std::cos(radian);
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 c;
	c.m[0][0] = std::cos(radian);
	c.m[0][1] = 0;
	c.m[0][2] = -std::sin(radian);
	c.m[0][3] = 0;
	c.m[1][0] = 0;
	c.m[1][1] = 1;
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = std::sin(radian);
	c.m[2][1] = 0;
	c.m[2][2] = std::cos(radian);
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 c;
	c.m[0][0] = std::cos(radian);
	c.m[0][1] = std::sin(radian);
	c.m[0][2] = 0;
	c.m[0][3] = 0;
	c.m[1][0] = -std::sin(radian);
	c.m[1][1] = std::cos(radian);
	c.m[1][2] = 0;
	c.m[1][3] = 0;
	c.m[2][0] = 0;
	c.m[2][1] = 0;
	c.m[2][2] = 1;
	c.m[2][3] = 0;
	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;
	return c;
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 c;
	// 回転行列統合
	Matrix4x4 rx = MakeRotateXMatrix(rotate.x);
	Matrix4x4 ry = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rz = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rxyz = Multiply(rx, Multiply(ry, rz));

	c.m[0][0] = scale.x * rxyz.m[0][0];
	c.m[0][1] = scale.x * rxyz.m[0][1];
	c.m[0][2] = scale.x * rxyz.m[0][2];
	c.m[0][3] = 0;
	c.m[1][0] = scale.y * rxyz.m[1][0];
	c.m[1][1] = scale.y * rxyz.m[1][1];
	c.m[1][2] = scale.y * rxyz.m[1][2];
	c.m[1][3] = 0;
	c.m[2][0] = scale.z * rxyz.m[2][0];
	c.m[2][1] = scale.z * rxyz.m[2][1];
	c.m[2][2] = scale.z * rxyz.m[2][2];
	c.m[2][3] = 0;
	c.m[3][0] = translate.x;
	c.m[3][1] = translate.y;
	c.m[3][2] = translate.z;
	c.m[3][3] = 1;

	return c;
}

Vector3 Multiply(const float& s, const Vector3& v) {
	Vector3 c;
	c.x = s * v.x;
	c.y = s * v.y;
	c.z = s * v.z;
	return c;
}

float Dot(const Vector3& v1, const Vector3& v2) {
	float c;
	c = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return c;
}

float Dot2(const Vector2& v1, const Vector2& v2) {
	float c;
	c = v1.x * v2.x + v1.y * v2.y;
	return c;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 p;
	p = Add(v1, Multiply(t, Subtract(v2, v1)));
	return p;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	//各ベクトルを正規化
	Vector3 v1n, v2n;
	v1n = Normalize2(v1);
	v2n = Normalize2(v2);
	//内積を求める→cosθ
	float dot = Dot(v1n, v2n);
	//sinθを求める
	if (dot > 1.0f) {
		dot = 1.0f;
	}
	//アークコサインでθの角度を求める
	float theta = std::acos(dot);
	//シータの角度からsinθを求める
	float sinTheta = std::sin(theta);
	//サイン(θ(1-t))を求める
	float sinThetaFrom = std::sin((1 - t) * theta);
	//sinθtを求める
	float sinThetaTo = std::sin(t * theta);
	//球面線形補完したベクトル(単位ベクトル)
	Vector3 nvec = sinThetaFrom / sinTheta * v1n + sinThetaTo / sinTheta * v2n;

	return result;
}
