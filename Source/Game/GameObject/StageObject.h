//**********************************************************
// �t�@�C����		:StageObject.h
//**********************************************************

#pragma once
#include "../../Engine/Render/StaticMesh.h"
#include "../../Temp/stdafx.h"
#include "../ObjectInfo.h"

#define SCROLL_COUNT 6
#define RESET_POS 100 - (SCROLL_COUNT * 100)

class StageObject
{
	// �v���p�e�B -------------------------------------------
	// �ϊ��p���
	Pos pos[SCROLL_COUNT];
	Scale scale;
	Angle angle;


	shared_ptr<CStaticMesh> mesh;

	VOID ResetPos(FLOAT&);
public:
	StageObject();
	~StageObject();
	VOID Render();
	VOID SetMesh(shared_ptr<CStaticMesh>);
};

