#include <Novice.h>
#include "Struct.h"
#include "Matrix.h"

const char kWindowTitle[] = "LE2C_28_ヨシノゲント_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;


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
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.4f,1.43f,-0.8f };

	Matrix4x4 rotateXMatrix = MatrixMath::MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MatrixMath::MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MatrixMath::MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYMatrix = MatrixMath::Multiply(rotateXMatrix, MatrixMath::Multiply(rotateYMatrix, rotateZMatrix));

	

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



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYMatrix, "rotateXYMatrix");
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
