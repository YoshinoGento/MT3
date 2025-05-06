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

Matrix4x4 MatrixMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;

	//拡大縮小行列を生成する
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	//回転行列を生成する
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = {

	}

	//平行移動行列を生成する
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 result = {
		{scale.x*,scale.x*rotate.x,scale.x*rotate.x}
	}

	return result;
}



