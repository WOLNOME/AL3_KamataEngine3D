#pragma once

#define _USE_MATH_DEFINES
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <math.h>

// よく使う数値
const float pi = (float)M_PI;
const int kHowToPlayPageNum = 5;

// 列挙型
enum SCENE {
	TITLE,
	STAGE,
	DIRECTIION,
	GAMEOVER,
	RESULT,
};

// 使ってるデバイス
enum Device {
	KEYBOARD,
	GAMEPAD,
};

// 使われるシーン
enum UseScene {
	USETITLE,
	USESTAGE,
	USEDIRECTION,
	USEGAMEOVER,
	USERESULT,
};

// 構造体
struct Matrix3x3 {
	float m[3][3];
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct Sphere {
	Vector3 center;
	float radius;
};

// 関数
Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtract(const Vector3& v1, const Vector3& v2);

float Multiply(const Vector3& v1, const Vector3& v2);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 Transpose(const Matrix4x4& m);

Matrix4x4 MakeIdentity4x4();

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

float Cot(float rad);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 makeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Cross(const Vector3& a, const Vector3& b);

float Cross(const Vector2& a, const Vector2& b);

float Length(const Vector3& v);

float Length(const Vector2& v);

Vector3 Normalize(const Vector3& v);

Vector2 Normalize(const Vector2& v);

Vector3 Transform(const Vector3& vecotor, const Matrix4x4& matrix);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

float Dot(const Vector3& v1, const Vector3& v2);

float Dot(const Vector2& v1, const Vector2& v2);

float AngleOf2VectorX(const Vector3& v1, const Vector3& v2);

float AngleOf2VectorY(const Vector3& v1, const Vector3& v2);

float AngleOf2VectorZ(const Vector3& v1, const Vector3& v2);

float AngleOf2Vector(const Vector2& v1, const Vector2& v2);

Vector3 Multiply(const float& s, const Vector3& v);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

float Lerp(const float& l1, const float& l2, float t);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);
