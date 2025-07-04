#include <Novice.h>
#include "Matrix.h"
#include "math.h"
#include <cmath>
#include <numbers>
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
			Vector3 d = {
				sphere.center.x + sphere.radius * cosf(nextLat) * cosf(nextLon),
				sphere.center.y + sphere.radius * sinf(nextLat),
				sphere.center.z + sphere.radius * cosf(nextLat) * sinf(nextLon)
			};

			// a, b, c, dをScreen座標系まで変換  
			a = Transform(a, MultiplyM(viewProjectionMatrix, viewportMatrix));
			b = Transform(b, MultiplyM(viewProjectionMatrix, viewportMatrix));
			c = Transform(c, MultiplyM(viewProjectionMatrix, viewportMatrix));
			d = Transform(d, MultiplyM(viewProjectionMatrix, viewportMatrix));

			// ab, ac, bd, cdで線を引く  
			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), color);
			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(c.x), static_cast<int>(c.y), color);
			Novice::DrawLine(static_cast<int>(b.x), static_cast<int>(b.y), static_cast<int>(d.x), static_cast<int>(d.y), color);
			Novice::DrawLine(static_cast<int>(c.x), static_cast<int>(c.y), static_cast<int>(d.x), static_cast<int>(d.y), color);
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

bool MatrixMath::IsCollision(const Segment& segment, const Plane& plane) {
	// 球の中心から平面までの距離を計算
  // 平面の方程式: normal・P = distance
  // ここでは球の中心ベクトルと平面法線ベクトルの内積から距離を求めている
	float distance = segment.diff.x * plane.normal.x +
		segment.diff.y * plane.normal.y +
		segment.diff.z * plane.normal.z - plane.distance;

	// 球の中心から平面までの距離の絶対値が
	// 球の半径以下なら衝突していると判断してtrueを返す
	return std::abs(distance) <= 0.0f;
}

void MatrixMath::DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// 平面の中心点（法線ベクトルに距離を掛けたもの）
	Vector3 center = MultiplyV(plane.distance, plane.normal);

	// 平面に垂直な2つの単位ベクトルを作成（平面上の軸）
    Vector3 u = MatrixMath::Normalize(MatrixMath::Perpendicular(plane.normal)); // 法線と垂直な任意のベクトル
	Vector3 v = MatrixMath::Normalize(Cross(plane.normal, u));                // uと法線に垂直なもう一つのベクトル

	float size = 2.0f; // 平面を描画する正方形の一辺の半分の長さ

	// 平面の四隅の座標を計算（正方形の4頂点）
	Vector3 corners[4] = {
		Add(center, Add(MultiplyV(size, u), MultiplyV(size, v))),   // +u +v方向の頂点
		Add(center, Add(MultiplyV(size, u), MultiplyV(-size, v))),  // +u -v方向の頂点
		Add(center, Add(MultiplyV(-size, u), MultiplyV(-size, v))), // -u -v方向の頂点
		Add(center, Add(MultiplyV(-size, u), MultiplyV(size, v)))   // -u +v方向の頂点
	};

	// 4つの頂点をスクリーン座標に変換して線で繋ぐ
	// viewProjectionMatrix と viewportMatrix をかけ合わせて変換行列を作成
	Matrix4x4 transform = MultiplyM(viewProjectionMatrix, viewportMatrix);

	for (int i = 0; i < 4; ++i) {
		Vector3 screen0 = Transform(corners[i], transform);                 // 頂点iを変換
		Vector3 screen1 = Transform(corners[(i + 1) % 4], transform);       // 次の頂点を変換
		Novice::DrawLine(
			static_cast<int>(screen0.x), static_cast<int>(screen0.y),       // 頂点iのスクリーン座標
			static_cast<int>(screen1.x), static_cast<int>(screen1.y),       // 頂点i+1のスクリーン座標
			color                                                           // 線の色
		);
	}
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
