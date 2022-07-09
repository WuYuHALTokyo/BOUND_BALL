//=============================================================================
//
// ボール処理 [ball.cpp]
// Author : 呉優
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "ball.h"
#include "field.h"
#include "block.h"
#include "boundPoint.h"
#include "bar.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
BALL g_Ball;		// ボール構造体

//=============================================================================
// ボールの初期化処理
//=============================================================================
void InitBall(void) {
	// ボールの初期位置
	g_Ball.x = 10;
	g_Ball.y = 10;
	// ボールの初期移動量
	g_Ball.ver_x = 1;
	g_Ball.ver_y = 1;
	// 初期火力
	g_Ball.power = 2;
}

//=============================================================================
// ボールの終了処理
//=============================================================================
void UninitBall(void) {

}

//=============================================================================
// ボールの更新処理
//=============================================================================
void UpdateBall(void) {

	// 元の場所をバックアップ
	g_Ball.x_old = g_Ball.x;
	g_Ball.y_old = g_Ball.y;
	// 移動処理
	g_Ball.x += g_Ball.ver_x;
	g_Ball.y += g_Ball.ver_y;

	/////////////////////////////////////////////////////////////////////////////
	// バウンド処理
	/////////////////////////////////////////////////////////////////////////////
	if (CheckField(g_Ball.x, g_Ball.y)) {
		// バウンド地点だったら、無効化して無視
		if (CheckField(g_Ball.x, g_Ball.y, BOUNDPOINT_DATA)) {
			GetBoundPoint()->use = 0;
		}
		else {
			// デッパリなのかチェック
			int isBump = 0;

			// 元の場所のx方向に物があれば
			if (CheckField(g_Ball.x, g_Ball.y_old)) {
				// ブロックだったら消す
				if (CheckField(g_Ball.x, g_Ball.y_old, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y_old, g_Ball.power);
				}
				// POWだったら消してパワーアップ
				else if (CheckField(g_Ball.x, g_Ball.y_old, POW_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y_old, g_Ball.power);
					DoPOW();
				}
				// バーだったらFPSを上げる
				else if (CheckField(g_Ball.x, g_Ball.y_old, BAR_DATA)) {
					FPSIncrease();
				}
				// 地面だったらFPSを下げる
				else if (CheckField(g_Ball.x, g_Ball.y_old, CEILING_DATA)) {
					FPSDecrease();
				}

				// 移動量のxを反転
				g_Ball.ver_x = -g_Ball.ver_x;
				// 場所を調整
				g_Ball.x = g_Ball.x_old + g_Ball.ver_x;
				isBump++;
			}

			// 元の場所のy方向に物があれば
			if (CheckField(g_Ball.x_old, g_Ball.y)) {
				// ブロックだったら消す
				if (CheckField(g_Ball.x_old, g_Ball.y, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x_old, g_Ball.y, g_Ball.power);
				}
				// POWだったら消してパワーアップ
				else if (CheckField(g_Ball.x_old, g_Ball.y, POW_DATA)) {
					DeleteBlock(g_Ball.x_old, g_Ball.y, g_Ball.power);
					DoPOW();
				}
				// バーだったらFPSを上げる
				else if (CheckField(g_Ball.x_old, g_Ball.y, BAR_DATA)) {
					FPSIncrease();
				}
				// 地面だったらFPSを下げる
				else if (CheckField(g_Ball.x_old, g_Ball.y, CEILING_DATA)) {
					FPSDecrease();
				}

				// 移動量のyを反転
				g_Ball.ver_y = -g_Ball.ver_y;
				// 場所を調整
				g_Ball.y = g_Ball.y_old + g_Ball.ver_y;
				isBump++;
			}

			// 両方実行された場合凹みの処理となる

			// デッパリだった場合
			if (isBump == 0) {
				// ブロックだったら消す
				if (CheckField(g_Ball.x, g_Ball.y, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y, g_Ball.power);
				}
				// POWだったら消してパワーアップ
				else if (CheckField(g_Ball.x, g_Ball.y, POW_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y, g_Ball.power);
					DoPOW();
				}
				// FPS変化無し

				// 移動量を反転
				g_Ball.ver_x = -g_Ball.ver_x;
				g_Ball.ver_y = -g_Ball.ver_y;
				// 場所を調整
				g_Ball.x = g_Ball.x_old + g_Ball.ver_x;
				g_Ball.y = g_Ball.y_old + g_Ball.ver_y;
			}
			// 着地点の再生成
			CreateBoundPoint();
		}
	}
}

//=============================================================================
// ボールの描画処理
//=============================================================================
void DrawBall(void) {
	SetField(g_Ball.x, g_Ball.y, BALL_DATA);
}

//=============================================================================
// ボールを取得
//=============================================================================
BALL* GetBall(void) {
	return &g_Ball;
}

//=============================================================================
// チャージショット
//=============================================================================
void Shot(void) {

}