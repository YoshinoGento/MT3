#pragma once  
#include "Struct.h"  
#include "assert.h"  
#include <cstdint> 
#include <array>

// -- 4x4行列 -- //
struct Matrix4x4 {
	float m[4][4];
	Matrix4x4() {
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = (i == j) ? 1.0f : 0.0f;
	}
	Matrix4x4 operator*(const Matrix4x4& rhs) const {
		Matrix4x4 R;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				R.m[i][j] = 0;
				for (int k = 0; k < 4; k++) {
					R.m[i][j] += m[i][k] * rhs.m[k][j];
				}
			}
		}
		return R;
	}
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

	//6.回転行列（オイラー角）
	Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

	// ビュー行列作成（LookAt方式）
	Matrix4x4 MakeViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

	// 透視射影行列を作成
	Matrix4x4 MakePerspectiveMatrix(float fovY, float aspect, float nearZ, float farZ);

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
	bool IsCollisionSphereAABB(const AABB &aabb,const Sphere & sphere);

	//AABBと線分の衝突判定
	bool IsIntersectAABBAndSegment(const AABB& box, const segment& seg);

	//線分の描画
	void DrawSegment(const segment& seg, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color);

	//線形補間関数（Leap)
	Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

	//2次ベジェ曲線関数
	Vector3 QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

	//ベジェ曲線の描画関数
	void DrawBezierCurve(const Vector3 controlPoints[3], const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color);


	//    --- 3次ベジェ曲線関数 ---   //
	Matrix4x4 MakeRotateXMatrix(float rad);

	Matrix4x4 MakeRotateYMatrix(float rad);

	Matrix4x4 MakeRotateZMatrix(float rad);



	// 4x4行列の表示
	void ShowMatrix(const char* label, const Matrix4x4& mat);

};
