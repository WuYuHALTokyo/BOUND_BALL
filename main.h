//=============================================================================
//
// メイン処理 [main.h]
// Author : 呉優
//
//=============================================================================
#pragma once

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum {
	MODE_TITLE,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX,
};

#define		DISP_H				(24)		// 表示画面の縦サイズ
#define		DISP_W				(80)		// 表示画面の横サイズ

#define		FPS_DEFAULT			(60)		// 初期FPS 
#define		FPS_MAX				(120)		// (理論)最大FPS
#define		FPS_MIN				(10)		// (理論)最小FPS
#define		FPS_TITLE			(10)		// タイトルのFPS

//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Init(void);
void Uninit(void);
void Update(void);
void Draw(void);

void SetMode(int mode);
int GetMode(void);

// FPSをセット
void SetFPS(int fps);

// FPSを取得
int GetFPS(void);

// FPS増減
void FPSIncrease(void);
void FPSDecrease(void);