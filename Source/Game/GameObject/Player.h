//**********************************************************
// ファイル名		:Player.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"
#include "../../Engine/Render/StaticMesh.h"
#include "../../Engine/Collision/Collision.h"
#include "../ObjectInfo.h"
#include "Enemy.h"
#include "PlayerBullet.h"

enum Direction {
	Up,
	Down,
	Right,
	Left
};

class CPlayer
{
private:
	// プロパティ -------------------------------------------
	// 変換用情報
	Pos pos;
	Scale scale;
	Angle angle;
	FLOAT MoveSpeed;


	// メッシュ格納用
	shared_ptr<CStaticMesh> mesh;
	vector<shared_ptr<CPlayerBullet>> m_vBullet;
	CBox3D* myColl = NULL;

	//ウェーブモーション用
	DOUBLE anim;
	UINT currentBullentCnt = 0;
	BOOL isDead = FALSE;
	UINT hp = 3;

	// メソッド -----------------------------------------------
	VOID RimitPos();	//移動制限
public:
	CPlayer();
	~CPlayer();

	VOID Render();		
	VOID SetMesh(shared_ptr<CStaticMesh>);	
	VOID SetBullet(vector<shared_ptr<CPlayerBullet>>);

	VOID Move(Direction);										//移動
	VOID Shot();
	VOID Damage() { hp--; }
	VOID Respawn() { hp = 3; isDead = FALSE; }
	VOID Dead() { isDead = TRUE; }
	BOOL IsDead() const { return isDead == TRUE; };

	BOOL HitCheck(vector<shared_ptr<CEnemy>>);
	BOOL bulletHitCheck(UINT index_, vector<shared_ptr<CEnemy>> target_);
};

