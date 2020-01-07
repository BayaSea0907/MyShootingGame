//**********************************************************
// ファイル名		:Input.h
//**********************************************************
#pragma once
#include "../../Temp/stdafx.h"
#include "MyInputCodeList.h"

struct InputFlag
{
	int frame;
	bool down;
	bool once;
	bool up;
	bool off;

	InputFlag()
	{
		frame = 0;
		down = false;
		once = false;
		up = false;
		off = false;
	}
};

struct InputInfo
{		
	InputFlag* key;			// InputEngineで配列の先頭アドレスをぶち込みます
	InputFlag* mouse;		// 
	InputFlag* pad;			// 
	XMFLOAT3 mouseMoveValue;
	XMFLOAT3 rStickValue;
	XMFLOAT3 lStickValue;
};

// 入力インターフェース
class CInput
{
private:
	static CInput* instance;	// シングルトン
	CInput(){}
	~CInput(){}

public:
	void Initialize();
	void Release();

	InputInfo inputInfo;
	static CInput& GetInstance() { return *instance; }

	// キーボード処理
	bool GetKey(InputCode code_)const;
	bool GetKeyDown(InputCode code_)const;
	bool GetKeyUp(InputCode code_)const;
	bool GetKeyOff(InputCode code_) const;

	// マウス入力判断
	bool GetMouseButton(InputCode code_)const;
	bool GetMouseButtonDown(InputCode code_)const;
	bool GetMouseButtonUp(InputCode code_)const;
	bool GetMouseButtonOff(InputCode code_)const;

	// ゲームパッド入力判断
	bool GetButton(InputCode code_)const;
	bool GetButtonDown(InputCode code_)const;
	bool GetButtonUp(InputCode code_)const;
	bool GetButtonOff(InputCode code_)const;
	
	// ゲームパッド十字キー判断用
	bool GetButtonDirection(InputCode code_)const;

	// マウス・スティックの入力量取得 -1~1の範囲の値を返す
	float GetAxis(InputCode cpde_)const;
};

