//**********************************************************
// �t�@�C����		:Application.h
//**********************************************************

#pragma once
#include "../Temp/stdafx.h"
#include "../Engine/Render/StaticMesh.h"
#include "../Engine/Camera.h"

//�V�F�[�_�[�ɓn���l
struct SHADER_GLOBAL0
{
	XMFLOAT4 vLightDir;	//���C�g����
	XMFLOAT4 vEye;		//�J�����ʒu
};
class CDirect3D
{
	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;
	IDXGISwapChain * m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	ID3D11Texture2D* m_pDepthStencil = NULL;
	ID3D11InputLayout* m_pVertexLayout = NULL;
	ID3D11VertexShader* m_pVertexShader = NULL;		// effect�ł͂Ȃ��A�ʂɃo�[�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* m_pPixelShader = NULL;		// effect�ł͂Ȃ��A�ʂɃs�N�Z���V�F�[�_�[
	ID3D11Buffer* m_pConstantBuffer0 = NULL;		// �A�v�������V�F�[�_�[�˂����@�R���X�^���g�o�b�t�@�[
	ID3D11Buffer* m_pConstantBuffer1 = NULL;		// �A�v�������V�F�[�_�[�˂����@�R���X�^���g�o�b�t�@�[�@
	ID3D11SamplerState* m_pSampleLinear = NULL;		// �e�N�X�`���[�̃T���v���[ �S�}�e���A���e�N�X�`���[����
	XMFLOAT3 m_vLightDir;	// (1, 1,-1);			// ���C�g����
	vector<shared_ptr<CStaticMesh>> v_shMesh;		// FBX mesh

	CCamera camera;
public:

	CDirect3D();
	~CDirect3D();
	HRESULT Initialize(HWND _hWnd);

	void Rendering();
	void destroyD3D();
	void FullScreenMode();


	/*
	CPlayer* player = NULL;
	CPlayerBullet* playerBullet = NULL;
	CEnemy* enemy = NULL;
	CFPSManager* fpsManager = NULL;
	*/

	// �L�������ƂɕK�v�Ȃ̂�������Ȃ��H
	XMVECTORF32 eye;
	XMVECTORF32 at;
	XMVECTORF32 up{ 0.0f, 1.0f, 0.0f };
};

