#include "../../Temp/stdafx.h"
#include "Player.h"



CPlayer::CPlayer()
{
	pos   = { -20.0f,7.0f, 4.0f };
	angle = { 0.0f,0.0f,0.0f };
	scale = { 0.05f,0.05f,0.05f };
	MoveSpeed = 0.03f;
	anim = 0;
	myColl = new CBox3D(pos);
}
CPlayer::~CPlayer()
{
	SAFE_DELETE(myColl);
}
VOID CPlayer::SetMesh(shared_ptr<CStaticMesh> _mesh)
{
	mesh = _mesh;
}

VOID CPlayer::SetBullet(vector<shared_ptr<CPlayerBullet>> bullet_)
{
	for (int i = 0; i < PLAYER_BULLET_NUM; i++)
	{
		m_vBullet.push_back(bullet_[i]);
	}
}

VOID CPlayer::Move(Direction MoveMode)
{
	switch (MoveMode)
	{
	case Up:		
		pos.Y += MoveSpeed;
		angle.Z = -0.2f;
		break;
	case Down:
		pos.Y -= MoveSpeed;
		angle.Z =  1.0f;
		break;
	case Right:
		pos.Z -= MoveSpeed;	
		break;
	case Left:
		pos.Z += MoveSpeed;
		break;
	default:
		angle.Z = 0.0f;
		break;
	}
	RimitPos();
}

VOID CPlayer::Shot()
{
	if (currentBullentCnt >= PLAYER_BULLET_NUM)
	{
		currentBullentCnt = 0;
	}
	m_vBullet[currentBullentCnt]->Action(pos);
	currentBullentCnt++;
}

BOOL CPlayer::HitCheck(vector<shared_ptr<CEnemy>> target_)
{
	BOOL hit = FALSE;

	myColl->updatePos(pos);

	// プレイヤーとエネミーの当たり判定
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (myColl->Intersects(target_[i]->GetCollision()) && !target_[i]->IsDead())
		{
			hit = TRUE;
			Dead();
			break;
		}
	}

	for (int i = 0; i < PLAYER_BULLET_NUM; i++)
	{
		// バレットが放たれているか？
		if (m_vBullet[i]->IsShot())
		{
			if (bulletHitCheck(i, target_))
			{
				hit = TRUE;
			}
		}
	}

	return hit;
}

//---------------------------------------------------------
// バレットと敵の当たり判定を取る
//---------------------------------------------------------
BOOL CPlayer::bulletHitCheck(UINT index_, vector<shared_ptr<CEnemy>> target_)
{
	return m_vBullet[index_]->HitCheck(target_);
}


VOID CPlayer::Render()
{
	anim += 0.001;
	double degree = anim * 180.0 / PI;
	pos.Y += (FLOAT)(sin(degree) * 0.005);

	XMMATRIX Scale = XMMatrixScaling(scale.X, scale.Y, scale.Z);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(angle.X, angle.Y, angle.Z);
	XMMATRIX Tran = XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	XMMATRIX WorldMatrix = Scale * Rotation * Tran;

	// 死んでなければ自身とバレットを描画
	if (isDead == FALSE)
	{
		for (int i = 0; i < PLAYER_BULLET_NUM; i++)
		{
			if (m_vBullet[i]->IsShot()) m_vBullet[i]->Render();
		}
		mesh->render(&WorldMatrix);
	}
}
VOID CPlayer::RimitPos()
{
	if (pos.Z > RIMIT_LEFT_POS_Z)	pos.Z = RIMIT_LEFT_POS_Z;
	if (pos.Z < RIMIT_RIGHT_POS_Z)	pos.Z = RIMIT_RIGHT_POS_Z;

	if (pos.Y > RIMIT_UP_POS_Y)	pos.Y = RIMIT_UP_POS_Y;
	if (pos.Y < RIMIT_DOWN_POS_Y)	pos.Y = RIMIT_DOWN_POS_Y;
}




