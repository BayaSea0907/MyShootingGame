//**********************************************************
// ファイル名		：GameEngine.h
//**********************************************************

#pragma once
#include <locale>
#include <chrono>

#include "../Temp/stdafx.h"
#include "../DirectX/Direct3D.h"
#include "../DirectX/DirectInput.h"
#include "../DirectX/DirectSound.h"
#include "Input/InputEngine.h"
#include "../Game/GameManager.h"
#include "FPSManager.h"


// 時間定義
using microseconds_df = chrono::duration<double, std::ratio<1, 1000000>>;
using seconds_df = chrono::duration<double, std::ratio<1, 1>>;
using namespace std;

class CGameEngine
{
public:
	CGameEngine();
	~CGameEngine();

	void Initialize(HWND _hWnd);
	void Update();
	void Release();

private:
	CInputEngine* InputEngine;
	CDirect3D* RenderEngine;
	CDirectSound* SoundEngine;
	
	seconds_df nowTime;
	seconds_df deltaTime;
	seconds_df GetNanoTime(const chrono::system_clock::time_point tp_);
};

