//**********************************************************
// ファイル名		:Application.h
//**********************************************************

#pragma once
#include "../Temp/stdafx.h"
#include "../Engine/Render/StaticMesh.h"
#include "../Engine/Camera.h"

//シェーダーに渡す値
struct SHADER_GLOBAL0
{
	XMFLOAT4 vLightDir;	//ライト方向
	XMFLOAT4 vEye;		//カメラ位置
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
	ID3D11VertexShader* m_pVertexShader = NULL;		// effectではなく、個別にバーテックスシェーダー
	ID3D11PixelShader* m_pPixelShader = NULL;		// effectではなく、個別にピクセルシェーダー
	ID3D11Buffer* m_pConstantBuffer0 = NULL;		// アプリ←→シェーダー架け橋　コンスタントバッファー
	ID3D11Buffer* m_pConstantBuffer1 = NULL;		// アプリ←→シェーダー架け橋　コンスタントバッファー　
	ID3D11SamplerState* m_pSampleLinear = NULL;		// テクスチャーのサンプラー 全マテリアルテクスチャー共通
	XMFLOAT3 m_vLightDir;	// (1, 1,-1);			// ライト方向
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

	// キャラごとに必要なのかもしれない？
	XMVECTORF32 eye;
	XMVECTORF32 at;
	XMVECTORF32 up{ 0.0f, 1.0f, 0.0f };
};

