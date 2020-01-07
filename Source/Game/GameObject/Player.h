//**********************************************************
// �t�@�C����		:Player.h
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
	// �v���p�e�B -------------------------------------------
	// �ϊ��p���
	Pos pos;
	Scale scale;
	Angle angle;
	FLOAT MoveSpeed;


	// ���b�V���i�[�p
	shared_ptr<CStaticMesh> mesh;
	vector<shared_ptr<CPlayerBullet>> m_vBullet;
	CBox3D* myColl = NULL;

	//�E�F�[�u���[�V�����p
	DOUBLE anim;
	UINT currentBullentCnt = 0;
	BOOL isDead = FALSE;
	UINT hp = 3;

	// ���\�b�h -----------------------------------------------
	VOID RimitPos();	//�ړ�����
public:
	CPlayer();
	~CPlayer();

	VOID Render();		
	VOID SetMesh(shared_ptr<CStaticMesh>);	
	VOID SetBullet(vector<shared_ptr<CPlayerBullet>>);

	VOID Move(Direction);										//�ړ�
	VOID Shot();
	VOID Damage() { hp--; }
	VOID Respawn() { hp = 3; isDead = FALSE; }
	VOID Dead() { isDead = TRUE; }
	BOOL IsDead() const { return isDead == TRUE; };

	BOOL HitCheck(vector<shared_ptr<CEnemy>>);
	BOOL bulletHitCheck(UINT index_, vector<shared_ptr<CEnemy>> target_);
};

