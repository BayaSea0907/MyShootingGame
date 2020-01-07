#include "StageObject.h"


StageObject::StageObject()
{
	for (int i = 0; i < SCROLL_COUNT; i++)
	{
		pos[i] = { 0.0f, 0.0f, -(i * 100.0f) + 100.0f };
	}
	angle = { 0.0f,0.0f,0.0f };
	scale = { 0.1f,0.1f,0.1f };

}


StageObject::~StageObject()
{
}
VOID StageObject::SetMesh(shared_ptr<CStaticMesh> _mesh)
{
	mesh = _mesh;
}
VOID StageObject::Render()
{
	XMMATRIX Scale = XMMatrixScaling(scale.X, scale.Y, scale.Z);
	XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(angle.X, angle.Y, angle.Z);
	for (int i = 0; i < SCROLL_COUNT; i++)
	{
		XMMATRIX Tran = XMMatrixTranslation(pos[i].X, pos[i].Y, pos[i].Z);
		XMMATRIX WorldMatrix = Scale * Rotation * Tran;
		mesh->render(&WorldMatrix);
		pos[i].Z += 0.1f;
		if ((int)pos[i].Z == 100)
		{
			ResetPos(pos[i].Z);
		}
	}
	
}
VOID StageObject::ResetPos(FLOAT& pos)
{
	pos = RESET_POS;
}

