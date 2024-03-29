//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 呉優
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// 
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// 入力の初期化処理
//=============================================================================
void InitInput(void) {
	rewind(stdin);						// 要る？
}

//=============================================================================
// 入力の終了処理
//=============================================================================
void UninitInput(void) {

}

//=============================================================================
// キーを取得
//=============================================================================
void GetKeyboardTrigger(char *key) {
	// キー入力がなければ飛ばす
	if (_kbhit() == 0) {
		return;
	}

	// キー入力処理
	*key = _getch();
	// 特殊キーならもう一度取得
	if ((*key == 0) || (*key == 224)) {
		*key = _getch();
	}
}