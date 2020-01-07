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
// 初期化
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
			// 仮
			SoundEngine = new CDirectSound();
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/BGM/bgm_mainGame.wav");
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/SE/se_shot.wav");
			SoundEngine->CreateSecondaryBuffer("../Resources/sounds/SE/se_explosion.wav");

			SoundEngine->PlayLoop(0);
		}
		// 現在の時間を取得
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
// 各エンジンの更新
//----------------------------------------------------------
void CGameEngine::Update()
{
	try
	{
		// 1秒に満たない間のフレームを返す
		seconds_df beforeTime = nowTime;
		nowTime = (GetNanoTime(chrono::system_clock::now()));
		deltaTime = nowTime - beforeTime;

		// 出力ウインドウにfps表示
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

	// 例外処理
	catch (bad_alloc& err)
	{
		_RPTN(_CRT_ERROR, "Bad allocation.\n%s\n", err.what());
		throw err;
	}
	catch (invalid_argument err)
	{
		// 関数の仮引数で不正な値を渡された場合に使用する。
		_RPTN(_CRT_ERROR, "Invalid argument.\n%s\n", err.what());
		throw err;
	}
	catch (runtime_error err)
	{
		// 論理エラーで取りきることのできない実行時エラー。
		_RPTN(_CRT_ERROR, "Runtime error.\n%s\n", err.what());
		throw err;
	}
	catch (domain_error err)
	{
		// 主に数学関連の処理で想定範囲外の値が渡された場合に使用する。
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
// 解放
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

// ナノ秒で返す
seconds_df CGameEngine::GetNanoTime(const chrono::system_clock::time_point tp_)
{
	return chrono::duration_cast<seconds_df>(chrono::duration_cast<microseconds_df>(tp_.time_since_epoch()));
}