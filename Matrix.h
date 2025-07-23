#pragma once  
#include "Struct.h"  
#include "assert.h"  
#include <cstdint> 

struct Matrix4x4 {  
   float m[4][4];  
};  

namespace MatrixMath {


	////1.行列の加法
	//Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	////2.行列の減法
	//Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

	//1.行列の加法
	Vector3 Add(const Vector3& m1, const Vector3& m2);
	//2.行列の減法
	Vector3 Subtract(const Vector3& m1, const Vector3& m2);

	//1.透視投影行列  
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	//2.正射影行列  
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	//3.ビューポート変換行列  
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	//--アフィン行列--//～～～～～～～～～～～～～～～～～～～～  
	//1.拡大縮小行列  
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	//2.X軸回転行列  
	Matrix4x4 MakeRotateXMatrix(float radian);

	//3.Y軸回転行列  
	Matrix4x4 MakeRotateYMatrix(float radian);

	//4.Z軸回転行列  
	Matrix4x4 MakeRotateZMatrix(float radian);

	//5.行列の積  
	Matrix4x4 MultiplyM(const Matrix4x4& m1, const Matrix4x4& m2);

	Vector3 MultiplyV(float scalar, const Vector3& v);


	//平行移動行列  
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	//3次元アフィン変換行列  
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	//_________________________________________________________________________________  

	//クロス積  
	Vector3 Cross(const Vector3& v1, const Vector3& v2);

	//内積
	float Dot(const Vector3& v1, const Vector3& v2);

	//4.逆行列  
	Matrix4x4 Inverse(const Matrix4x4& m);

	//3.座標変換  
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	//
	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrx, const Matrix4x4& viewportMatrix, uint32_t color);

	//グリッド  
	void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

	//単位行列
	Matrix4x4 MakeIdentity();

	//
	Vector3 Project(const Vector3& v1, const Vector3& v2);

	Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	//平面の描画
	void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	//平面と線分の衝突判定
	bool IsCollisionP(const Segment& segment, const Plane& plane);

	//三角形の描画
	void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	//三角形と線分の衝突判定
	bool IsCollisionT(const Triangle& triangle, const Segment& segment);

	Vector3 Perpendicular(const Vector3& vector);

	//長さ(ノルム)
	float Length(const Vector3& v);

	//正規化
	Vector3 Normalize(const Vector3& v);

	//AABBとAABBの衝突判定（立方体の衝突判定）
	bool IsCollisionAABB(const AABB& aabb1, const AABB& aabb2);

	//AABBの描画
	void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

	//AABBと球の衝突判定
	void IsCollisionSphereAABB(const AABB &aabb,const Sphere & sphere);



};
