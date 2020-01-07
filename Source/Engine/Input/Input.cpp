#include "Input.h"

CInput* CInput::instance = NULL;		// シングルトン

// 初期化
void CInput::Initialize()
{
	if (instance == NULL)
	{
		instance = new CInput();
		ZeroMemory(&(instance->inputInfo), sizeof(InputInfo));
	}
}

// 解放
void CInput::Release()
{
	if (instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}

// キーボード入力判断用
bool CInput::GetKey(InputCode code_)const{ return inputInfo.key[code_].down; }
bool CInput::GetKeyDown(InputCode code_)const{ return inputInfo.key[code_].once; }
bool CInput::GetKeyUp(InputCode code_)const{ return inputInfo.key[code_].up; }
bool CInput::GetKeyOff(InputCode code_)const{ return inputInfo.key[code_].off; }

// マウス入力判断
bool CInput::GetMouseButton(InputCode code_)const{ return inputInfo.mouse[code_].down; }
bool CInput::GetMouseButtonDown(InputCode code_)const{ return inputInfo.mouse[code_].once; }
bool CInput::GetMouseButtonUp(InputCode code_)const{ return inputInfo.mouse[code_].up; }
bool CInput::GetMouseButtonOff(InputCode code_)const{	return inputInfo.mouse[code_].off; }

// ゲームパッド入力判断
bool CInput::GetButton(InputCode code_)const{	return inputInfo.pad[code_].down; }
bool CInput::GetButtonDown(InputCode code_)const{	return inputInfo.pad[code_].once; }
bool CInput::GetButtonUp(InputCode code_)const{ return inputInfo.pad[code_].up; }
bool CInput::GetButtonOff(InputCode code_)const{ return inputInfo.pad[code_].off; }

// 十字キー入力判断用
bool CInput::GetButtonDirection(InputCode code_)const
{
	switch (code_)
	{
	case PAD_UP:	return inputInfo.lStickValue.y > 0;
	case PAD_DOWN:	return inputInfo.lStickValue.y < 0;
	case PAD_RIGHT:	return inputInfo.lStickValue.x > 0;
	case PAD_LEFT:	return inputInfo.lStickValue.x < 0;
	}
	return false;
}
// スティック・マウスの入力量取得
float CInput::GetAxis(InputCode cpde_)const
{
	float f = 0.0f;

	switch (cpde_)
	{
	// マウス
	case MOUSE_X:
		f = inputInfo.mouseMoveValue.x;
		break;

	case MOUSE_Y:
		f = inputInfo.mouseMoveValue.y;
		break;

	case MOUSE_Z:
		f = inputInfo.mouseMoveValue.z;
		break;

	// スティック
	case PAD_LX:
		f = inputInfo.lStickValue.x;
		break;

	case PAD_LY:
		f = inputInfo.lStickValue.y;
		break;

	case PAD_RX:
		f = inputInfo.rStickValue.x;
		break;

	case PAD_RY:
		f = inputInfo.rStickValue.y;
		break;
	}

	return f;
}
	


