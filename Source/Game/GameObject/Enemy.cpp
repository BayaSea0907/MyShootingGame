#include "Enemy.h"


CEnemy::CEnemy()
{
	pos = { 0.0f, 0.0f, 0.0f };
	angle = { 0.0f,0.0f,0.0f };
	scale = { 0.06f,0.06f,0.06f };
	moveSpeed = 0.07f;

	Scale collScale = { 0.3f, 0.3f, 0.3f };
	myColl = new CBox3D(pos, collScale);
}


CEnemy::~CEnemy()
{
	SAFE_DELETE(myColl);
}
VOID CEnemy::SetMesh(shared_ptr<CStaticMesh> _mesh)
{
	mesh = _mesh;
}

VOID CEnemy::SetPosition(Pos pos_)
{
	pos = pos_;
}

CBox3D& CEnemy::GetCollision()const
{
	return *myColl;
}


VOID CEnemy::Render()
{
	myColl->updatePos(pos);

	angle.Z -= 0.0005f;
	angle.X -= 0.0001f;

	XMMATRIX Scale = XMMatrixScaling(scale.X, scale.Y, scale.Z);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(angle.X, angle.Y, angle.Z);
	XMMATRIX Tran = XMMatrixTranslation(pos.X, pos.Y, pos.Z);
 	XMMATRIX WorldMatrix = Scale * Rotation * Tran;

	pos.Z += moveSpeed;



	// �ʂ�߂��Ă�������A���̏ꏊ�����΂�
	if (pos.Z >= RIMIT_LEFT_POS_Z)
	{
		pos.Z = -190.0f;
		isDead = FALSE;
	}

	// ����łȂ������A�J�����Ɏʂ��Ă�Ԃ̂ݕ`����s��
	if ( !IsDead() && ((RIMIT_LEFT_POS_Z + 2.0 > pos.Z) && (pos.Z >= RIMIT_RIGHT_POS_Z - 20.0)))
	{
		mesh->render(&WorldMatrix);
	}
}
