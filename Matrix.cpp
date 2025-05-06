#include "Matrix.h"
//math.hではなくcmathをinclude
#include <cmath>

//拡大縮小行列
Matrix4x4 MatrixMath::MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 result = { {
	   {scale.x, 0,  0,  0},  // 横の大きさ（x方向）
	   {0,  scale.y, 0,  0},  // 縦の大きさ（y方向）
	   {0,  0,  scale.z, 0},  // 奥行きの大きさ（z方向）
	   {0,  0,  0,  1}        // おまじない（そのままでOK）
   } };

	return result;
}

//X軸回転行列
Matrix4x4 MatrixMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 result = { {

		{1.0f,0.0f,0.0f,0.0f},
		{0.0f,std::cos(radian),std::sin(radian),0.0f},
		{0.0f,-std::sin(radian),std::cos(radian),0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };

	return result;
}

//Y軸回転行列
Matrix4x4 MatrixMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { {

		{std::cos(radian),0.0f,-std::sin(radian),0.0f},
		{0.0f,1.0f,0.0f,0.0f},
		{std::sin(radian),0.0f,std::cos(radian),0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };
	return result;
}

//Z軸回転行列
Matrix4x4 MatrixMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { {

		{ std::cos(radian),std::sin(radian),0.0f,0.0f},
		{-std::sin(radian),std::cos(radian),0.0f,0.0f},
		{0.0f,0.0f,1.0f,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	} };
	return result;
}

//平行移動行列
Matrix4x4 MatrixMath::MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 result = { {
		{1, 0, 0, 0},  // x方向にtranslateだけ動かす
		{0, 1, 0, 0},  // y方向にtranslateだけ動かす
		{0, 0, 1, 0},  // z方向にtranslateだけ動かす
		{translate.x, translate.y, translate.z, 1}    // この行は変えない（特別な意味）
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

Matrix4x4 MatrixMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;



	//回転行列を生成する
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	
	result = {
		scale.x * rotateMatrix.m[0][0],scale.x * rotateMatrix.m[0][1],scale.x * rotateMatrix.m[0][2],0.0f,
		scale.y * rotateMatrix.m[1][0],scale.y * rotateMatrix.m[1][1],scale.y * rotateMatrix.m[1][2],0.0f,
		scale.z * rotateMatrix.m[2][0],scale.z * rotateMatrix.m[2][1],scale.z * rotateMatrix.m[2][2],0.0f,
		translate.x,translate.y,translate.z,1.0f
	};

	return result;
}



