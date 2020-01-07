//**********************************************************
// ファイル名		:Enemy.h
//**********************************************************

#pragma once
#include "../../Engine/Render/StaticMesh.h"
#include "../../Engine/Collision/Collision.h"
#include "../../Temp/stdafx.h"
#include "../ObjectInfo.h"

#define N 256

class CEnemy
{
	// プロパティ -------------------------------------------
	// 変換用情報
	Pos pos;
	Scale scale;
	Angle angle;
	FLOAT moveSpeed;

	BOOL isDead = FALSE;

	shared_ptr<CStaticMesh> mesh;
	CBox3D* myColl = NULL;

public:
	CEnemy();
	~CEnemy();

	VOID Render();
	VOID SetMesh(shared_ptr<CStaticMesh>);
	VOID SetPosition(Pos pos_);
	CBox3D& GetCollision()const;

	VOID Dead() { isDead = TRUE; }
	BOOL IsDead() const { return isDead == TRUE; };
};

