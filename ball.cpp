//=============================================================================
//
// �{�[������ [ball.cpp]
// Author : ���D
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "ball.h"
#include "field.h"
#include "block.h"
#include "boundPoint.h"
#include "bar.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
BALL g_Ball;		// �{�[���\����

//=============================================================================
// �{�[���̏���������
//=============================================================================
void InitBall(void) {
	// �{�[���̏����ʒu
	g_Ball.x = 10;
	g_Ball.y = 10;
	// �{�[���̏����ړ���
	g_Ball.ver_x = 1;
	g_Ball.ver_y = 1;
	// �����Η�
	g_Ball.power = 2;
}

//=============================================================================
// �{�[���̏I������
//=============================================================================
void UninitBall(void) {

}

//=============================================================================
// �{�[���̍X�V����
//=============================================================================
void UpdateBall(void) {

	// ���̏ꏊ���o�b�N�A�b�v
	g_Ball.x_old = g_Ball.x;
	g_Ball.y_old = g_Ball.y;
	// �ړ�����
	g_Ball.x += g_Ball.ver_x;
	g_Ball.y += g_Ball.ver_y;

	/////////////////////////////////////////////////////////////////////////////
	// �o�E���h����
	/////////////////////////////////////////////////////////////////////////////
	if (CheckField(g_Ball.x, g_Ball.y)) {
		// �o�E���h�n�_��������A���������Ė���
		if (CheckField(g_Ball.x, g_Ball.y, BOUNDPOINT_DATA)) {
			GetBoundPoint()->use = 0;
		}
		else {
			// �f�b�p���Ȃ̂��`�F�b�N
			int isBump = 0;

			// ���̏ꏊ��x�����ɕ��������
			if (CheckField(g_Ball.x, g_Ball.y_old)) {
				// �u���b�N�����������
				if (CheckField(g_Ball.x, g_Ball.y_old, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y_old, g_Ball.power);
				}
				// POW������������ăp���[�A�b�v
				else if (CheckField(g_Ball.x, g_Ball.y_old, POW_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y_old, g_Ball.power);
					DoPOW();
				}
				// �o�[��������FPS���グ��
				else if (CheckField(g_Ball.x, g_Ball.y_old, BAR_DATA)) {
					FPSIncrease();
				}
				// �n�ʂ�������FPS��������
				else if (CheckField(g_Ball.x, g_Ball.y_old, CEILING_DATA)) {
					FPSDecrease();
				}

				// �ړ��ʂ�x�𔽓]
				g_Ball.ver_x = -g_Ball.ver_x;
				// �ꏊ�𒲐�
				g_Ball.x = g_Ball.x_old + g_Ball.ver_x;
				isBump++;
			}

			// ���̏ꏊ��y�����ɕ��������
			if (CheckField(g_Ball.x_old, g_Ball.y)) {
				// �u���b�N�����������
				if (CheckField(g_Ball.x_old, g_Ball.y, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x_old, g_Ball.y, g_Ball.power);
				}
				// POW������������ăp���[�A�b�v
				else if (CheckField(g_Ball.x_old, g_Ball.y, POW_DATA)) {
					DeleteBlock(g_Ball.x_old, g_Ball.y, g_Ball.power);
					DoPOW();
				}
				// �o�[��������FPS���グ��
				else if (CheckField(g_Ball.x_old, g_Ball.y, BAR_DATA)) {
					FPSIncrease();
				}
				// �n�ʂ�������FPS��������
				else if (CheckField(g_Ball.x_old, g_Ball.y, CEILING_DATA)) {
					FPSDecrease();
				}

				// �ړ��ʂ�y�𔽓]
				g_Ball.ver_y = -g_Ball.ver_y;
				// �ꏊ�𒲐�
				g_Ball.y = g_Ball.y_old + g_Ball.ver_y;
				isBump++;
			}

			// �������s���ꂽ�ꍇ���݂̏����ƂȂ�

			// �f�b�p���������ꍇ
			if (isBump == 0) {
				// �u���b�N�����������
				if (CheckField(g_Ball.x, g_Ball.y, BLOCK_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y, g_Ball.power);
				}
				// POW������������ăp���[�A�b�v
				else if (CheckField(g_Ball.x, g_Ball.y, POW_DATA)) {
					DeleteBlock(g_Ball.x, g_Ball.y, g_Ball.power);
					DoPOW();
				}
				// FPS�ω�����

				// �ړ��ʂ𔽓]
				g_Ball.ver_x = -g_Ball.ver_x;
				g_Ball.ver_y = -g_Ball.ver_y;
				// �ꏊ�𒲐�
				g_Ball.x = g_Ball.x_old + g_Ball.ver_x;
				g_Ball.y = g_Ball.y_old + g_Ball.ver_y;
			}
			// ���n�_�̍Đ���
			CreateBoundPoint();
		}
	}
}

//=============================================================================
// �{�[���̕`�揈��
//=============================================================================
void DrawBall(void) {
	SetField(g_Ball.x, g_Ball.y, BALL_DATA);
}

//=============================================================================
// �{�[�����擾
//=============================================================================
BALL* GetBall(void) {
	return &g_Ball;
}

//=============================================================================
// �`���[�W�V���b�g
//=============================================================================
void Shot(void) {

}