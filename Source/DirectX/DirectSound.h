//**********************************************************
// ファイル名		:DirectSound.h
//**********************************************************

#pragma once
#pragma comment(lib, "dsound.lib")
#include <vector>
#include <dsound.h>
#include "../Temp/stdafx.h"

class CDirectSound
{
private:
	static CDirectSound* instance;

	LPDIRECTSOUND8		lpDS = NULL;
	LPDIRECTSOUNDBUFFER lpPrimaryBuffer;
	std::vector<LPDIRECTSOUNDBUFFER*> lpSB;

	HRESULT Init(HWND hWnd);
	HRESULT CreatePrimaryBuffer();
	HRESULT LoadWavFile(HMMIO& _hSrc, MMCKINFO* _mSrcWaveData, LPWAVEFORMATEX &_wf, const char *fileName);
public:
	CDirectSound();
	~CDirectSound();

	// シングルトンパターン
	static CDirectSound* GetInstance(HWND hWnd_);
	static VOID	ReleaseInstance();

	HRESULT CreateSecondaryBuffer(const char *fileName);		// サウンドバッファ作成　※Waveファイルに限る
	VOID Play(UINT _index);
	VOID PlayLoop(UINT _index);
	VOID Stop(UINT _index);
	VOID Reset(UINT _index);
	HRESULT Exit();
};

