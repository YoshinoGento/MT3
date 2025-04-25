#pragma once
#include "Struct.h"
struct  Matrix4x4 {
	float m[4][4];
};
namespace MatrixMath {

	//1.X軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);
	//2.Y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);
	//3.Z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radian);
	//4.XY軸回転行列の積
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	
};

