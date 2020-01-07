#include "DirectSound.h"

CDirectSound*	CDirectSound::instance = NULL;

CDirectSound::CDirectSound()
{
}

CDirectSound::~CDirectSound()
{
	Exit();
}

//---------------------------------------------------------
// 初期化
//---------------------------------------------------------
HRESULT CDirectSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMの初期化
	CoInitialize(NULL);

	// DirectSound8を作成
	hr = DirectSoundCreate8(NULL, &lpDS, NULL);
	if (FAILED(hr)) 
	{
		return hr;
	}

	// 強調モード
	hr = lpDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		return hr;
	}

	CreatePrimaryBuffer();

	return hr;
}

//---------------------------------------------------------
// 終了
//---------------------------------------------------------
HRESULT CDirectSound::Exit()
{
	for (size_t i = 0; i < lpSB.size(); i++)
	{
		SAFE_RELEASE((*lpSB[i]));
		SAFE_DELETE(lpSB[i]);
	}

	SAFE_RELEASE(lpDS);

	// COMの終了
	CoUninitialize();

	return TRUE;
}
//----------------------------------------------------------
// インスタンスを取得
//----------------------------------------------------------
CDirectSound* CDirectSound::GetInstance(HWND hWnd_)
{
	if (instance == NULL)
	{
		instance = new CDirectSound();
		instance->Init(hWnd_);
	}
	return instance;
}

//----------------------------------------------------------
// インスタンスを解放
//----------------------------------------------------------
VOID CDirectSound::ReleaseInstance()
{
	instance->Exit();
	SAFE_DELETE(instance);
}

//---------------------------------------------------------
// プライマリサウンドバッファの作成
//---------------------------------------------------------
HRESULT CDirectSound::CreatePrimaryBuffer()
{
	HRESULT hr;
	WAVEFORMATEX wf;

	// プライマリサウンドバッファの作成
	DSBUFFERDESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));

	dsdesc.dwSize		 = sizeof(DSBUFFERDESC);
	dsdesc.dwFlags		 = DSBCAPS_PRIMARYBUFFER;
	dsdesc.dwBufferBytes = 0;
	dsdesc.lpwfxFormat   = NULL;

	hr = lpDS->CreateSoundBuffer(&dsdesc, &lpPrimaryBuffer, NULL);
	if (FAILED(hr)) 
	{
		return hr;
	}

	// プライマリバッファのステータスを決定
	wf.cbSize = sizeof(WAVEFORMATEX);
	wf.wFormatTag	     = WAVE_FORMAT_PCM;
	wf.nChannels         = 2;
	wf.nSamplesPerSec    = 44100;
	wf.wBitsPerSample    = 16;
	wf.nBlockAlign		 = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec   = wf.nSamplesPerSec * wf.nBlockAlign;

	hr = lpPrimaryBuffer->SetFormat(&wf);
	if (FAILED(hr)) 
	{
		return hr;
	}

	return hr;
}

//---------------------------------------------------------
// サウンドバッファの作成
//---------------------------------------------------------
HRESULT CDirectSound::CreateSecondaryBuffer(const char *fileName)
{
	HRESULT hr;
	MMCKINFO mSrcWaveData;
	LPWAVEFORMATEX wf;

	LPDIRECTSOUNDBUFFER* temp = new LPDIRECTSOUNDBUFFER;
	lpSB.push_back(temp);


	// WAVファイルをロード
	HMMIO hSrc;
	LoadWavFile(hSrc, &mSrcWaveData, wf, fileName);

	// サウンドバッファの作成
	DSBUFFERDESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));

	dsdesc.dwSize		   = sizeof(DSBUFFERDESC);
	dsdesc.dwFlags		   = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
	dsdesc.dwBufferBytes   = mSrcWaveData.cksize;
	dsdesc.lpwfxFormat     = wf;
	dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;

	hr = lpDS->CreateSoundBuffer(&dsdesc, temp, NULL);
	if (FAILED(hr)) 
	{
		free(wf);
		mmioClose(hSrc, 0);
		return hr;
	}

	// ロック開始
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;

	hr = (*temp)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
	if (FAILED(hr)) 
	{
		free(wf);
		mmioClose(hSrc, 0);
		return hr;
	}

	// データ書き込み
	mmioRead(hSrc, (char*)pMem1, dwSize1);
	mmioRead(hSrc, (char*)pMem2, dwSize2);

	// ロック解除
	(*temp)->Unlock(pMem1, dwSize1, pMem2, dwSize2);

	// ヘッダ用メモリを開放
	free(wf);

	// WAVを閉じる
	mmioClose(hSrc, 0);

	return hr;
}

//---------------------------------------------------------
// wavファイルをロード
//---------------------------------------------------------
HRESULT CDirectSound::LoadWavFile(HMMIO& _hSrc, MMCKINFO* _mSrcWaveData, LPWAVEFORMATEX &_wf, const char *fileName)
{
	HRESULT hr;
	MMCKINFO mSrcWaveFile;
	MMCKINFO mSrcWaveFmt;

	_hSrc = mmioOpenA((LPSTR)fileName, NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
	if (!_hSrc)
	{
		return 0;
	}

	// 'WAVE'チャンクチェック
	ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));
	hr = mmioDescend(_hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
	if (mSrcWaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		mmioClose(_hSrc, 0);
		return hr;
	}

	// 'fmt 'チャンクチェック
	ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
	hr = mmioDescend(_hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
	if (mSrcWaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' '))
	{
		mmioClose(_hSrc, 0);
		return hr;
	}

	// ヘッダサイズの計算
	int iSrcHeaderSize = mSrcWaveFmt.cksize;
	if (iSrcHeaderSize < sizeof(WAVEFORMATEX))
	{
		iSrcHeaderSize = sizeof(WAVEFORMATEX);
	}

	// ヘッダメモリ確保
	_wf = (LPWAVEFORMATEX)malloc(iSrcHeaderSize);
	if (!_wf)
	{
		mmioClose(_hSrc, 0);
		return hr;
	}
	ZeroMemory(_wf, iSrcHeaderSize);

	// WAVEフォーマットのロード
	hr = mmioRead(_hSrc, (char*)_wf, mSrcWaveFmt.cksize);
	if (FAILED(hr))
	{
		free(_wf);
		mmioClose(_hSrc, 0);
		return hr;
	}

	// fmtチャンクに戻る
	mmioAscend(_hSrc, &mSrcWaveFmt, 0);

	// dataチャンクを探す
	while (1)
	{
		// 検索
		hr = mmioDescend(_hSrc, _mSrcWaveData, &mSrcWaveFile, 0);

		if (FAILED(hr))
		{
			free(_wf);
			mmioClose(_hSrc, 0);
			return hr;
		}
		if (_mSrcWaveData->ckid == mmioStringToFOURCCA("data", 0))
			break;
		// 次のチャンクへ
		hr = mmioAscend(_hSrc, _mSrcWaveData, 0);
	}

	return hr;
}

//---------------------------------------------------------
// 再生
//---------------------------------------------------------
VOID CDirectSound::Play(UINT _index)
{
	(*lpSB[_index])->Play(0, 0, 0);
}

//---------------------------------------------------------
// 再生	ループ有り
//---------------------------------------------------------
VOID CDirectSound::PlayLoop(UINT _index)
{
	(*lpSB[_index])->Play(0, 0, DSBPLAY_LOOPING);
}

//---------------------------------------------------------
// 停止(その場で)
//---------------------------------------------------------
VOID CDirectSound::Stop(UINT _index)
{
	(*lpSB[_index])->Stop();
}

//---------------------------------------------------------
// 初期位置に戻す
//---------------------------------------------------------
VOID CDirectSound::Reset(UINT _index)
{
	(*lpSB[_index])->SetCurrentPosition(0);
}

