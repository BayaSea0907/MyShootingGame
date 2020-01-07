//**********************************************************
// �t�@�C����		:Input.h
//**********************************************************
#pragma once
#include "../../Temp/stdafx.h"
#include "MyInputCodeList.h"

struct InputFlag
{
	int frame;
	bool down;
	bool once;
	bool up;
	bool off;

	InputFlag()
	{
		frame = 0;
		down = false;
		once = false;
		up = false;
		off = false;
	}
};

struct InputInfo
{		
	InputFlag* key;			// InputEngine�Ŕz��̐擪�A�h���X���Ԃ����݂܂�
	InputFlag* mouse;		// 
	InputFlag* pad;			// 
	XMFLOAT3 mouseMoveValue;
	XMFLOAT3 rStickValue;
	XMFLOAT3 lStickValue;
};

// ���̓C���^�[�t�F�[�X
class CInput
{
private:
	static CInput* instance;	// �V���O���g��
	CInput(){}
	~CInput(){}

public:
	void Initialize();
	void Release();

	InputInfo inputInfo;
	static CInput& GetInstance() { return *instance; }

	// �L�[�{�[�h����
	bool GetKey(InputCode code_)const;
	bool GetKeyDown(InputCode code_)const;
	bool GetKeyUp(InputCode code_)const;
	bool GetKeyOff(InputCode code_) const;

	// �}�E�X���͔��f
	bool GetMouseButton(InputCode code_)const;
	bool GetMouseButtonDown(InputCode code_)const;
	bool GetMouseButtonUp(InputCode code_)const;
	bool GetMouseButtonOff(InputCode code_)const;

	// �Q�[���p�b�h���͔��f
	bool GetButton(InputCode code_)const;
	bool GetButtonDown(InputCode code_)const;
	bool GetButtonUp(InputCode code_)const;
	bool GetButtonOff(InputCode code_)const;
	
	// �Q�[���p�b�h�\���L�[���f�p
	bool GetButtonDirection(InputCode code_)const;

	// �}�E�X�E�X�e�B�b�N�̓��͗ʎ擾 -1~1�͈̔͂̒l��Ԃ�
	float GetAxis(InputCode cpde_)const;
};

