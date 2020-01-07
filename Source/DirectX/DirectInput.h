//**********************************************************
// �t�@�C����		�FDirectInput.h
//**********************************************************
#pragma once
#pragma warning(disable:4996)

#define DIRECTINPUT_VERSION 0x0800
#define INPUT_QUANTITY_RATE 150
#define DI_KEYBOARD_BUFFER_SIZE 256
#define DI_JOYSTICK1_BUTTON_SIZE 32
#define DI_JOYSTICK2_BUTTON_SIZE 128
#define DI_MOUSE1_BUTTON_SIZE 4
#define DI_MOUSE2_BUTTON_SIZE 8

#include <dInput.h>
#include "../Temp/stdafx.h"
#include "../Engine/Input/InputEngine.h"
#include "../Engine/Input/MyInputCodeList.h"
#include "../Engine/Input/Input.h"

#pragma comment(lib,"dxguid")
#pragma comment(lib,"dInput8")

class CDirectInput : public CInputEngine
{
public:
	CDirectInput();
	~CDirectInput();

	void Initialize();
	void Release();
	void Update();

	// �C�ӂ̃f�o�C�X���쐬
	void CreateKeyboard(HWND hWnd_);
	void CreateMouse(HWND hWnd_);
	void CreateJoystick(HWND hWnd_);

	// �C�ӂ̃f�o�C�X�����
	void ReleaseKeyboard();
	void ReleaseMouse();
	void ReleaseJoystick();

private:
	// �f�o�C�X�̃|�C���^
	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpKeyboard;
	LPDIRECTINPUTDEVICE8	m_lpMouse;
	LPDIRECTINPUTDEVICE8	m_lpJoystick;

	// ���݂̓��͏��
	DIMOUSESTATE	m_mouseCurrentState;
	DIJOYSTATE		m_joystickCurrentState;
	byte			m_keyboardCurrentState[DI_KEYBOARD_BUFFER_SIZE];

	// �O�t���[���̍��W
	DIMOUSESTATE	m_mousePrevState;

	// �C�ӂ̃f�o�C�X���X�V
	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateJoystick();

	// �f�o�C�X���ڑ�����Ă��邩�`�F�b�N
	bool IsKeyboardConected()const;
	bool IsMouseConected()const;
	bool IsJoystickConected()const;
	bool CheckInputDeviceConectted()const;

	// �X�e�B�b�N�̎������p
	float AdjustValue(const LONG n);

	// ���͏��X�V�p
	void UpdateInputFlag(bool isInput_, InputFlag& flag_);
};
