#include <Novice.h>
#include "Matrix.h"
#include "math.h"
#include <cmath>
#include <numbers>

float pi = std::numbers::pi_v<float>;     // float版のπ


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

	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));


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
			a = Transform(a, Multiply(viewProjectionMatrix, viewportMatrix));
			b = Transform(b, Multiply(viewProjectionMatrix, viewportMatrix));
			c = Transform(c, Multiply(viewProjectionMatrix, viewportMatrix));
			d = Transform(d, Multiply(viewProjectionMatrix, viewportMatrix));

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
		Vector3 ndcStart = Transform(start, Multiply(viewProjectionMatrix, viewportMatrix));
		Vector3 ndcEnd = Transform(end, Multiply(viewProjectionMatrix, viewportMatrix));

		// 変換した座標系を使って表示。
		Novice::DrawLine(
			static_cast<int>(ndcStart.x), static_cast<int>(ndcStart.y),
			static_cast<int>(ndcEnd.x), static_cast<int>(ndcEnd.y),
			color
		);
		if (xIndex == 4) {
			color = BLACK;
		} else {
			color = 0xAAAAAAFF;
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + kGridEvery * zIndex;
		// 上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 start = { -kGridHalfWidth, 0.0f, z };
		Vector3 end = { kGridHalfWidth, 0.0f, z };

		// スクリーン座標系まで変換をかける
		Vector3 ndcStart = Transform(start, Multiply(viewProjectionMatrix, viewportMatrix));
		Vector3 ndcEnd = Transform(end, Multiply(viewProjectionMatrix, viewportMatrix));

		// 変換した座標系を使って表示。
		Novice::DrawLine(
			static_cast<int>(ndcStart.x), static_cast<int>(ndcStart.y),
			static_cast<int>(ndcEnd.x), static_cast<int>(ndcEnd.y),
			color // 定義した色を使用
		);

		if (zIndex == 4) {
			color = BLACK;
		} else {
			color = 0xAAAAAAFF;
		}
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
