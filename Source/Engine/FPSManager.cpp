#include "FPSManager.h"


CFPSManager::CFPSManager()
{
	//FPS固定******************************************************************************************************************************
	// メインループに入る前に精度を取得しておく
	QueryPerformanceFrequency(&timeFreq);
	// 1度取得しておく(初回計算用)
	QueryPerformanceCounter(&timeStart);

	fps = 0;
	//**************************************************************************************************************************************
}


CFPSManager::~CFPSManager()
{
}
VOID CFPSManager::SetFPS60()
{
	// 今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // 時間に余裕がある
									  // ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // 分解能を上げる(こうしないとSleepの精度はガタガタ)
		Sleep(sleepTime);   // 寝る
		timeEndPeriod(1);   // 戻す

							// 次週に持ち越し(こうしないとfpsが変になる?)
		return;
	}

	if (frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		fps = (fps*0.99f) + (0.01f / frameTime); // 平均fpsを計算


		timeStart = timeEnd; // 入れ替え

	}
}