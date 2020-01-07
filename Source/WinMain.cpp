#pragma once

#include "Temp/stdafx.h"
#include "Engine\GameEngine.h"
#define APP_NAME L"MeteorBreak"

//----------------------------------------------------------
// �֐��v���g�^�C�v
//----------------------------------------------------------
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND InitWindow(HINSTANCE _hInst, WNDPROC _wndProc,
	INT _minX, INT _minY, INT _maxX, INT _maxY, LPCWSTR _windowName);

//----------------------------------------------------------
//�A�v���P�[�V�����̃G���g���[�֐�
//----------------------------------------------------------
int APIENTRY wWinMain(
	_In_ HINSTANCE _hInst,
	_In_opt_ HINSTANCE _hPrevInst,
	_In_ LPWSTR _lpCmdLine,
	_In_ int _iCmdShow)
{
	UNREFERENCED_PARAMETER(_hPrevInst);
	UNREFERENCED_PARAMETER(_lpCmdLine);

	// �E�C���h�E�쐬
	HWND hWnd = InitWindow(_hInst, WndProc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);

	// �G���W���쐬
	CGameEngine* GameEngine = new CGameEngine();
	GameEngine->Initialize(hWnd);

	// �Q�[���̍쐬
	CGameManager* GameManager = new CGameManager();
	GameManager->Initialize();

	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	// ���b�Z�[�W���[�v
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GameEngine != NULL)
			{
				GameEngine->Update();
				GameManager->Update();
			}
		}
	}
	//�A�v���P�[�V�����̏I��
	GameEngine->Release();
	delete GameEngine;

	return 0;
}

//----------------------------------------------------------
// �E�C���h�E�쐬
//----------------------------------------------------------
HWND InitWindow(HINSTANCE _hInst, WNDPROC _wndProc,
	INT _minX, INT _minY, INT _maxX, INT _maxY, LPCWSTR _windowName)
{

	// �E�B���h�E�̒�`
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = _wndProc;
	wc.hInstance = _hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = _windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//�E�B���h�E�̍쐬
	 HWND hWnd = CreateWindow(_windowName, _windowName, WS_OVERLAPPEDWINDOW,
		_minX, _minY, _maxX, _maxY, 0, 0, _hInst, 0);
	if (!hWnd)
	{
		throw E_FAIL;
	}

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// �J�[�\���\���؂�ւ�
	ShowCursor(1);

	return hWnd;
}

//----------------------------------------------------------
// �󂯎�肽�����b�Z�[�W
//----------------------------------------------------------
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)_wParam)
		{
		case VK_ESCAPE://ESC�L�[�ŏI��
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(_hWnd, _iMsg, _wParam, _lParam);
}
