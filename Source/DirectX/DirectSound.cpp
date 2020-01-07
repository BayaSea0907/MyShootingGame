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
// ������
//---------------------------------------------------------
HRESULT CDirectSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM�̏�����
	CoInitialize(NULL);

	// DirectSound8���쐬
	hr = DirectSoundCreate8(NULL, &lpDS, NULL);
	if (FAILED(hr)) 
	{
		return hr;
	}

	// �������[�h
	hr = lpDS->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
	if (FAILED(hr))
	{
		return hr;
	}

	CreatePrimaryBuffer();

	return hr;
}

//---------------------------------------------------------
// �I��
//---------------------------------------------------------
HRESULT CDirectSound::Exit()
{
	for (size_t i = 0; i < lpSB.size(); i++)
	{
		SAFE_RELEASE((*lpSB[i]));
		SAFE_DELETE(lpSB[i]);
	}

	SAFE_RELEASE(lpDS);

	// COM�̏I��
	CoUninitialize();

	return TRUE;
}
//----------------------------------------------------------
// �C���X�^���X���擾
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
// �C���X�^���X�����
//----------------------------------------------------------
VOID CDirectSound::ReleaseInstance()
{
	instance->Exit();
	SAFE_DELETE(instance);
}

//---------------------------------------------------------
// �v���C�}���T�E���h�o�b�t�@�̍쐬
//---------------------------------------------------------
HRESULT CDirectSound::CreatePrimaryBuffer()
{
	HRESULT hr;
	WAVEFORMATEX wf;

	// �v���C�}���T�E���h�o�b�t�@�̍쐬
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

	// �v���C�}���o�b�t�@�̃X�e�[�^�X������
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
// �T�E���h�o�b�t�@�̍쐬
//---------------------------------------------------------
HRESULT CDirectSound::CreateSecondaryBuffer(const char *fileName)
{
	HRESULT hr;
	MMCKINFO mSrcWaveData;
	LPWAVEFORMATEX wf;

	LPDIRECTSOUNDBUFFER* temp = new LPDIRECTSOUNDBUFFER;
	lpSB.push_back(temp);


	// WAV�t�@�C�������[�h
	HMMIO hSrc;
	LoadWavFile(hSrc, &mSrcWaveData, wf, fileName);

	// �T�E���h�o�b�t�@�̍쐬
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

	// ���b�N�J�n
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;

	hr = (*temp)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
	if (FAILED(hr)) 
	{
		free(wf);
		mmioClose(hSrc, 0);
		return hr;
	}

	// �f�[�^��������
	mmioRead(hSrc, (char*)pMem1, dwSize1);
	mmioRead(hSrc, (char*)pMem2, dwSize2);

	// ���b�N����
	(*temp)->Unlock(pMem1, dwSize1, pMem2, dwSize2);

	// �w�b�_�p���������J��
	free(wf);

	// WAV�����
	mmioClose(hSrc, 0);

	return hr;
}

//---------------------------------------------------------
// wav�t�@�C�������[�h
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

	// 'WAVE'�`�����N�`�F�b�N
	ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));
	hr = mmioDescend(_hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
	if (mSrcWaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		mmioClose(_hSrc, 0);
		return hr;
	}

	// 'fmt '�`�����N�`�F�b�N
	ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
	hr = mmioDescend(_hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
	if (mSrcWaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' '))
	{
		mmioClose(_hSrc, 0);
		return hr;
	}

	// �w�b�_�T�C�Y�̌v�Z
	int iSrcHeaderSize = mSrcWaveFmt.cksize;
	if (iSrcHeaderSize < sizeof(WAVEFORMATEX))
	{
		iSrcHeaderSize = sizeof(WAVEFORMATEX);
	}

	// �w�b�_�������m��
	_wf = (LPWAVEFORMATEX)malloc(iSrcHeaderSize);
	if (!_wf)
	{
		mmioClose(_hSrc, 0);
		return hr;
	}
	ZeroMemory(_wf, iSrcHeaderSize);

	// WAVE�t�H�[�}�b�g�̃��[�h
	hr = mmioRead(_hSrc, (char*)_wf, mSrcWaveFmt.cksize);
	if (FAILED(hr))
	{
		free(_wf);
		mmioClose(_hSrc, 0);
		return hr;
	}

	// fmt�`�����N�ɖ߂�
	mmioAscend(_hSrc, &mSrcWaveFmt, 0);

	// data�`�����N��T��
	while (1)
	{
		// ����
		hr = mmioDescend(_hSrc, _mSrcWaveData, &mSrcWaveFile, 0);

		if (FAILED(hr))
		{
			free(_wf);
			mmioClose(_hSrc, 0);
			return hr;
		}
		if (_mSrcWaveData->ckid == mmioStringToFOURCCA("data", 0))
			break;
		// ���̃`�����N��
		hr = mmioAscend(_hSrc, _mSrcWaveData, 0);
	}

	return hr;
}

//---------------------------------------------------------
// �Đ�
//---------------------------------------------------------
VOID CDirectSound::Play(UINT _index)
{
	(*lpSB[_index])->Play(0, 0, 0);
}

//---------------------------------------------------------
// �Đ�	���[�v�L��
//---------------------------------------------------------
VOID CDirectSound::PlayLoop(UINT _index)
{
	(*lpSB[_index])->Play(0, 0, DSBPLAY_LOOPING);
}

//---------------------------------------------------------
// ��~(���̏��)
//---------------------------------------------------------
VOID CDirectSound::Stop(UINT _index)
{
	(*lpSB[_index])->Stop();
}

//---------------------------------------------------------
// �����ʒu�ɖ߂�
//---------------------------------------------------------
VOID CDirectSound::Reset(UINT _index)
{
	(*lpSB[_index])->SetCurrentPosition(0);
}

