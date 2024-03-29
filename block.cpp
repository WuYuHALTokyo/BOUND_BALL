//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : 呉優
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"
#include "field.h"
#include "ball.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		EMPTY_LINE			('!')		// 未使用ブロックデータタグ。先頭に付いてれば未使用
#define		EMPTY_INDEX			(-1)		// 未使用番号

#define		BLOCK_ORIGIN_X		(1)			// ブロックの基本配置場所(壁の内側の一番左上)
#define		BLOCK_ORIGIN_Y		(1)

#define		BLOCK_SLOT			(20)		// ブロックパターンのバリエーション上限数
#define		BLOCK_MAX			(50)		// ブロックの最大保存行数
#define		BLOCK_START			(5)			// ゲームスタート時のブロック行数

#define		BOMB_PROP			(75)		// 爆弾の生成確率
#define		BOMB_DOUBLE_PROP	(25)		// ダブル爆弾の生成確率
#define		BOMB_POWER			(2)			// 一度に爆破する行数

#define		POW_PROP			(20)		// POWの生成確率

#define		WAVE_TIME			(120)		// ブロック生成の間隔

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
// ブロックパターン。横を-1してるのは、壁を除いて-2、NULLが入って+1の合計
char block_org[BLOCK_SLOT][FIELD_W - 1] = {		
	"##########################################################",		// デフォルトパターン
};

// ブロックデータ
char block[BLOCK_MAX][FIELD_W - 1]{
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
	"                                                          ",
};

char block_empty[FIELD_W - 1] = "                                                          ";

int g_BlockIndex[BLOCK_MAX];					// ブロックの番号リスト
int g_TopIndex;									// リストの先頭へのポインタ

int g_ActiveLine ;								// 稼働中のブロックの行数

int waveCount;									// ウェーブのカウンタ

//=============================================================================
// ブロックの初期化処理
//=============================================================================
void InitBlock(void) {

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i][0] = EMPTY_LINE;			// ブロックデータにすべて未使用タグをつける
		g_BlockIndex[i] = EMPTY_INDEX;		// 番号をすべてEMPTYに
	}
	g_TopIndex = EMPTY_INDEX;
	g_ActiveLine = 0;
	waveCount = 0;

	// 初期ブロックをセットし、番号を振る
	for (int i = 0; i < BLOCK_START; i++)
		AddBlockLine();
}

//=============================================================================
// ブロックの終了処理
//=============================================================================
void UninitBlock(void) {

}

//=============================================================================
// ブロックの更新処理
//=============================================================================
void UpdateBlock(void) {

	//ウェーブ制御
	//時間ごとにブロックが増える
	waveCount++;
	
	if (waveCount % WAVE_TIME == 0)
		AddBlockLine();

	// 空っぽになった先頭ブロックデータがあれば解放
	if (strcmp(block[g_TopIndex], block_empty) == 0)
		ClearTopLine();

	ReleaseDownLine();
}

//=============================================================================
// ブロックの描画処理
//=============================================================================
void DrawBlock(void) {
	///////////////////////////////////////////////////////////////////////////
	// リスト順に描画
	///////////////////////////////////////////////////////////////////////////
	int cur = g_TopIndex;

	if (cur >= 0 && cur <= BLOCK_MAX)									// オーバーフロー防止
		for (int i = 0; i < g_ActiveLine; i++) {						// 稼働中行数のブロックしか描画しない
			SetField(BLOCK_ORIGIN_X, g_ActiveLine - i, block[cur]);		// 床側から天井へ描画
			cur = g_BlockIndex[cur];									// 次の番号を取得
		}
}

//=============================================================================
// ブロックを1行追加
// 0628: ブロックが枠を突き破るのと関係ない
//=============================================================================
void AddBlockLine(void) {

	// 空っぽのブロックデータを検索
	int i = 0;
	while (block[i][0] != EMPTY_LINE)
		i++;

	// 新しいブロックデータを格納
	memcpy(block[i], block_org[0], sizeof(block_org[0]));

	// 爆弾生成
	CreateBomb(i);

	// POW生成
	CreatePOW(i);

	///////////////////////////////////////////////////////////////////////////
	// リスト管理
	// 0628: 未デバッグ
	///////////////////////////////////////////////////////////////////////////
	// 番号検索用
	int cur = g_TopIndex;

	// リストがすべて未使用だった場合
	if (g_TopIndex == EMPTY_INDEX) {
		// 先頭要素を指定
		g_BlockIndex[i] = g_TopIndex;
		g_TopIndex = i;
	}

	// リストが一部未使用じゃなかった場合
	else {
		// 末尾要素を検索
		while (g_BlockIndex[cur] != EMPTY_INDEX) {
			cur = g_BlockIndex[cur];

			// オーバーフロー防止
			if (cur >= BLOCK_MAX) {
				return;
			}
		}
		// 格納した要素へポイント
		g_BlockIndex[i] = g_BlockIndex[cur];
		g_BlockIndex[cur] = i;
	}

	// 稼働中行数+1
	g_ActiveLine++;
}

//=============================================================================
// ブロックを消す
// フィールド座標(x, y)を中心に、半径power以内のブロックを削除ォ(魅上照)
// 0628: 枠を突き破る元凶。FIELD_W-1をFIELD_W-2にしたらすべてが直った
//=============================================================================
void DeleteBlock(int x, int y, int power) {

	// ボール座標をブロック座標へ変換
  	int x_block = x - BLOCK_ORIGIN_X;

	// 中心点のブロックの所属番号
	int idx = g_ActiveLine - y;

	//////////////////////////////////////////////////////////////////////////////
	// 消去処理
	//////////////////////////////////////////////////////////////////////////////
	int cur = g_TopIndex;									// 番号を追う用

	/*最も下にある行から中央の下1行目まで*/
	if (idx - power <= 0) {		// ブロックデータの下を超える或いはちょうどな場合

		// 縦に削除
		for (int i = 0; i < idx; i++) {

			// 横に削除
			for (int j = 0; j < i + 1; j++) {
				if (x_block + j < FIELD_W - 2) {	// オーバーフロー防止(FIELD_W-2しているのは、壁を除いて)
					// 爆弾だったら爆発
					if (block[cur][x_block + j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// ブロックを消す
					block[cur][x_block + j] = ' ';
				}
				if (x_block - j >= 0) {		// オーバーフロー防止
					// 爆弾だったら爆発
					if (block[cur][x_block - j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// ブロックを消す
					block[cur][x_block - j] = ' ';
				}

				// オーバーフロー防止
				if (cur > BLOCK_MAX) {
					return;
				}
			}
			cur = g_BlockIndex[cur];
		}
	}
	else {		// ブロックデータの下を超えない場合
		
		// 削除範囲内で最も下の行を取得
		for (int i = 0; i < idx - power; i++) {
			// オーバーフロー防止
			if (cur > BLOCK_MAX) {
				return;
			}
			cur = g_BlockIndex[cur];
		}

		// 縦に消す
		for (int i = 0; i < power; i++) {		

			// 横に消す
			for (int j = 0; j < i + 1; j++) {
				if (x_block + j < FIELD_W - 2) {	// オーバーフロー防止
					// 爆弾だったら爆発
					if (block[cur][x_block + j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// ブロックを消す
					block[cur][x_block + j] = ' ';
				}
				if (x_block - j >= 0) {		// オーバーフロー防止
					// 爆弾だったら爆発
					if (block[cur][x_block - j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// 横に消す
					block[cur][x_block - j] = ' ';
				}

				// オーバーフロー防止
				if (cur > BLOCK_MAX) {
					return;
				}

			}
			cur = g_BlockIndex[cur];
		}
	}

	/*中央の行*/
	// 横に消す
	for (int j = 0; j < (power + 1); j++) {		// +1しているのは、横にpower*2+1分消すため
		if (x_block + j < FIELD_W - 2) {	// オーバーフロー防止
			// 爆弾だったら爆発
			if (block[cur][x_block + j] == BOMB_DATA) {
				DoBomb(cur);
				continue;
			}
			// ブロックを消す
			block[cur][x_block + j] = ' ';
		}
		if (x_block - j >= 0) {		// オーバーフロー防止
			// 爆弾だったら爆発
			if (block[cur][x_block - j] == BOMB_DATA) {
				DoBomb(cur);
				continue;
			}
			// ブロックを消す
			block[cur][x_block - j] = ' ';
		}

		// オーバーフロー防止
		if (cur > BLOCK_MAX) {
			return;
		}

	}
	cur = g_BlockIndex[cur];

	/*中央の上1行目から最も上にある行まで*/
	// 縦に消す
	for (int i = 0; i < power; i++) {

		if (cur == EMPTY_INDEX)		// 空っぽのブロックデータに触れようとすると終了
			break;

		// 横に消す
		for (int j = 0; j < (power - i); j++) {
			if (x_block + j < FIELD_W - 2) {	// オーバーフロー防止
				// 爆弾だったら爆発
				if (block[cur][x_block + j] == BOMB_DATA) {
					DoBomb(cur);
					continue;
				}
				// ブロックを消す
				block[cur][x_block + j] = ' ';
			}
			if (x_block - j >= 0) {		// オーバーフロー防止
				// 爆弾だったら爆発
				if (block[cur][x_block - j] == BOMB_DATA) {
					DoBomb(cur);
					continue;
				}
				//ブロックを消す
				block[cur][x_block - j] = ' ';
			}

			// オーバーフロー防止
			if (cur > BLOCK_MAX) {
				return;
			}

		}
		cur = g_BlockIndex[cur];
	}
}

//=============================================================================
// 先頭ブロックデータを解放
//=============================================================================
void ClearTopLine(void) {
	int cur = g_BlockIndex[g_TopIndex];						// 番号をリストから除外
	g_TopIndex = cur;
	g_ActiveLine--;											// 稼働中行数-1
}

//=============================================================================
// 限界まで下りたブロックを解放、爆弾が含まれていたらゲームオーバー
//=============================================================================
void ReleaseDownLine(void) {

	if (g_ActiveLine >= FIELD_H - DEAD_LINE) {

		// 爆弾があるかチェック
		for (int i = 0; i < FIELD_W - 1; i++) {
			if (block[g_TopIndex][i] == BOMB_DATA) {
				SetMode(MODE_RESULT);
				return;
			}
		}

		// 先頭ブロックを削除
		ClearTopLine();
	}
}

//=============================================================================
// 爆弾ブロック生成
//=============================================================================
void CreateBomb(int idx) {
	// 生成確率以内なら
	if (rand() % 100 < BOMB_PROP) {
		// ボムをidx番データのランダムな位置に生成
		int x = rand() % (FIELD_W - 2);
		block[idx][x] = BOMB_DATA;

		// ダブル爆弾生成確率以内なら二回目を実行
		if (rand() % 100 < BOMB_DOUBLE_PROP) {
			// ボムをidx番データのランダムな位置に生成
			int x = rand() % (FIELD_W - 2);
			block[idx][x] = BOMB_DATA;
		}
	}
}

//=============================================================================
// 爆弾ブロック作動
//=============================================================================
void DoBomb(int idx) {
	for (int i = 0; i < BOMB_POWER; i++) {
		memcpy(block[idx], block_empty, sizeof(block_empty));		// 丸一行消す
		idx = g_BlockIndex[idx];
	}
}

//=============================================================================
// パワーアップブロック生成
//=============================================================================
void CreatePOW(int idx) {
	// 生成確率以内なら
	if (rand() % 100 < POW_PROP) {
		// ボムをidx番データのランダムな位置に生成
		int x = rand() % (FIELD_W - 2);
		block[idx][x] = POW_DATA;
	}
}

//=============================================================================
// パワーアップブロック作動
//=============================================================================
void DoPOW(void) {
	BALL* ball = GetBall();
	// パワー上限に既に達していたら
	if (ball->power >= POW_MAX) {
		return;
	}
	// パワーアップ
	ball->power++;
}