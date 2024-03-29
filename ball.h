//=============================================================================
//
// ボール処理 [ball.h]
// Author : 呉優
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		BALL_MAX		(1)			// 最大ボール数
#define		BALL_DATA		('o')
#define		POW_MAX			(10)		// パワー上限

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct BALL {
	int x ,y;				// 座標
	int x_old, y_old;		// 前の座標
	int ver_x, ver_y;		// 移動量

	int power;				// 威力
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBall(void);
void UninitBall(void);
void UpdateBall(void);
void DrawBall(void);

BALL* GetBall(void);

// チャージショット
void Shot(void);