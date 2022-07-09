//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : ���D
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"
#include "field.h"
#include "ball.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		EMPTY_LINE			('!')		// ���g�p�u���b�N�f�[�^�^�O�B�擪�ɕt���Ă�Ζ��g�p
#define		EMPTY_INDEX			(-1)		// ���g�p�ԍ�

#define		BLOCK_ORIGIN_X		(1)			// �u���b�N�̊�{�z�u�ꏊ(�ǂ̓����̈�ԍ���)
#define		BLOCK_ORIGIN_Y		(1)

#define		BLOCK_SLOT			(20)		// �u���b�N�p�^�[���̃o���G�[�V���������
#define		BLOCK_MAX			(50)		// �u���b�N�̍ő�ۑ��s��
#define		BLOCK_START			(5)			// �Q�[���X�^�[�g���̃u���b�N�s��

#define		BOMB_PROP			(75)		// ���e�̐����m��
#define		BOMB_DOUBLE_PROP	(25)		// �_�u�����e�̐����m��
#define		BOMB_POWER			(2)			// ��x�ɔ��j����s��

#define		POW_PROP			(20)		// POW�̐����m��

#define		WAVE_TIME			(120)		// �u���b�N�����̊Ԋu

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
// �u���b�N�p�^�[���B����-1���Ă�̂́A�ǂ�������-2�ANULL��������+1�̍��v
char block_org[BLOCK_SLOT][FIELD_W - 1] = {		
	"##########################################################",		// �f�t�H���g�p�^�[��
};

// �u���b�N�f�[�^
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

int g_BlockIndex[BLOCK_MAX];					// �u���b�N�̔ԍ����X�g
int g_TopIndex;									// ���X�g�̐擪�ւ̃|�C���^

int g_ActiveLine ;								// �ғ����̃u���b�N�̍s��

int waveCount;									// �E�F�[�u�̃J�E���^

//=============================================================================
// �u���b�N�̏���������
//=============================================================================
void InitBlock(void) {

	for (int i = 0; i < BLOCK_MAX; i++) {
		block[i][0] = EMPTY_LINE;			// �u���b�N�f�[�^�ɂ��ׂĖ��g�p�^�O������
		g_BlockIndex[i] = EMPTY_INDEX;		// �ԍ������ׂ�EMPTY��
	}
	g_TopIndex = EMPTY_INDEX;
	g_ActiveLine = 0;
	waveCount = 0;

	// �����u���b�N���Z�b�g���A�ԍ���U��
	for (int i = 0; i < BLOCK_START; i++)
		AddBlockLine();
}

//=============================================================================
// �u���b�N�̏I������
//=============================================================================
void UninitBlock(void) {

}

//=============================================================================
// �u���b�N�̍X�V����
//=============================================================================
void UpdateBlock(void) {

	//�E�F�[�u����
	//���Ԃ��ƂɃu���b�N��������
	waveCount++;
	
	if (waveCount % WAVE_TIME == 0)
		AddBlockLine();

	// ����ۂɂȂ����擪�u���b�N�f�[�^������Ή��
	if (strcmp(block[g_TopIndex], block_empty) == 0)
		ClearTopLine();

	ReleaseDownLine();
}

//=============================================================================
// �u���b�N�̕`�揈��
//=============================================================================
void DrawBlock(void) {
	///////////////////////////////////////////////////////////////////////////
	// ���X�g���ɕ`��
	///////////////////////////////////////////////////////////////////////////
	int cur = g_TopIndex;

	if (cur >= 0 && cur <= BLOCK_MAX)									// �I�[�o�[�t���[�h�~
		for (int i = 0; i < g_ActiveLine; i++) {						// �ғ����s���̃u���b�N�����`�悵�Ȃ�
			SetField(BLOCK_ORIGIN_X, g_ActiveLine - i, block[cur]);		// ��������V��֕`��
			cur = g_BlockIndex[cur];									// ���̔ԍ����擾
		}
}

//=============================================================================
// �u���b�N��1�s�ǉ�
// 0628: �u���b�N���g��˂��j��̂Ɗ֌W�Ȃ�
//=============================================================================
void AddBlockLine(void) {

	// ����ۂ̃u���b�N�f�[�^������
	int i = 0;
	while (block[i][0] != EMPTY_LINE)
		i++;

	// �V�����u���b�N�f�[�^���i�[
	memcpy(block[i], block_org[0], sizeof(block_org[0]));

	// ���e����
	CreateBomb(i);

	// POW����
	CreatePOW(i);

	///////////////////////////////////////////////////////////////////////////
	// ���X�g�Ǘ�
	// 0628: ���f�o�b�O
	///////////////////////////////////////////////////////////////////////////
	// �ԍ������p
	int cur = g_TopIndex;

	// ���X�g�����ׂĖ��g�p�������ꍇ
	if (g_TopIndex == EMPTY_INDEX) {
		// �擪�v�f���w��
		g_BlockIndex[i] = g_TopIndex;
		g_TopIndex = i;
	}

	// ���X�g���ꕔ���g�p����Ȃ������ꍇ
	else {
		// �����v�f������
		while (g_BlockIndex[cur] != EMPTY_INDEX) {
			cur = g_BlockIndex[cur];

			// �I�[�o�[�t���[�h�~
			if (cur >= BLOCK_MAX) {
				return;
			}
		}
		// �i�[�����v�f�փ|�C���g
		g_BlockIndex[i] = g_BlockIndex[cur];
		g_BlockIndex[cur] = i;
	}

	// �ғ����s��+1
	g_ActiveLine++;
}

//=============================================================================
// �u���b�N������
// �t�B�[���h���W(x, y)�𒆐S�ɁA���apower�ȓ��̃u���b�N���폜�H(�����)
// 0628: �g��˂��j�錳���BFIELD_W-1��FIELD_W-2�ɂ����炷�ׂĂ�������
//=============================================================================
void DeleteBlock(int x, int y, int power) {

	// �{�[�����W���u���b�N���W�֕ϊ�
  	int x_block = x - BLOCK_ORIGIN_X;

	// ���S�_�̃u���b�N�̏����ԍ�
	int idx = g_ActiveLine - y;

	//////////////////////////////////////////////////////////////////////////////
	// ��������
	//////////////////////////////////////////////////////////////////////////////
	int cur = g_TopIndex;									// �ԍ���ǂ��p

	/*�ł����ɂ���s���璆���̉�1�s�ڂ܂�*/
	if (idx - power <= 0) {		// �u���b�N�f�[�^�̉��𒴂��鈽���͂��傤�ǂȏꍇ

		// �c�ɍ폜
		for (int i = 0; i < idx; i++) {

			// ���ɍ폜
			for (int j = 0; j < i + 1; j++) {
				if (x_block + j < FIELD_W - 2) {	// �I�[�o�[�t���[�h�~(FIELD_W-2���Ă���̂́A�ǂ�������)
					// ���e�������甚��
					if (block[cur][x_block + j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// �u���b�N������
					block[cur][x_block + j] = ' ';
				}
				if (x_block - j >= 0) {		// �I�[�o�[�t���[�h�~
					// ���e�������甚��
					if (block[cur][x_block - j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// �u���b�N������
					block[cur][x_block - j] = ' ';
				}

				// �I�[�o�[�t���[�h�~
				if (cur > BLOCK_MAX) {
					return;
				}
			}
			cur = g_BlockIndex[cur];
		}
	}
	else {		// �u���b�N�f�[�^�̉��𒴂��Ȃ��ꍇ
		
		// �폜�͈͓��ōł����̍s���擾
		for (int i = 0; i < idx - power; i++) {
			// �I�[�o�[�t���[�h�~
			if (cur > BLOCK_MAX) {
				return;
			}
			cur = g_BlockIndex[cur];
		}

		// �c�ɏ���
		for (int i = 0; i < power; i++) {		

			// ���ɏ���
			for (int j = 0; j < i + 1; j++) {
				if (x_block + j < FIELD_W - 2) {	// �I�[�o�[�t���[�h�~
					// ���e�������甚��
					if (block[cur][x_block + j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// �u���b�N������
					block[cur][x_block + j] = ' ';
				}
				if (x_block - j >= 0) {		// �I�[�o�[�t���[�h�~
					// ���e�������甚��
					if (block[cur][x_block - j] == BOMB_DATA) {
						DoBomb(cur);
						continue;
					}
					// ���ɏ���
					block[cur][x_block - j] = ' ';
				}

				// �I�[�o�[�t���[�h�~
				if (cur > BLOCK_MAX) {
					return;
				}

			}
			cur = g_BlockIndex[cur];
		}
	}

	/*�����̍s*/
	// ���ɏ���
	for (int j = 0; j < (power + 1); j++) {		// +1���Ă���̂́A����power*2+1����������
		if (x_block + j < FIELD_W - 2) {	// �I�[�o�[�t���[�h�~
			// ���e�������甚��
			if (block[cur][x_block + j] == BOMB_DATA) {
				DoBomb(cur);
				continue;
			}
			// �u���b�N������
			block[cur][x_block + j] = ' ';
		}
		if (x_block - j >= 0) {		// �I�[�o�[�t���[�h�~
			// ���e�������甚��
			if (block[cur][x_block - j] == BOMB_DATA) {
				DoBomb(cur);
				continue;
			}
			// �u���b�N������
			block[cur][x_block - j] = ' ';
		}

		// �I�[�o�[�t���[�h�~
		if (cur > BLOCK_MAX) {
			return;
		}

	}
	cur = g_BlockIndex[cur];

	/*�����̏�1�s�ڂ���ł���ɂ���s�܂�*/
	// �c�ɏ���
	for (int i = 0; i < power; i++) {

		if (cur == EMPTY_INDEX)		// ����ۂ̃u���b�N�f�[�^�ɐG��悤�Ƃ���ƏI��
			break;

		// ���ɏ���
		for (int j = 0; j < (power - i); j++) {
			if (x_block + j < FIELD_W - 2) {	// �I�[�o�[�t���[�h�~
				// ���e�������甚��
				if (block[cur][x_block + j] == BOMB_DATA) {
					DoBomb(cur);
					continue;
				}
				// �u���b�N������
				block[cur][x_block + j] = ' ';
			}
			if (x_block - j >= 0) {		// �I�[�o�[�t���[�h�~
				// ���e�������甚��
				if (block[cur][x_block - j] == BOMB_DATA) {
					DoBomb(cur);
					continue;
				}
				//�u���b�N������
				block[cur][x_block - j] = ' ';
			}

			// �I�[�o�[�t���[�h�~
			if (cur > BLOCK_MAX) {
				return;
			}

		}
		cur = g_BlockIndex[cur];
	}
}

//=============================================================================
// �擪�u���b�N�f�[�^�����
//=============================================================================
void ClearTopLine(void) {
	int cur = g_BlockIndex[g_TopIndex];						// �ԍ������X�g���珜�O
	g_TopIndex = cur;
	g_ActiveLine--;											// �ғ����s��-1
}

//=============================================================================
// ���E�܂ŉ��肽�u���b�N������A���e���܂܂�Ă�����Q�[���I�[�o�[
//=============================================================================
void ReleaseDownLine(void) {

	if (g_ActiveLine >= FIELD_H - DEAD_LINE) {

		// ���e�����邩�`�F�b�N
		for (int i = 0; i < FIELD_W - 1; i++) {
			if (block[g_TopIndex][i] == BOMB_DATA) {
				SetMode(MODE_RESULT);
				return;
			}
		}

		// �擪�u���b�N���폜
		ClearTopLine();
	}
}

//=============================================================================
// ���e�u���b�N����
//=============================================================================
void CreateBomb(int idx) {
	// �����m���ȓ��Ȃ�
	if (rand() % 100 < BOMB_PROP) {
		// �{����idx�ԃf�[�^�̃����_���Ȉʒu�ɐ���
		int x = rand() % (FIELD_W - 2);
		block[idx][x] = BOMB_DATA;

		// �_�u�����e�����m���ȓ��Ȃ���ڂ����s
		if (rand() % 100 < BOMB_DOUBLE_PROP) {
			// �{����idx�ԃf�[�^�̃����_���Ȉʒu�ɐ���
			int x = rand() % (FIELD_W - 2);
			block[idx][x] = BOMB_DATA;
		}
	}
}

//=============================================================================
// ���e�u���b�N�쓮
//=============================================================================
void DoBomb(int idx) {
	for (int i = 0; i < BOMB_POWER; i++) {
		memcpy(block[idx], block_empty, sizeof(block_empty));		// �ۈ�s����
		idx = g_BlockIndex[idx];
	}
}

//=============================================================================
// �p���[�A�b�v�u���b�N����
//=============================================================================
void CreatePOW(int idx) {
	// �����m���ȓ��Ȃ�
	if (rand() % 100 < POW_PROP) {
		// �{����idx�ԃf�[�^�̃����_���Ȉʒu�ɐ���
		int x = rand() % (FIELD_W - 2);
		block[idx][x] = POW_DATA;
	}
}

//=============================================================================
// �p���[�A�b�v�u���b�N�쓮
//=============================================================================
void DoPOW(void) {
	BALL* ball = GetBall();
	// �p���[����Ɋ��ɒB���Ă�����
	if (ball->power >= POW_MAX) {
		return;
	}
	// �p���[�A�b�v
	ball->power++;
}