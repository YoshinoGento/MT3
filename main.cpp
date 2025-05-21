#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"
#include "Player.h"
#include <cstdint>

const char kWindowTitle[] = "LE2C_28_ヨシノゲント_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matirix, const char* label) {
	Novice::ScreenPrintf(x, y + 20, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight + 20,
				"%6.02f", matirix.m[row][column]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	int kWindowWidth = 1280;
	int kWindowHeight = 720;

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = MatrixMath::Cross(v1, v2);


	Vector3 rotate = { 0.0f,0.0f,0.0f };
	Vector3 translate{};
	Vector3 cameraPosition = { 0.0f,0.0f,-1.0f };
	/*Player player ;*/

	//スクリーン
	/*Vector3 screenVertices[3] = {};*/


	//三角形の３点

	Vector3 kLocalVertices[3] = {
	{ 0.0f, 0.1f, 0.0f },
	{ -0.1f, -0.1f, 0.0f },
	{ 0.1f, -0.1f, 0.0f }
	};




	Player player;

	player.speed_ = 0.01f;



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		if (keys[DIK_W]) {
			translate.z -= player.speed_;
		}
		if (keys[DIK_S]) {
			translate.z += player.speed_;
		}
		if (keys[DIK_A]) {
			translate.x -= player.speed_;
		}
		if (keys[DIK_D]) {
			translate.x += player.speed_;
		}


		rotate.y -= 0.1f;

		//Matrix4x4 rotateMatrix = MatrixMath::MakeRotateYMatrix(rotate.y);

		//rotate = MatrixMath::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);


		//各種行列の計算
		Matrix4x4 worldMatrix = MatrixMath::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);

		Matrix4x4 cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPosition);

		Matrix4x4 viewMatrix = MatrixMath::Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, MatrixMath::Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MatrixMath::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		//スクリーン
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {

			Vector3 ndcVertex = MatrixMath::Transform(kLocalVertices[i], worldViewProjectionMatrix);

			screenVertices[i] = MatrixMath::Transform(ndcVertex, viewportMatrix);

		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y),
			int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y),
			RED, kFillModeSolid);

		for (int i = 0; i < 3; ++i) {

			Novice::ScreenPrintf(i, i * 15, "screenVertices:x = %f,y = %f,z = %f", screenVertices[i].x, screenVertices[i].y, screenVertices[i].z);
		}
		//VectorScreenPrintf(0, 0, cross, "Cross");
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
