#include "GameEngine.h"

CGameEngine::CGameEngine()
{
	InputEngine = NULL;
	RenderEngine = NULL;
	SoundEngine = NULL;
}

CGameEngine::~CGameEngine()
{
}

//----------------------------------------------------------
// ������
//----------------------------------------------------------
void CGameEngine::Initialize(HWND _hWnd)
{
	try
	{
		if (RenderEngine == NULL)
		{
			RenderEngine = new CDirect3D;
			RenderEngine->Initialize(_hWnd);
		}

		if (InputEngine == NULL)
		{
			InputEngine = new CDirectInput();
			InputEngine->Initialize();
			InputEngine->CreateMouse(_hWnd);
			InputEngine->CreateKeyboard(_hWnd);
			InputEngine->CreateJoystick(_hWnd);
		}

		if (SoundEngine == NULL)
		{
			// ��
			SoundEngine = new CDirectSound();
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/BGM/bgm_mainGame.wav");
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/SE/se_shot.wav");
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/SE/se_explosion.wav");

			SoundEngine->PlayLoop(0);
		}
		// ���݂̎��Ԃ��擾
		nowTime = GetNanoTime(chrono::system_clock::now());
	}
	catch (bad_alloc& err)
	{
		throw err;
	}
	catch (...)
	{
		throw;
	}

}

//----------------------------------------------------------
// �e�G���W���̍X�V
//----------------------------------------------------------
void CGameEngine::Update()
{
	try
	{
		// 1�b�ɖ����Ȃ��Ԃ̃t���[����Ԃ�
		seconds_df beforeTime = nowTime;
		nowTime = (GetNanoTime(chrono::system_clock::now()));
		deltaTime = nowTime - beforeTime;

		// �o�̓E�C���h�E��fps�\��
		TCHAR str[256];
		_stprintf_s(str, TEXT("%f		fps "));
		OutputDebugString(str);

		// Input
		if (InputEngine != NULL)
		{
			InputEngine->Update();
		}

		// Render
		if (RenderEngine != NULL)
		{
			RenderEngine->Rendering();
		}
	}

	// ��O����
	catch (bad_alloc& err)
	{
		_RPTN(_CRT_ERROR, "Bad allocation.\n%s\n", err.what());
		throw err;
	}
	catch (invalid_argument err)
	{
		// �֐��̉������ŕs���Ȓl��n���ꂽ�ꍇ�Ɏg�p����B
		_RPTN(_CRT_ERROR, "Invalid argument.\n%s\n", err.what());
		throw err;
	}
	catch (runtime_error err)
	{
		// �_���G���[�Ŏ�肫�邱�Ƃ̂ł��Ȃ����s���G���[�B
		_RPTN(_CRT_ERROR, "Runtime error.\n%s\n", err.what());
		throw err;
	}
	catch (domain_error err)
	{
		// ��ɐ��w�֘A�̏����őz��͈͊O�̒l���n���ꂽ�ꍇ�Ɏg�p����B
		_RPTN(_CRT_ERROR, "Domain error.\n%s\n", err.what());
		throw err;
	}
	// length_error
	// out_of_range
	// range_error
	// overflow_error
	// underflow_error
	// system_error
	catch (...)
	{
		_RPTN(_CRT_ERROR, "an unexpected error.%s\n", "");
		throw;
	}
}

//----------------------------------------------------------
// ���
//----------------------------------------------------------
void CGameEngine::Release()
{
	if (InputEngine != NULL)
	{
		InputEngine->Release();
		delete InputEngine;
		InputEngine = NULL;
	}
}

// �i�m�b�ŕԂ�
seconds_df CGameEngine::GetNanoTime(const chrono::system_clock::time_point tp_)
{
	return chrono::duration_cast<seconds_df>(chrono::duration_cast<microseconds_df>(tp_.time_since_epoch()));
}