#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"
#include "Player.h"
#include <cstdint>
#include <imgui.h>
#include <cmath>
#include <algorithm>


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
	//int kWindowWidth = 1280;
	//int kWindowHeight = 720;


	// 各関節のワールド座標を計算
	Matrix4x4 localMatrices[3];
	Matrix4x4 worldMatrices[3];


	Vector3 cameraPosition = { 0.0f,0.0f,-1.0f };

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Sphere spherePlayer = { {0.0f, 1.0f, 0.0f}, 1.0f }; // 中心が(0,1,0)、半径1.0の球
	//Sphere sphereEnemy  = { {0.5f, 1.0f, 0.0f}, 1.0f }; // 中心が(0,1,0)、半径1.5の球

	Plane plane = { {0.0f, 1.0f, 0.0f}, 1.0f };

	segment segment = { {0.0f, 1.0f, -1.0f}, {0.0f, -2.0f, 2.0f} };

	Vector3 point{ -1.5f,0.6f,0.6f };


	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};


	Vector3 rotates[3] = {
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};

	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};


	/*AABB aabb1{
		.min{-0.5f,-0.5f,-0.5f},
		.max{0.5f,0.5f,0.5f}
	};*/

	static AABB box = { {-0.5f, 0.0f, -0.5f}, {0.5f, 1.0f, 0.5f} };


	Triangle triangle = {
			{{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};





	// 球
	Sphere sphereA = { {0.0f, 1.0f, 0.0f}, 1.0f };

	// グローバル変数
	Vector3 controlPoints[3] = {
		{-0.8f, 0.58f, 1.0f},
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.3f},
	};

	//unsigned int color = BLACK;

	//pointを線分に射影したベクトル。今回は正しく計算できているかを確認するためだけに使う
	//Vector3 closestPoint = ClosestPoint(point, segment);


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



		ImGui::NewFrame();

		Vector3 a{ 0.2f, 1.0f, 0.0f };
		Vector3 b{ 2.4f, 3.1f, 1.2f };
		Vector3 c = a + b;
		Vector3 d = a - b;
		Vector3 e = a * 2.4f;

		Vector3 rotate{ -0.4f, -1.43f, 0.8f }; // 正確な回転角（ラジアン）
		Matrix4x4 rotX = MatrixMath::MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotY = MatrixMath::MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotZ = MatrixMath::MakeRotateZMatrix(rotate.z);
		Matrix4x4 rot = rotX * rotY * rotZ;

		///																							
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///



		ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);
		ImGui::Begin("Window");
		ImGui::Text("c: %.6f, %.6f, %.6f", c.x, c.y, c.z);
		ImGui::Text("d: %.6f, %.6f, %.6f", d.x, d.y, d.z);
		ImGui::Text("e: %.6f, %.6f, %.6f", e.x, e.y, e.z);
		MatrixMath::ShowMatrix("rotateMatrix:", rot);
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
