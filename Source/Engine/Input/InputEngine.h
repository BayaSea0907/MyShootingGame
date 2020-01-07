//**********************************************************
// �t�@�C����		�FInputEngine.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"
#include "MyInputCodeList.h"

class CInputEngine
{
private:
public:
	CInputEngine();
	virtual ~CInputEngine();

	virtual void Update();
	virtual void Initialize();

	// �f�o�C�X����
	virtual void CreateKeyboard(HWND hWnd_);
	virtual void CreateMouse(HWND hWnd_);
	virtual void CreateJoystick(HWND hWnd_);

	// �f�o�C�X���
	virtual void Release();
	virtual void ReleaseKeyboard();
	virtual void ReleaseMouse();
	virtual void ReleaseJoystick();
};

