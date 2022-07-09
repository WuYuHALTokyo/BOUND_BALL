//=============================================================================
//
// ���n�_���� [boundPoint.h]
// Author : ���D
//
//=============================================================================
#pragma once

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		BOUNDPOINT_DATA		('@')

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct BOUNDPOINT {
	int x, y;							// ���n�_�̍��W
	int use;							// ���n�_�̗L�����t���O�B0�Ŗ����A1�ŗL��
};
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBoundPoint(void);
void UninitBoundPoint(void);
void UpdateBoundPoint(void);
void DrawBoundPoint(void);

// ���n�_�̎擾
BOUNDPOINT* GetBoundPoint(void);

// ���n�_�̐���
void CreateBoundPoint(void);