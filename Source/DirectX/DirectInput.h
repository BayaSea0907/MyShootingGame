//**********************************************************
// ファイル名		：DirectInput.h
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

	// 任意のデバイスを作成
	void CreateKeyboard(HWND hWnd_);
	void CreateMouse(HWND hWnd_);
	void CreateJoystick(HWND hWnd_);

	// 任意のデバイスを解放
	void ReleaseKeyboard();
	void ReleaseMouse();
	void ReleaseJoystick();

private:
	// デバイスのポインタ
	LPDIRECTINPUT8			m_lpDI;
	LPDIRECTINPUTDEVICE8	m_lpKeyboard;
	LPDIRECTINPUTDEVICE8	m_lpMouse;
	LPDIRECTINPUTDEVICE8	m_lpJoystick;

	// 現在の入力情報
	DIMOUSESTATE	m_mouseCurrentState;
	DIJOYSTATE		m_joystickCurrentState;
	byte			m_keyboardCurrentState[DI_KEYBOARD_BUFFER_SIZE];

	// 前フレームの座標
	DIMOUSESTATE	m_mousePrevState;

	// 任意のデバイスを更新
	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateJoystick();

	// デバイスが接続されているかチェック
	bool IsKeyboardConected()const;
	bool IsMouseConected()const;
	bool IsJoystickConected()const;
	bool CheckInputDeviceConectted()const;

	// スティックの軸調整用
	float AdjustValue(const LONG n);

	// 入力情報更新用
	void UpdateInputFlag(bool isInput_, InputFlag& flag_);
};
