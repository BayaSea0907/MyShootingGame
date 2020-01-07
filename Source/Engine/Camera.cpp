#include "Camera.h"


CCamera::CCamera()
{
	eye = { -30.0f,  8.0f, 0.0f };	
	at  = {  30.0f,  8.0f, 0.0f };
	up  = {  0.0f,  1.0f,   0.0f };
}
CCamera::~CCamera()
{

}
VOID CCamera::UpDate()
{
	m_matView = XMMatrixLookAtLH(eye, at, up);
	m_matProj = XMMatrixPerspectiveFovLH((float)XM_PI / 4, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, -4000.0f);
}
