//**********************************************************
// �t�@�C����		:FPSManager.h
//**********************************************************

#pragma once
#include "../Temp/stdafx.h"

class CFPSManager
{
	//FPS�Œ�p
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;
	float fps = 0;

public:
	CFPSManager();
	~CFPSManager();
	VOID SetFPS60();
};

