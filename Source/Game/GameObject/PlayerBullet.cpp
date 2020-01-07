#include "../../Temp/stdafx.h"
#include "PlayerBullet.h"


CPlayerBullet::CPlayerBullet()
{
	pos = { -20.0f,7.0f, 4.0f };
	angle = { -1.5f,0.0f,0.0f };
	scale = { 0.7f,0.6f,0.7f };
	bulletSpeed = 0.06f;

	Scale temp = { 0.2f, 0.3f, 0.2f };
	myColl = new CBox3D(pos, temp);
}


CPlayerBullet::~CPlayerBullet()
{
	SAFE_DELETE(myColl);
}

VOID CPlayerBullet::SetMesh(shared_ptr<CStaticMesh> mesh_)
{
	m_mesh = mesh_;
}

VOID CPlayerBullet::Action(Pos pos_)
{
	if (isShot == FALSE)
	{
		SetPosition(pos_);
		isShot = TRUE;
	}
}
VOID CPlayerBullet::SetPosition(Pos pos_)
{
	pos = { pos_.X, pos_.Y - 0.01f, pos_.Z - 0.2f };
}

BOOL CPlayerBullet::HitCheck(vector<shared_ptr<CEnemy>> target_)
{
	BOOL hit = FALSE;

	myColl->updatePos(pos);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (myColl->Intersects(target_[i]->GetCollision()) && !target_[i]->IsDead())
		{
			hit = TRUE;
			Dead();
			target_[i]->Dead();
			break;
		}
	}

	return hit;
}

VOID CPlayerBullet::Render()
{
	if (!IsDead())
	{
		pos.Z -= bulletSpeed;
	}

	XMMATRIX Scale = XMMatrixScaling(scale.X, scale.Y, scale.Z);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(angle.X, angle.Y, angle.Z);
	XMMATRIX Tran = XMMatrixTranslation(pos.X, pos.Y, pos.Z);
	XMMATRIX WorldMatrix = Scale * Rotation * Tran;

	// åÇÇΩÇÍÇƒÇ¢ÇƒâÊñ äOÇ…Ç†ÇÈéûÇÕï`âÊÇµÇ»Ç¢ÅBìGÇ∆Ç‘Ç¬Ç©Ç¡ÇΩÇ∆Ç´Ç‡ï`âÊÇµÇ»Ç¢ÅB
	if (((isShot == TRUE) && (pos.Z < RIMIT_RIGHT_POS_Z)) || (isDead == TRUE))
	{
		pos.Z = 4.0f;
		isShot = FALSE;
		isDead = FALSE;
	}
	else
	{
		m_mesh->render(&WorldMatrix);
	}
}