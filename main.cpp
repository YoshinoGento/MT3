#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"
#include "Player.h"
#include <cstdint>
#include <imgui.h>

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



	
	Vector3 cameraPosition = { 0.0f,0.0f,-1.0f };
	
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,-1.0f };

	Sphere sphere = { {0.0f, 1.0f, 0.0f}, 1.5f }; // 中心が(0,1,0)、半径1.5の球



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


		
		//各種行列の計算
		

		/*Matrix4x4 cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = MatrixMath::Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(cameraMatrix, MatrixMath::Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MatrixMath::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);*/


		// ビュー行列（カメラ位置と向き）
		Matrix4x4 cameraMatrix = MatrixMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = MatrixMath::Inverse(cameraMatrix);

		// プロジェクション行列
		Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveFovMatrix(
			0.45f,
			float(kWindowWidth) / float(kWindowHeight),
			0.1f,
			100.0f
		);

		// ワールド行列（グリッドや球の位置）
		Matrix4x4 worldMatrix = MatrixMath::MakeIdentity();

		// 各種行列の合成
		Matrix4x4 viewProjectionMatrix = MatrixMath::Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 worldViewProjectionMatrix = MatrixMath::Multiply(worldMatrix, viewProjectionMatrix);

		// ビューポート行列
		Matrix4x4 viewportMatrix = MatrixMath::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// 描画
		MatrixMath::DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		MatrixMath::DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, BLACK);

		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixMath::DrawGrid(worldViewProjectionMatrix, viewportMatrix);
		MatrixMath::DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, BLACK);


		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f); // ←中心座標
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);     // ←半径
		ImGui::End();
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
