//*****************************************************************************
// �t�@�C����		:Collision.h
//*****************************************************************************


#pragma once
#include "../../Temp/stdafx.h"
#include "../../Game/ObjectInfo.h"
//----------------------------------------------------------
// �{�b�N�X
//----------------------------------------------------------
struct BoundingBox
{
	XMFLOAT3 center;            // �{�b�N�X�̒��S
	XMFLOAT3 extents;           // ���S����i���ʂ܂ł̋���
};

//----------------------------------------------------------
// �e���v���[�g
//----------------------------------------------------------
template<class X>
shared_ptr<X> create(XMFLOAT3 n, XMFLOAT3 m) { return (shared_ptr<X>)(new X(n, m)); }
template<class X>
shared_ptr<X> create(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2) { return (shared_ptr<X>)(new X(v0, v1, v2)); }

//----------------------------------------------------------
// �����蔻��N���X
//----------------------------------------------------------
class CBox3D
{
private:
	BoundingBox bb;

public:
	CBox3D();
	CBox3D(Pos& pos_);
	CBox3D(Pos& pos_, Scale& scale_);
	~CBox3D();

	//----------------------------------------------------------
	// BB�X�V
	//----------------------------------------------------------
	VOID updatePos(const Pos& pos_);
	VOID updateExtents(const Scale& scale_);

	//----------------------------------------------------------
	// �Փ˔���̎擾
	//----------------------------------------------------------
	BoundingBox getBB()const;

	//----------------------------------------------------------
	// �Փ˔���	BB x BB
	//----------------------------------------------------------
	BOOL Intersects(CBox3D& target_)const;

	//---------------------------------------------------------
	// �Փ˔���	BB x Ray (_pDist:�^�[�Q�b�g�܂ł̋���)
	//---------------------------------------------------------
	// BOOL IntersectsRay(XMFLOAT3 _pos, XMFLOAT3 _dir, BoundingBox& _target, FLOAT* _pDist);
	
	//---------------------------------------------------------
	// �Փ˔��� BB x Triangle
	//---------------------------------------------------------
	// BOOL IntersecstTriangle(FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR V2);
};

/*
//---------------------------------------------------------
// �Փ˔��� Ray x Triangle
//---------------------------------------------------------
BOOL IntersectRayTriangle(XMFLOAT3& _pos, XMFLOAT3& _dir,
	XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, FLOAT* _pDist);
*/
