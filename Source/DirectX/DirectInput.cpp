#include "DirectInput.h"

CDirectInput::CDirectInput()
{
}

CDirectInput::~CDirectInput()
{
}

//--------------------------------------------------
// ライブラリ初期化と生成
//--------------------------------------------------
void CDirectInput::Initialize()
{
	m_lpDI = NULL;
	m_lpKeyboard = NULL;
	m_lpMouse = NULL;
	m_lpJoystick = NULL;
	ZeroMemory(&m_mouseCurrentState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_mousePrevState, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_joystickCurrentState, sizeof(DIJOYSTATE));
	ZeroMemory(&m_keyboardCurrentState, sizeof(BYTE) * DI_KEYBOARD_BUFFER_SIZE);

	// インプットインターフェース作成
	CInput::GetInstance().Initialize();

	// DirectInputDeviceの作成
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_lpDI, NULL);

	if (m_lpDI == nullptr) {
		throw invalid_argument("DirectInput");
	}
}

//--------------------------------------------------
// キーボードの生成
//--------------------------------------------------
void CDirectInput::CreateKeyboard(HWND hWnd_)
{
	// デバイスの作成
	m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL);
	
	// 入力判断用データの生成
	auto& input = CInput::GetInstance();
	input.inputInfo.key = new InputFlag[DI_KEYBOARD_BUFFER_SIZE];

	if (m_lpKeyboard != NULL)
	{
		// データ形式を設定
		m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);

		// 協調レベルの設定	
		m_lpKeyboard->SetCooperativeLevel(hWnd_, DISCL_EXCLUSIVE | DISCL_BACKGROUND);

		// 更新
		m_lpKeyboard->Acquire();
	}
}

//--------------------------------------------------
// マウス生成
//--------------------------------------------------
void CDirectInput::CreateMouse(HWND _hWnd)
{
	// デバイスの作成
	m_lpDI->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL);

	// 入力判断用データの生成
	auto& input = CInput::GetInstance();
	input.inputInfo.mouse = new InputFlag[DI_MOUSE1_BUTTON_SIZE];
	if (m_lpMouse != NULL)
	{
		// データ形式を設定
		m_lpMouse->SetDataFormat(&c_dfDIMouse);

		// モードを設定（フォアグラウンド＆非排他モード）
		m_lpMouse->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	// 相対値値モードで設定
		m_lpMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		//更新
		m_lpMouse->Acquire();
	}
}

//--------------------------------------------------
// ゲームパッドの生成
//--------------------------------------------------
void CDirectInput::CreateJoystick(HWND hWnd_)
{
	// デバイスの作成
	m_lpDI->CreateDevice(GUID_Joystick, &m_lpJoystick, NULL);

	// 入力判断用データの生成
	auto& input = CInput::GetInstance();
	input.inputInfo.pad = new InputFlag[DI_JOYSTICK1_BUTTON_SIZE];

	if (m_lpJoystick != NULL)
	{
		// データ形式を設定
		m_lpJoystick->SetDataFormat(&c_dfDIJoystick);

		// 強調レベルの設定
		m_lpJoystick->SetCooperativeLevel(hWnd_, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		// スティックの範囲を指定
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_DEVICE;
		diprg.diph.dwObj = 0;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		m_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 更新
		m_lpJoystick->Acquire();
	}
}

//----------------------------------------------------------
// ライブラリの更新
//----------------------------------------------------------
void CDirectInput::Update()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateJoystick();
}

//--------------------------------------------------
// キーボードの更新
//--------------------------------------------------
void CDirectInput::UpdateKeyboard()
{
	if (!IsKeyboardConected()) return;

	// デバイスが取得できるまで更新
	if (FAILED(m_lpKeyboard->Poll()))
	{
		while (m_lpKeyboard->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpKeyboard->Acquire();

	// 入力情報を更新
	m_lpKeyboard->GetDeviceState(sizeof(byte) * DI_KEYBOARD_BUFFER_SIZE, &m_keyboardCurrentState);	

	// 入力フラグを更新
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_KEYBOARD_BUFFER_SIZE; i++)
	{
		UpdateInputFlag(m_keyboardCurrentState[i], input.inputInfo.key[i]);
	}
}

//--------------------------------------------------
// マウスの更新
//--------------------------------------------------
void CDirectInput::UpdateMouse()
{
	if (!IsMouseConected()) return;


	// デバイスが取得できるまで更新
	if (FAILED(m_lpMouse->Poll()))
	{
		while (m_lpMouse->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpMouse->Acquire();

	// 前の状態を記憶する
	CopyMemory(&m_mouseCurrentState, &m_mousePrevState, sizeof(DIMOUSESTATE));

	// 入力情報を更新
	m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseCurrentState);

	// 入力フラグを更新
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_MOUSE1_BUTTON_SIZE; i++)
	{
		UpdateInputFlag(m_mouseCurrentState.rgbButtons[i], input.inputInfo.mouse[i]);
	}

	// マウス移動量の更新
	input.inputInfo.mouseMoveValue.x =  (float)m_mouseCurrentState.lX - (float)m_mousePrevState.lX;
	input.inputInfo.mouseMoveValue.y = -((float)m_mouseCurrentState.lY - (float)m_mousePrevState.lY);
	input.inputInfo.mouseMoveValue.z = -((float)m_mouseCurrentState.lZ - (float)m_mousePrevState.lZ);
}

//--------------------------------------------------
// ゲームパッドの更新
//--------------------------------------------------
void CDirectInput::UpdateJoystick()
{
	if (!IsJoystickConected()) return;

	// デバイスが取得できるまで更新
	if (FAILED(m_lpJoystick->Poll()))
	{
		while (m_lpJoystick->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpJoystick->Acquire();

	// 入力情報を更新
	m_lpJoystick->GetDeviceState(sizeof(DIJOYSTATE), &m_joystickCurrentState);

	// 入力フラグを更新
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_JOYSTICK1_BUTTON_SIZE; i++)
	{
		// 入力中されてたらtrue
		UpdateInputFlag(m_joystickCurrentState.rgbButtons[i], input.inputInfo.pad[i]);
	}

	// スティックの入力量を制限して返す
	input.inputInfo.lStickValue = { AdjustValue(m_joystickCurrentState.lX),  -AdjustValue(m_joystickCurrentState.lY), 0.0f };
	input.inputInfo.rStickValue = { AdjustValue(m_joystickCurrentState.lZ),  -AdjustValue(m_joystickCurrentState.lRz), 0.0f };
}

// 入力情報の更新
void CDirectInput::UpdateInputFlag(bool isInput_, InputFlag& flag_)
{
	if (isInput_)
	{
		flag_.frame++;

		// 入力の瞬間の処理
		if (flag_.frame == 1)
		{
			flag_.down = true;
			flag_.once = true;
		}
		// 入力中ずっとの処理
		else
		{
			flag_.down = true;
			flag_.once = false;
		}
	}
	// 離した瞬間の処理
	else if (flag_.frame > 0)
	{
		flag_.frame = -1;
		flag_.up = true;
		flag_.down = false;
		flag_.once = false;
	}
	// 離した次のフレームの処理
	else if (flag_.frame == -1)
	{
		flag_.frame = 0;
		flag_.up = false;
	}
}

// スティック入力量を-1~1の間に制限する
float CDirectInput::AdjustValue(const LONG n)
{
	float f = 0.0f;

	if (-100 < n && n < 100)
	{
		f = 0;
	}
	else
	{
		f = (float)n / 1000.0f;		
	} 

	return f;
}
//--------------------------------------------------
// ライブラリ開放
//--------------------------------------------------
void CDirectInput::Release()
{
	ReleaseKeyboard();
	ReleaseMouse();
	ReleaseJoystick();

	// インプットインターフェースの
	CInput::GetInstance().Release();

	// DirectInputDeviceの解放
	SAFE_RELEASE(m_lpDI);
}

//--------------------------------------------------
// キーボードの開放
//--------------------------------------------------
void CDirectInput::ReleaseKeyboard()
{
	if (!IsKeyboardConected()) return;

	m_lpKeyboard->Unacquire();
	SAFE_RELEASE(m_lpKeyboard);
}
//--------------------------------------------------
// マウスの開放
//--------------------------------------------------
void CDirectInput::ReleaseMouse()
{
	if (!IsMouseConected()) return;

	m_lpMouse->Unacquire();
	SAFE_RELEASE(m_lpMouse);
}
//--------------------------------------------------
// ゲームパッドの開放
//--------------------------------------------------
void CDirectInput::ReleaseJoystick()
{
	if (!IsJoystickConected()) return;

	m_lpJoystick->Unacquire();
	SAFE_RELEASE(m_lpJoystick);
}

//---------------------------------------------------------
// デバイスの接続チェック
//---------------------------------------------------------
bool CDirectInput::CheckInputDeviceConectted() const { return m_lpDI != NULL; }		// DirectInput
bool CDirectInput::IsKeyboardConected() const { return m_lpKeyboard != NULL; }		// keyboard
bool CDirectInput::IsMouseConected() const { return m_lpMouse != NULL; }			// mouse
bool CDirectInput::IsJoystickConected() const { return m_lpJoystick != NULL; }		// gamePad