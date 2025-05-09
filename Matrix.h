#pragma once
#include "Struct.h"
struct  Matrix4x4 {
	float m[4][4];
};
namespace MatrixMath {

	//1.透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//2.正射影行列
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	//3.ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	
};

