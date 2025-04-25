#include "Matrix.h"
//math.hではなくcmathをinclude
#include <cmath>

Matrix4x4 MatrixMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 result = { {

		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,std::cos(radian),std::sin(radian),0.0f},
		{0.0f,-std::sin(radian),std::cos(radian),0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };

	return result;
}

Matrix4x4 MatrixMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { {

		{std::cos(radian),0.0f,-std::sin(radian),0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{std::sin(radian),0.0f,std::cos(radian),0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };
	return result;
}

Matrix4x4 MatrixMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { {

		{ std::cos(radian),std::sin(radian),0.0f,0.0f},
		{-std::sin(radian),std::cos(radian),0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };
	return result;
}

Matrix4x4 MatrixMath::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;  // 初期化
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}
