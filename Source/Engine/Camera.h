//*****************************************************************************
// ƒtƒ@ƒCƒ‹–¼		:Camera.h
//*****************************************************************************

#pragma once
#include "../Temp/stdafx.h"
class CCamera
{
private:
	XMVECTORF32 eye;
	XMVECTORF32 at;
	XMVECTORF32 up;
	XMMATRIX m_matView;
	XMMATRIX m_matProj;
public:
	CCamera();
	~CCamera();

	VOID UpDate();
	XMMATRIX getMatView()const { return m_matView; }
	XMMATRIX getMatProj()const { return m_matProj; }
};

