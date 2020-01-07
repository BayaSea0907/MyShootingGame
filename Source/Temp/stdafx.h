// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//
#pragma once
#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから使用されていない部分を除外します。
// Windows ヘッダー ファイル:
#include <windows.h>
// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <vector>
#include<iostream>
#include<string>
#include<fstream>
#include <iomanip>
#include <sstream>
#include <conio.h>

using namespace std;
// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#pragma warning(disable:4005)
#pragma warning(disable:4316)
#pragma warning(disable:4838)
// DXGI関連の定義の停止<XNAMath>
#undef DXGI_STATUS_OCCLUDED
#undef DXGI_STATUS_CLIPPED
#undef DXGI_STATUS_NO_REDIRECTION
#undef DXGI_STATUS_NO_DESKTOP_ACCESS
#undef DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_STATUS_MODE_CHANGED
#undef DXGI_STATUS_MODE_CHANGE_IN_PROGRESS
#undef DXGI_ERROR_INVALID_CALL
#undef DXGI_ERROR_NOT_FOUND
#undef DXGI_ERROR_MORE_DATA
#undef DXGI_ERROR_UNSUPPORTED
#undef DXGI_ERROR_DEVICE_REMOVED
#undef DXGI_ERROR_DEVICE_HUNG
#undef DXGI_ERROR_DEVICE_RESET
#undef DXGI_ERROR_WAS_STILL_DRAWING
#undef DXGI_ERROR_FRAME_STATISTICS_DISJOINT
#undef DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_ERROR_DRIVER_INTERNAL_ERROR
#undef DXGI_ERROR_NONEXCLUSIVE
#undef DXGI_ERROR_NOT_CURRENTLY_AVAILABLE
#undef DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED
#undef DXGI_ERROR_REMOTE_OUTOFMEMORY
#undef D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D11_ERROR_FILE_NOT_FOUND
#undef D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
#undef D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD
#undef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D10_ERROR_FILE_NOT_FOUND
#include <mmsystem.h>
#include <stdio.h>
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include <fbxsdk.h>
#include <string.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx11d.lib" )
#else
#pragma comment( lib, "d3dx11.lib" )
#endif
#pragma comment( lib, "dxgi.lib" ) // 11/18 16:13
//定数定義
#define WINDOW_WIDTH  1920 //ウィンドウ幅
#define WINDOW_HEIGHT 1080 //ウィンドウ高さ
//define
#define SAFE_RELEASE(x)		 { if(x) { x->Release(); x = NULL; } }
#define SAFE_DELETE(x)		 { if(x) { delete x; x = 0; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] x; x = 0; } }

#define MAX_LOADSTRING 100
#define WAIT_ATACK 3000				//攻撃のインターバル
//使う距離の二乗で書いてください。
#define MAX_TRACK_DISTANSE 5000.0f	//追尾する距離
#define MIN_TRACK_DISTANSE 50.0f	//追尾やめる距離

#define PI 3.1415926f				//円周率
#define UI_NUM	1					// UI表示数

#define PLAYER_NUM 1
#define PLAYER_BULLET_NUM 15
#define STAGE_NUM		1//ステージオブジェクト数
#define ENEMY_NUM		20	//エネミーの数

#define FBX_NUM PLAYER_NUM + STAGE_NUM + ENEMY_NUM + PLAYER_BULLET_NUM		//FBX表示数

#define RIMIT_LEFT_POS_Z  7
#define RIMIT_RIGHT_POS_Z -6.8
#define RIMIT_UP_POS_Y 11.8
#define RIMIT_DOWN_POS_Y 4

//テンプレート
template <class X>
shared_ptr<X>  create() { return (shared_ptr<X>(new X)); }

template <class X>
shared_ptr<X>  create(wchar_t* c) { return (shared_ptr<X>(new X(c))); }