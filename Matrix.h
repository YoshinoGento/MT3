#pragma once
#include "Struct.h"
struct  Matrix4x4 {
	float m[4][4];
};
namespace MatrixMath {

	//1.拡大縮小行列
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	//2.X軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);
	//3.Y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);
	//4.Z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radian);
	//5.行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	//平行移動行列
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	//3次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	
	
};

