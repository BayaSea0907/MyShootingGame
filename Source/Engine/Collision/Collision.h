//*****************************************************************************
// ファイル名		:Collision.h
//*****************************************************************************


#pragma once
#include "../../Temp/stdafx.h"
#include "../../Game/ObjectInfo.h"
//----------------------------------------------------------
// ボックス
//----------------------------------------------------------
struct BoundingBox
{
	XMFLOAT3 center;            // ボックスの中心
	XMFLOAT3 extents;           // 中心から格側面までの距離
};

//----------------------------------------------------------
// テンプレート
//----------------------------------------------------------
template<class X>
shared_ptr<X> create(XMFLOAT3 n, XMFLOAT3 m) { return (shared_ptr<X>)(new X(n, m)); }
template<class X>
shared_ptr<X> create(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2) { return (shared_ptr<X>)(new X(v0, v1, v2)); }

//----------------------------------------------------------
// 当たり判定クラス
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
	// BB更新
	//----------------------------------------------------------
	VOID updatePos(const Pos& pos_);
	VOID updateExtents(const Scale& scale_);

	//----------------------------------------------------------
	// 衝突判定の取得
	//----------------------------------------------------------
	BoundingBox getBB()const;

	//----------------------------------------------------------
	// 衝突判定	BB x BB
	//----------------------------------------------------------
	BOOL Intersects(CBox3D& target_)const;

	//---------------------------------------------------------
	// 衝突判定	BB x Ray (_pDist:ターゲットまでの距離)
	//---------------------------------------------------------
	// BOOL IntersectsRay(XMFLOAT3 _pos, XMFLOAT3 _dir, BoundingBox& _target, FLOAT* _pDist);
	
	//---------------------------------------------------------
	// 衝突判定 BB x Triangle
	//---------------------------------------------------------
	// BOOL IntersecstTriangle(FXMVECTOR V0, FXMVECTOR V1, FXMVECTOR V2);
};

/*
//---------------------------------------------------------
// 衝突判定 Ray x Triangle
//---------------------------------------------------------
BOOL IntersectRayTriangle(XMFLOAT3& _pos, XMFLOAT3& _dir,
	XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, FLOAT* _pDist);
*/
