#include <Novice.h>
#include "Matrix.h"
#include "math.h"
#include <cmath>
#include <numbers>
#include <array>
#include <algorithm> // clamp に必要

float pi = std::numbers::pi_v<float>;     // float版のπ


////加法
//Matrix4x4 MatrixMath::Add(const Matrix4x4& m1, const Matrix4x4& m2) {
//	Matrix4x4 result;
//	for (int row = 0; row < 4; row++) {
//		for (int col = 0; col < 4; col++) {
//			result.m[row][col] = m1.m[row][col] + m2.m[row][col];
//		}
//	}
//
//	return result;
//}
//
////減法
//Matrix4x4 MatrixMath::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
//	Matrix4x4 result;
//	for (int row = 0; row < 4; row++) {
//		for (int col = 0; col < 4; col++) {
//			result.m[row][col] = m1.m[row][col] - m2.m[row][col];
//		}
//	}
//
//	return result;
//}

Vector3	MatrixMath::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}



Vector3 MatrixMath::Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}



//1.透視投影行列
Matrix4x4 MatrixMath::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	float cot = (1.0f / tanf(fovY / 2.0f));
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f / aspectRatio * cot;
	result.m[1][1] = cot;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	return result;
}


//2.正射影行列
Matrix4x4 MatrixMath::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result = {};
	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1.0f;
	return result;
}


//3.ビューポート変換行列
Matrix4x4 MatrixMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {};
	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + width / 2.0f;
	result.m[3][1] = top + height / 2.0f;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;
	return result;

}


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
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;
	return result;
}


//Y軸回転行列
Matrix4x4 MatrixMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1.0f;
	return result;
}


//Z軸回転行列
Matrix4x4 MatrixMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {};
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}


//アフィン行列
Matrix4x4 MatrixMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;



	//回転行列を生成する
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = MultiplyM(rotateXMatrix, MultiplyM(rotateYMatrix, rotateZMatrix));


	result = {
		scale.x * rotateMatrix.m[0][0],scale.x * rotateMatrix.m[0][1],scale.x * rotateMatrix.m[0][2],0.0f,
		scale.y * rotateMatrix.m[1][0],scale.y * rotateMatrix.m[1][1],scale.y * rotateMatrix.m[1][2],0.0f,
		scale.z * rotateMatrix.m[2][0],scale.z * rotateMatrix.m[2][1],scale.z * rotateMatrix.m[2][2],0.0f,
		translate.x,translate.y,translate.z,1.0f
	};

	return result;
}

Matrix4x4 MatrixMath::MakeRotateMatrix(const Vector3& rotate) {
	float cosX = cosf(rotate.x), sinX = sinf(rotate.x);
	float cosY = cosf(rotate.y), sinY = sinf(rotate.y);
	float cosZ = cosf(rotate.z), sinZ = sinf(rotate.z);

	Matrix4x4 rotX = {
		1, 0,     0,    0,
		0, cosX,  sinX, 0,
		0, -sinX, cosX, 0,
		0, 0,     0,    1
	};
	Matrix4x4 rotY = {
		cosY, 0, -sinY, 0,
		0,    1, 0,     0,
		sinY, 0, cosY,  0,
		0,    0, 0,     1
	};
	Matrix4x4 rotZ = {
		cosZ, sinZ, 0, 0,
		-sinZ, cosZ, 0, 0,
		0,     0,    1, 0,
		0,     0,    0, 1
	};

	// rotZ * rotX * rotY
	Matrix4x4 result = MatrixMath::MultiplyM(MatrixMath::MultiplyM(rotZ, rotX), rotY);
	return result;
}

Matrix4x4 MatrixMath::MakeViewMatrix(const Vector3& eye, const Vector3& target, const Vector3& up) {
	Vector3 zAxis = Normalize({ target.x - eye.x, target.y - eye.y, target.z - eye.z });
	Vector3 xAxis = Normalize(Cross(up, zAxis));
	Vector3 yAxis = Cross(zAxis, xAxis);

	Matrix4x4 viewMatrix = {};
	viewMatrix.m[0][0] = xAxis.x;
	viewMatrix.m[1][0] = xAxis.y;
	viewMatrix.m[2][0] = xAxis.z;
	viewMatrix.m[3][0] = -(eye.x * xAxis.x + eye.y * xAxis.y + eye.z * xAxis.z);

	viewMatrix.m[0][1] = yAxis.x;
	viewMatrix.m[1][1] = yAxis.y;
	viewMatrix.m[2][1] = yAxis.z;
	viewMatrix.m[3][1] = -(eye.x * yAxis.x + eye.y * yAxis.y + eye.z * yAxis.z);

	viewMatrix.m[0][2] = zAxis.x;
	viewMatrix.m[1][2] = zAxis.y;
	viewMatrix.m[2][2] = zAxis.z;
	viewMatrix.m[3][2] = -(eye.x * zAxis.x + eye.y * zAxis.y + eye.z * zAxis.z);

	viewMatrix.m[0][3] = 0;
	viewMatrix.m[1][3] = 0;
	viewMatrix.m[2][3] = 0;
	viewMatrix.m[3][3] = 1;

	return viewMatrix;
}

Matrix4x4 MatrixMath::MakePerspectiveMatrix(float fovY, float aspect, float nearZ, float farZ) {

	float f = 1.0f / tanf(fovY / 2.0f);
	Matrix4x4 m{};
	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = farZ / (farZ - nearZ);
	m.m[3][2] = -nearZ * farZ / (farZ - nearZ);
	m.m[2][3] = 1.0f;
	m.m[3][3] = 0.0f;

	return m;
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


//積
Matrix4x4 MatrixMath::MultiplyM(const Matrix4x4& m1, const Matrix4x4& m2) {

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

Vector3 MatrixMath::MultiplyV(float scalar, const Vector3& v) {
	return {
	   v.x * scalar,
	   v.y * scalar,
	   v.z * scalar
	};
}


//クロス積
Vector3 MatrixMath::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x)
	};
	return result;
}

//内積
float MatrixMath::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


//逆行列
Matrix4x4 MatrixMath::Inverse(const Matrix4x4& m) {

	float aug[4][8] = {};
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			aug[row][col] = m.m[row][col];
		}
	}
	//単位行列を右に追加する
	aug[0][4] = 1.0f;
	aug[1][5] = 1.0f;
	aug[2][6] = 1.0f;
	aug[3][7] = 1.0f;



	for (int i = 0; i < 4; i++) {
		//ピボットが0の場合下の行と入れ替える
		if (aug[i][i] == 0.0f) {
			for (int j = i + 1; j < 4; j++) {
				if (aug[j][i] != 0.0f) {
					//行を交換する
					for (int k = 0; k < 8; k++) {//列
						float copyNum = aug[i][k];//元々ある上の行を代入
						aug[i][k] = aug[j][k];//上の行
						aug[j][k] = copyNum;//下の行
					}
					break;
				}
			}
		}


		//ピボットを1のする
		float pivot = aug[i][i];
		for (int k = 0; k < 8; k++) {
			aug[i][k] /= pivot;
		}



		//i列目のピボット以外を0にする
		for (int j = 0; j < 4; j++) {
			if (j != i) {
				float factor = aug[j][i];
				for (int k = 0; k < 8; k++) {
					aug[j][k] -= factor * aug[i][k];
				}
			}
		}
	}


	Matrix4x4 result = {};
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			result.m[row][col] = aug[row][col + 4];
		}
	}

	return result;
}


//座標変換
Vector3 MatrixMath::Transform(const Vector3& vector, const Matrix4x4& matrix) {
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

void MatrixMath::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 20;  // 分割数  
	const float kLonEvery = 2.0f * pi / kSubdivision;  // 経度分割1つ分の角度  
	const float kLatEvery = pi / kSubdivision;  // 緯度分割1つ分の角度  

	// 緯度の方向に分割 -π/2 ∼ π/2  
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;  // 現在の緯度  
		float nextLat = lat + kLatEvery;  // 次の緯度  

		// 経度の方向に分割 0 ～ 2π  
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;  // 現在の経度  
			float nextLon = lon + kLonEvery;  // 次の経度  

			// ワールド座標系でのa, b, c, dを求める  
			Vector3 a = {
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon)
			};
			Vector3 b = {
				sphere.center.x + sphere.radius * cosf(nextLat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(nextLat),
				sphere.center.z + sphere.radius * cosf(nextLat) * sinf(lon)
			};
			Vector3 c = {
				sphere.center.x + sphere.radius * cosf(lat) * cosf(nextLon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(nextLon)
			};
			

			// a, b, c, dをScreen座標系まで変換  
			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);
			

			// ab, ac, bd, cdで線を引く  
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x), int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x), int(screenC.y), color);
		}
	}
}

void MatrixMath::DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;                                       // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                        // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);  // 一つ分の長さ
	unsigned color = 0xAAAAAAFF; // 色を定義

	// 奥から手前への線を順々に引く
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHalfWidth + kGridEvery * xIndex;
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start = { x, 0.0f, -kGridHalfWidth };
		Vector3 end = { x, 0.0f, kGridHalfWidth };

		// スクリーン座標系まで変換をかける
		Vector3 ndcStart = Transform(start, MultiplyM(viewProjectionMatrix, viewportMatrix));
		Vector3 ndcEnd = Transform(end, MultiplyM(viewProjectionMatrix, viewportMatrix));

		// 変換した座標系を使って表示。
		Novice::DrawLine(
			static_cast<int>(ndcStart.x), static_cast<int>(ndcStart.y),
			static_cast<int>(ndcEnd.x), static_cast<int>(ndcEnd.y),
			color
		);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + kGridEvery * zIndex;
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start = { -kGridHalfWidth, 0.0f, z };
		Vector3 end = { kGridHalfWidth, 0.0f, z };

		// スクリーン座標系まで変換をかける
		Vector3 ndcStart = Transform(start, MultiplyM(viewProjectionMatrix, viewportMatrix));
		Vector3 ndcEnd = Transform(end, MultiplyM(viewProjectionMatrix, viewportMatrix));

		// 変換した座標系を使って表示。
		Novice::DrawLine(
			static_cast<int>(ndcStart.x), static_cast<int>(ndcStart.y),
			static_cast<int>(ndcEnd.x), static_cast<int>(ndcEnd.y),
			color // 定義した色を使用
		);


	}
}

Matrix4x4 MatrixMath::MakeIdentity() {
	Matrix4x4 result{};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}

	return result;
}

Vector3 MatrixMath::Project(const Vector3& v1, const Vector3& v2) {
	// v2への正規化を使ったv1の射影
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	float v2LengthSq = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;
	float scale = dot / v2LengthSq;

	return {
		v2.x * scale,
		v2.y * scale,
		v2.z * scale
	};
}

//長さ(ノルム)
float MatrixMath::Length(const Vector3& v) {
	float result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
}

//正規化
Vector3 MatrixMath::Normalize(const Vector3& v) {
	Vector3 result;
	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);
	return result;
}

bool MatrixMath::IsCollisionAABB(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;

	}
	return false;
}

void MatrixMath::DrawAABB(
	const AABB& aabb, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color) {
	// 1. AABBを構成する8頂点をmin/maxから作成
	Vector3 vertices[8] = {
		{aabb.min.x, aabb.min.y, aabb.min.z}, // 0: 左下奥
		{aabb.max.x, aabb.min.y, aabb.min.z}, // 1: 右下奥
		{aabb.max.x, aabb.max.y, aabb.min.z}, // 2: 右上奥
		{aabb.min.x, aabb.max.y, aabb.min.z}, // 3: 左上奥
		{aabb.min.x, aabb.min.y, aabb.max.z}, // 4: 左下手前
		{aabb.max.x, aabb.min.y, aabb.max.z}, // 5: 右下手前
		{aabb.max.x, aabb.max.y, aabb.max.z}, // 6: 右上手前
		{aabb.min.x, aabb.max.y, aabb.max.z}  // 7: 左上手前
	};

	// 2. 頂点を結ぶ辺（線）のインデックス（12本）
	int indices[12][2] = {
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, // 奥の面
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, // 手前の面
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // 側面の辺
	};

	// 3. 各辺を変換して描画（viewProjection → viewport）
	for (int i = 0; i < 12; ++i) {
		Vector3 start = Transform(Transform(vertices[indices[i][0]], viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(vertices[indices[i][1]], viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(
			static_cast<int>(start.x), static_cast<int>(start.y),
			static_cast<int>(end.x), static_cast<int>(end.y),
			color
		);
	}
}

bool MatrixMath::IsCollisionSphereAABB(const AABB& aabb, const Sphere& sphere) {

	// 最近接点を求める
	Vector3 closestPoint;
	closestPoint.x = std::clamp(sphere.center.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(sphere.center.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(sphere.center.z, aabb.min.z, aabb.max.z);

	// 球の中心との距離を求める
	Vector3 diff = Subtract(closestPoint, sphere.center);
	float distance = Length(diff);

	// 距離が半径以下なら衝突
	return distance <= sphere.radius;

}

bool MatrixMath::IsIntersectAABBAndSegment(const AABB& box, const segment& seg) {
	Vector3 dir = {
		seg.end.x - seg.start.x,
		seg.end.y - seg.start.y,
		seg.end.z - seg.start.z
	};

	float tMin = 0.0f;
	float tMax = 1.0f;

	for (int i = 0; i < 3; ++i) {
		float start = (&seg.start.x)[i];
		float d = (&dir.x)[i];
		float minB = (&box.min.x)[i];
		float maxB = (&box.max.x)[i];

		if (std::abs(d) < 1e-6f) {
			if (start < minB || start > maxB) return false;
		} else {
			float ood = 1.0f / d;
			float t1 = (minB - start) * ood;
			float t2 = (maxB - start) * ood;
			if (t1 > t2) std::swap(t1, t2);
			tMin = std::max<float>(tMin, t1);
			tMax = std::min<float>(tMax, t2);
			if (tMin > tMax) return false;
		}
	}
	return true;
}

void MatrixMath::DrawSegment(const segment& seg, const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color) {
	Vector3 screenStart = Transform(Transform(seg.start, viewProjection), viewport);
	Vector3 screenEnd = Transform(Transform(seg.end, viewProjection), viewport);
	Novice::DrawLine((int)screenStart.x, (int)screenStart.y, (int)screenEnd.x, (int)screenEnd.y, color);
}




Vector3 MatrixMath::Lerp(const Vector3& a, const Vector3& b, float t) {

	return{
	   a.x + (b.x - a.x) * t,
	   a.y + (b.y - a.y) * t,
	   a.z + (b.z - a.z) * t
	};

}

Vector3 MatrixMath::QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {

	Vector3 a = Lerp(p0, p1, t);
	Vector3 b = Lerp(p1, p2, t);
	return Lerp(a, b, t);
}

void MatrixMath::DrawBezierCurve(const Vector3 controlPoints[3], const Matrix4x4& viewProjection, const Matrix4x4& viewport, uint32_t color) {

	const int kSegments = 100; // 分割数
	for (int i = 0; i < kSegments; ++i) {
		float t0 = static_cast<float>(i) / kSegments;
		float t1 = static_cast<float>(i + 1) / kSegments;

		Vector3 p0 = QuadraticBezier(controlPoints[0], controlPoints[1], controlPoints[2], t0);
		Vector3 p1 = QuadraticBezier(controlPoints[0], controlPoints[1], controlPoints[2], t1);
		// ビュー変換とビューポート変換を適用
		Vector3 screenP0 = Transform(Transform(p0, viewProjection), viewport);
		Vector3 screenP1 = Transform(Transform(p1, viewProjection), viewport);
		// 線を描画
		Novice::DrawLine(int(screenP0.x), int(screenP0.y), int(screenP1.x), int(screenP1.y), color);

	}

}




Vector3 MatrixMath::ClosestPoint(const Vector3& point, const Segment& segment) {
	// 線分の終点を計算
	Vector3 end = {
		segment.origin.x + segment.diff.x,
		segment.origin.y + segment.diff.y,
		segment.origin.z + segment.diff.z
	};

	// 線分の方向ベクトル
	Vector3 segVec = {
		end.x - segment.origin.x,
		end.y - segment.origin.y,
		end.z - segment.origin.z
	};

	// 点から線分の始点へのベクトル
	Vector3 toPoint = {
		point.x - segment.origin.x,
		point.y - segment.origin.y,
		point.z - segment.origin.z
	};

	float dot = toPoint.x * segVec.x + toPoint.y * segVec.y + toPoint.z * segVec.z;
	float lengthSq = segVec.x * segVec.x + segVec.y * segVec.y + segVec.z * segVec.z;

	float t = dot / lengthSq;
	t = std::clamp(t, 0.0f, 1.0f); // 線分上にクランプ

	return {
		segment.origin.x + segVec.x * t,
		segment.origin.y + segVec.y * t,
		segment.origin.z + segVec.z * t
	};
}

bool MatrixMath::IsCollisionP(const Segment& segment, const Plane& plane) {
	// 線分の始点と終点を取得
	Vector3 start = segment.origin;                  // 線分の始点
	Vector3 end = Add(segment.origin, segment.diff);     // 線分の終点（始点 + 向きベクトル）

	// 始点と終点から平面までの距離（符号付き）を計算
	// 平面の方程式: normal・P = distance
	// ここでは、点と法線の内積 - 平面の距離 で符号付き距離を求めている
	float startDist = MatrixMath::Dot(start, plane.normal) - plane.distance; // 始点から平面までの距離
	float endDist = MatrixMath::Dot(end, plane.normal) - plane.distance;   // 終点から平面までの距離

	// 始点と終点が平面の両側にあれば、線分は平面と交差している
	// 例：d0 < 0 かつ d1 > 0、またはその逆、またはどちらかが0（ちょうど接している）
	return startDist * endDist <= 0.0f;
}


void MatrixMath::DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// 平面の中心点（法線ベクトルに距離を掛けたもの）
	Vector3 center = MultiplyV(plane.distance, plane.normal);

	// 平面に垂直な2つの単位ベクトルを作成（平面上の軸）
	Vector3 u = MatrixMath::Normalize(MatrixMath::Perpendicular(plane.normal)); // 法線と垂直な任意のベクトル
	Vector3 v = MatrixMath::Normalize(Cross(plane.normal, u));                // uと法線に垂直なもう一つのベクトル

	float size = 2.0f; // 平面を描画する正方形の一辺の半分の長さ

	// 平面の四隅の座標を計算（正方形の4頂点）
	Vector3 corners[3] = {
		Add(center, Add(MultiplyV(size, u), MultiplyV(size, v))),   // +u +v方向の頂点
		Add(center, Add(MultiplyV(size, u), MultiplyV(-size, v))),  // +u -v方向の頂点
		Add(center, Add(MultiplyV(-size, u), MultiplyV(-size, v))), // -u -v方向の頂点
	};

	// 4つの頂点をスクリーン座標に変換して線で繋ぐ
	// viewProjectionMatrix と viewportMatrix をかけ合わせて変換行列を作成
	Matrix4x4 transform = MultiplyM(viewProjectionMatrix, viewportMatrix);

	for (int i = 0; i < 3; ++i) {
		Vector3 screen0 = Transform(corners[i], transform);                 // 頂点iを変換
		Vector3 screen1 = Transform(corners[(i + 1) % 3], transform);       // 次の頂点を変換
		Novice::DrawLine(
			static_cast<int>(screen0.x), static_cast<int>(screen0.y),       // 頂点iのスクリーン座標
			static_cast<int>(screen1.x), static_cast<int>(screen1.y),       // 頂点i+1のスクリーン座標
			color                                                           // 線の色
		);
	}
}



void MatrixMath::DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 screenV[3];
	for (int i = 0; i < 3; ++i) {
		screenV[i] = MatrixMath::Transform(MatrixMath::Transform(triangle.vertex[i], viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawTriangle(
		(int)screenV[0].x, (int)screenV[0].y,
		(int)screenV[1].x, (int)screenV[1].y,
		(int)screenV[2].x, (int)screenV[2].y,
		color, kFillModeWireFrame
	);
}

//  ---      ---- //
bool MatrixMath::IsCollisionT(const Triangle& triangle, const Segment& segment) {
	// 三角形の法線を求める
	Vector3 v01 = {
		triangle.vertex[1].x - triangle.vertex[0].x,
		triangle.vertex[1].y - triangle.vertex[0].y,
		triangle.vertex[1].z - triangle.vertex[0].z
	};
	Vector3 v02 = {
		triangle.vertex[2].x - triangle.vertex[0].x,
		triangle.vertex[2].y - triangle.vertex[0].y,
		triangle.vertex[2].z - triangle.vertex[0].z
	};

	Vector3 normal = Normalize(Cross(v01, v02));

	// 三角形を含む平面
	float d = triangle.vertex[0].x * normal.x + triangle.vertex[0].y * normal.y + triangle.vertex[0].z * normal.z;

	// 始点と終点
	Vector3 p0 = segment.origin;
	Vector3 p1 = {
		segment.origin.x + segment.diff.x,
		segment.origin.y + segment.diff.y,
		segment.origin.z + segment.diff.z
	};

	// 始点終点の平面からの距離
	float dot0 = p0.x * normal.x + p0.y * normal.y + p0.z * normal.z - d;
	float dot1 = p1.x * normal.x + p1.y * normal.y + p1.z * normal.z - d;

	if (dot0 * dot1 > 0.0f) return false; // 平面と交差していない

	// 衝突点を線形補間で求める
	float t = dot0 / (dot0 - dot1);
	Vector3 p = {
		p0.x + (p1.x - p0.x) * t,
		p0.y + (p1.y - p0.y) * t,
		p0.z + (p1.z - p0.z) * t,
	};

	// バリツェントリック法で三角形内にあるかをチェック
	Vector3 v0 = {
		triangle.vertex[1].x - triangle.vertex[0].x,
		triangle.vertex[1].y - triangle.vertex[0].y,
		triangle.vertex[1].z - triangle.vertex[0].z,
	};
	Vector3 v1 = {
		triangle.vertex[2].x - triangle.vertex[0].x,
		triangle.vertex[2].y - triangle.vertex[0].y,
		triangle.vertex[2].z - triangle.vertex[0].z,
	};
	Vector3 v2 = {
		p.x - triangle.vertex[0].x,
		p.y - triangle.vertex[0].y,
		p.z - triangle.vertex[0].z,
	};

	float d00 = Dot(v0, v0);
	float d01 = Dot(v0, v1);
	float d11 = Dot(v1, v1);
	float d20 = Dot(v2, v0);
	float d21 = Dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	if (denom == 0.0f) return false;

	float u = (d11 * d20 - d01 * d21) / denom;
	float v = (d00 * d21 - d01 * d20) / denom;

	return (u >= 0.0f && v >= 0.0f && u + v <= 1.0f);
}

Vector3 MatrixMath::Perpendicular(const Vector3& vector) {
	// vectorと直交する適当なベクトルを返す
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}



//bool matrixmath::iscollision(const sphere& s1, const sphere& s2, unsigned int& color) {
//
//	// 2つの中心の差分ベクトル
//	vector3 diff = matrixmath::subtract(s1.center, s2.center);
//
//	// 距離の2乗を計算（平方根を使わない高速バージョン）
//	float distancesq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
//
//	// 半径の合計
//	float radiussum = s1.radius + s2.radius;
//
//	if (distancesq <= (radiussum * radiussum)) {
//
//		color = red; // 赤に変更
//		
//	} else {
//
//		color = black;
//
//	}
//	return color;
//}
