//**********************************************************
// ファイル名		:PlayerBullet.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"
#include "../../Engine/Render/StaticMesh.h"
#include "../../Engine/Collision/Collision.h"
#include "../ObjectInfo.h"
#include "Enemy.h"

class CPlayerBullet
{
private:

	// プロパティ -------------------------------------------
	// 変換用情報
	Pos   pos;
	Scale scale;
	Angle angle;
	FLOAT bulletSpeed;
	BOOL isDead = FALSE;
	BOOL isShot = FALSE;

	shared_ptr<CStaticMesh> m_mesh;
	CBox3D* myColl = NULL;

	VOID SetPosition(Pos);
public:
	CPlayerBullet();
	~CPlayerBullet();

	VOID Action(Pos);
	VOID Render();							//描画
	VOID SetMesh(shared_ptr<CStaticMesh>);	//メッシュ設定
	BOOL HitCheck(vector<shared_ptr<CEnemy>> target_);

	VOID Dead() { isDead = TRUE; }
	BOOL IsShot() const { return isShot == TRUE; };
	BOOL IsDead() const { return isDead == TRUE; };
};
