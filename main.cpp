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


		for (int i = 0; i < 3; ++i) {
			Matrix4x4 scaleMatrix = MatrixMath::MakeScaleMatrix(scales[i]);
			Matrix4x4 rotateMatrix = MatrixMath::MakeRotateMatrix(rotates[i]);
			Matrix4x4 translateMatrix = MatrixMath::MakeTranslateMatrix(translates[i]);
			localMatrices[i] = MatrixMath::MultiplyM(MatrixMath::MultiplyM(scaleMatrix, rotateMatrix), translateMatrix);
			if (i == 0) {
				worldMatrices[i] = localMatrices[i];
			} else {
				worldMatrices[i] = MatrixMath::MultiplyM(worldMatrices[i - 1], localMatrices[i]);
			}
		}

		///																							
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///



		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);



		for (int i = 0; i < 3; ++i) {
			ImGui::PushID(i);
			ImGui::DragFloat3("Translate", &translates[i].x, 0.01f);
			ImGui::DragFloat3("Rotate", &rotates[i].x, 0.01f);
			ImGui::DragFloat3("Scale", &scales[i].x, 0.01f);
			ImGui::PopID();
		}


		ImGui::End();
		Matrix4x4 rotateMatrix = MatrixMath::MakeRotateMatrix(cameraRotate);
		Vector3 forward = MatrixMath::Transform({ 0, 0, 1 }, rotateMatrix);
		Vector3 target = {
			cameraTranslate.x + forward.x,
			cameraTranslate.y + forward.y,
			cameraTranslate.z + forward.z
		};
		Vector3 up = MatrixMath::Transform({ 0, 1, 0 }, rotateMatrix);

		Matrix4x4 viewMatrix = MatrixMath::MakeViewMatrix(cameraTranslate, target, up);

		// パース付き射影行列
		float fovY = 0.5f;
		float aspect = 1280.0f / 720.0f;
		float nearZ = 0.1f;
		float farZ = 100.0f;
		Matrix4x4 projectionMatrix = MatrixMath::MakePerspectiveMatrix(fovY, aspect, nearZ, farZ);

		// viewProjectionMatrix = view × projection
		Matrix4x4 viewProjectionMatrix = {};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				viewProjectionMatrix.m[i][j] = 0.0f;
				for (int k = 0; k < 4; ++k) {
					viewProjectionMatrix.m[i][j] += viewMatrix.m[i][k] * projectionMatrix.m[k][j];
				}
			}
		}

		// ビューポート行列（画面中心に変換）
		Matrix4x4 viewportMatrix = {
			640.0f, 0,       0, 0,
			0,    -360.0f,   0, 0,
			0,       0,      1, 0,
			640.0f, 360.0f,  0, 1
		};

		// 関節の色
		unsigned int colors[3] = { RED, GREEN, BLUE };

		// 関節のスクリーン座標を計算
		Vector3 jointPositions[3];
		for (int i = 0; i < 3; ++i) {
			Vector3 pos = MatrixMath::Transform({ 0.0f, 0.0f, 0.0f }, worldMatrices[i]);
			Vector3 screenPos = MatrixMath::Transform(MatrixMath::Transform(pos, viewProjectionMatrix), viewportMatrix);
			jointPositions[i] = screenPos;
			// 球体を描画
			Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), 10, 10, 0.0f, colors[i], kFillModeSolid);
		}

		// 関節を線で接続
		for (int i = 0; i < 2; ++i) {
			Novice::DrawLine(int(jointPositions[i].x), int(jointPositions[i].y),
				int(jointPositions[i + 1].x), int(jointPositions[i + 1].y),
				0xFFFFFFFF);
		}

		MatrixMath::DrawGrid(viewProjectionMatrix, viewportMatrix);


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
