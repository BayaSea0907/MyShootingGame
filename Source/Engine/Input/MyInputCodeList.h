//**********************************************************
// ファイル名		:MyInputCodeList.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"

// DirectX 入力判断用コード
enum InputCode
{

	KEY_ESC = 0x01,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_TAB = 0x0F,
	KEY_Q = 0x10,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_RETURN = 0x1C,
	KEY_LCONTROL = 0x1D,
	KEY_A = 0x1E,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_LSHIFT = 0x2A,
	KEY_BACKSLASH = 0x2B,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_RSHIFT = 0x36,
	KEY_SPACE = 0x39,
	KEY_F1 = 0x3B,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F12,
	KEY_F13,
	KEY_UP = 0xC8,
	KEY_LEFT = 0xCB,
	KEY_RIGHT = 0xCD,
	KEY_END = 0xCF,
	KEY_DOWN = 0xD0,
	KEY_NEXT = 0xD1,
	KEY_INSERT = 0xD2,

	//--------------------------------------------------
	// マウス
	//--------------------------------------------------
	MOUSE_L_CRICK = 0,
	MOUSE_R_CRICK,
	MOUSE_WHEEL,
	MOUSE_SIDE_BUTTON1,
	MOUSE_SIDE_BUTTON2,
	MOUSE_X = 1000,
	MOUSE_Y,
	MOUSE_Z,

	//--------------------------------------------------
	// LOGICOOLパッド
	//--------------------------------------------------
	PAD_X = 0,
	PAD_A,
	PAD_B,
	PAD_Y,
	PAD_L1,
	PAD_R1,
	PAD_L2,
	PAD_R2,
	PAD_BACK,
	PAD_START,

	PAD_LX = 2000,
	PAD_LY,
	PAD_RX,
	PAD_RY,

	PAD_UP,
	PAD_DOWN,
	PAD_RIGHT,
	PAD_LEFT,
};