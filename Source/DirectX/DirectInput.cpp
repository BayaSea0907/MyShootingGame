#include "DirectInput.h"

CDirectInput::CDirectInput()
{
}

CDirectInput::~CDirectInput()
{
}

//--------------------------------------------------
// ���C�u�����������Ɛ���
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

	// �C���v�b�g�C���^�[�t�F�[�X�쐬
	CInput::GetInstance().Initialize();

	// DirectInputDevice�̍쐬
	DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_lpDI, NULL);

	if (m_lpDI == nullptr) {
		throw invalid_argument("DirectInput");
	}
}

//--------------------------------------------------
// �L�[�{�[�h�̐���
//--------------------------------------------------
void CDirectInput::CreateKeyboard(HWND hWnd_)
{
	// �f�o�C�X�̍쐬
	m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL);
	
	// ���͔��f�p�f�[�^�̐���
	auto& input = CInput::GetInstance();
	input.inputInfo.key = new InputFlag[DI_KEYBOARD_BUFFER_SIZE];

	if (m_lpKeyboard != NULL)
	{
		// �f�[�^�`����ݒ�
		m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);

		// �������x���̐ݒ�	
		m_lpKeyboard->SetCooperativeLevel(hWnd_, DISCL_EXCLUSIVE | DISCL_BACKGROUND);

		// �X�V
		m_lpKeyboard->Acquire();
	}
}

//--------------------------------------------------
// �}�E�X����
//--------------------------------------------------
void CDirectInput::CreateMouse(HWND _hWnd)
{
	// �f�o�C�X�̍쐬
	m_lpDI->CreateDevice(GUID_SysMouse, &m_lpMouse, NULL);

	// ���͔��f�p�f�[�^�̐���
	auto& input = CInput::GetInstance();
	input.inputInfo.mouse = new InputFlag[DI_MOUSE1_BUTTON_SIZE];
	if (m_lpMouse != NULL)
	{
		// �f�[�^�`����ݒ�
		m_lpMouse->SetDataFormat(&c_dfDIMouse);

		// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
		m_lpMouse->SetCooperativeLevel(_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND);

		// �f�o�C�X�̐ݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl�l���[�h�Őݒ�
		m_lpMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		//�X�V
		m_lpMouse->Acquire();
	}
}

//--------------------------------------------------
// �Q�[���p�b�h�̐���
//--------------------------------------------------
void CDirectInput::CreateJoystick(HWND hWnd_)
{
	// �f�o�C�X�̍쐬
	m_lpDI->CreateDevice(GUID_Joystick, &m_lpJoystick, NULL);

	// ���͔��f�p�f�[�^�̐���
	auto& input = CInput::GetInstance();
	input.inputInfo.pad = new InputFlag[DI_JOYSTICK1_BUTTON_SIZE];

	if (m_lpJoystick != NULL)
	{
		// �f�[�^�`����ݒ�
		m_lpJoystick->SetDataFormat(&c_dfDIJoystick);

		// �������x���̐ݒ�
		m_lpJoystick->SetCooperativeLevel(hWnd_, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		// �X�e�B�b�N�͈̔͂��w��
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_DEVICE;
		diprg.diph.dwObj = 0;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		m_lpJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �X�V
		m_lpJoystick->Acquire();
	}
}

//----------------------------------------------------------
// ���C�u�����̍X�V
//----------------------------------------------------------
void CDirectInput::Update()
{
	UpdateKeyboard();
	UpdateMouse();
	UpdateJoystick();
}

//--------------------------------------------------
// �L�[�{�[�h�̍X�V
//--------------------------------------------------
void CDirectInput::UpdateKeyboard()
{
	if (!IsKeyboardConected()) return;

	// �f�o�C�X���擾�ł���܂ōX�V
	if (FAILED(m_lpKeyboard->Poll()))
	{
		while (m_lpKeyboard->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpKeyboard->Acquire();

	// ���͏����X�V
	m_lpKeyboard->GetDeviceState(sizeof(byte) * DI_KEYBOARD_BUFFER_SIZE, &m_keyboardCurrentState);	

	// ���̓t���O���X�V
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_KEYBOARD_BUFFER_SIZE; i++)
	{
		UpdateInputFlag(m_keyboardCurrentState[i], input.inputInfo.key[i]);
	}
}

//--------------------------------------------------
// �}�E�X�̍X�V
//--------------------------------------------------
void CDirectInput::UpdateMouse()
{
	if (!IsMouseConected()) return;


	// �f�o�C�X���擾�ł���܂ōX�V
	if (FAILED(m_lpMouse->Poll()))
	{
		while (m_lpMouse->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpMouse->Acquire();

	// �O�̏�Ԃ��L������
	CopyMemory(&m_mouseCurrentState, &m_mousePrevState, sizeof(DIMOUSESTATE));

	// ���͏����X�V
	m_lpMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseCurrentState);

	// ���̓t���O���X�V
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_MOUSE1_BUTTON_SIZE; i++)
	{
		UpdateInputFlag(m_mouseCurrentState.rgbButtons[i], input.inputInfo.mouse[i]);
	}

	// �}�E�X�ړ��ʂ̍X�V
	input.inputInfo.mouseMoveValue.x =  (float)m_mouseCurrentState.lX - (float)m_mousePrevState.lX;
	input.inputInfo.mouseMoveValue.y = -((float)m_mouseCurrentState.lY - (float)m_mousePrevState.lY);
	input.inputInfo.mouseMoveValue.z = -((float)m_mouseCurrentState.lZ - (float)m_mousePrevState.lZ);
}

//--------------------------------------------------
// �Q�[���p�b�h�̍X�V
//--------------------------------------------------
void CDirectInput::UpdateJoystick()
{
	if (!IsJoystickConected()) return;

	// �f�o�C�X���擾�ł���܂ōX�V
	if (FAILED(m_lpJoystick->Poll()))
	{
		while (m_lpJoystick->Acquire() == DIERR_INPUTLOST);
	}
	else m_lpJoystick->Acquire();

	// ���͏����X�V
	m_lpJoystick->GetDeviceState(sizeof(DIJOYSTATE), &m_joystickCurrentState);

	// ���̓t���O���X�V
	auto& input = CInput::GetInstance();
	for (int i = 0; i < DI_JOYSTICK1_BUTTON_SIZE; i++)
	{
		// ���͒�����Ă���true
		UpdateInputFlag(m_joystickCurrentState.rgbButtons[i], input.inputInfo.pad[i]);
	}

	// �X�e�B�b�N�̓��͗ʂ𐧌����ĕԂ�
	input.inputInfo.lStickValue = { AdjustValue(m_joystickCurrentState.lX),  -AdjustValue(m_joystickCurrentState.lY), 0.0f };
	input.inputInfo.rStickValue = { AdjustValue(m_joystickCurrentState.lZ),  -AdjustValue(m_joystickCurrentState.lRz), 0.0f };
}

// ���͏��̍X�V
void CDirectInput::UpdateInputFlag(bool isInput_, InputFlag& flag_)
{
	if (isInput_)
	{
		flag_.frame++;

		// ���͂̏u�Ԃ̏���
		if (flag_.frame == 1)
		{
			flag_.down = true;
			flag_.once = true;
		}
		// ���͒������Ƃ̏���
		else
		{
			flag_.down = true;
			flag_.once = false;
		}
	}
	// �������u�Ԃ̏���
	else if (flag_.frame > 0)
	{
		flag_.frame = -1;
		flag_.up = true;
		flag_.down = false;
		flag_.once = false;
	}
	// ���������̃t���[���̏���
	else if (flag_.frame == -1)
	{
		flag_.frame = 0;
		flag_.up = false;
	}
}

// �X�e�B�b�N���͗ʂ�-1~1�̊Ԃɐ�������
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
// ���C�u�����J��
//--------------------------------------------------
void CDirectInput::Release()
{
	ReleaseKeyboard();
	ReleaseMouse();
	ReleaseJoystick();

	// �C���v�b�g�C���^�[�t�F�[�X��
	CInput::GetInstance().Release();

	// DirectInputDevice�̉��
	SAFE_RELEASE(m_lpDI);
}

//--------------------------------------------------
// �L�[�{�[�h�̊J��
//--------------------------------------------------
void CDirectInput::ReleaseKeyboard()
{
	if (!IsKeyboardConected()) return;

	m_lpKeyboard->Unacquire();
	SAFE_RELEASE(m_lpKeyboard);
}
//--------------------------------------------------
// �}�E�X�̊J��
//--------------------------------------------------
void CDirectInput::ReleaseMouse()
{
	if (!IsMouseConected()) return;

	m_lpMouse->Unacquire();
	SAFE_RELEASE(m_lpMouse);
}
//--------------------------------------------------
// �Q�[���p�b�h�̊J��
//--------------------------------------------------
void CDirectInput::ReleaseJoystick()
{
	if (!IsJoystickConected()) return;

	m_lpJoystick->Unacquire();
	SAFE_RELEASE(m_lpJoystick);
}

//---------------------------------------------------------
// �f�o�C�X�̐ڑ��`�F�b�N
//---------------------------------------------------------
bool CDirectInput::CheckInputDeviceConectted() const { return m_lpDI != NULL; }		// DirectInput
bool CDirectInput::IsKeyboardConected() const { return m_lpKeyboard != NULL; }		// keyboard
bool CDirectInput::IsMouseConected() const { return m_lpMouse != NULL; }			// mouse
bool CDirectInput::IsJoystickConected() const { return m_lpJoystick != NULL; }		// gamePad