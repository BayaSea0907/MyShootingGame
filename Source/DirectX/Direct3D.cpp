#include "Direct3D.h"

CDirect3D::CDirect3D()
{
}
CDirect3D::~CDirect3D()
{
}

//
HRESULT CDirect3D::Initialize(HWND _hWnd)
{
	m_vLightDir = { 0.0f, 1.0f, 0.0f };//ライト方向

	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}
	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);
	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, NULL, &m_pDepthStencilView);
	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//<<<______________________________________________<<<
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"../Resources/hlsl/MShader.hlsl", NULL, NULL, "VS",
		"vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMFLOAT4) * 1,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT4) *
		2, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader ->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))	return FALSE;
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(L"../Resources/hlsl/MShader.hlsl", NULL, NULL, "PS",
		"ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);

	SAFE_RELEASE(pIr);


	//コンスタントバッファー0作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー1作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
	{
		return E_FAIL;
	}

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	//---------------------------------------------------------
	// メッシュの生成
	//---------------------------------------------------------
	for (int i = 0; i <= FBX_NUM ; i++)
	{
		v_shMesh.push_back(create<CStaticMesh>());
		v_shMesh[i]->setDeviceInfo(m_pDevice, m_pDeviceContext, m_pSampleLinear,m_pConstantBuffer1);
	}
	// gameManager->createFBX(v_shMesh);

	return S_OK;

	//>>>______________________________________________>>>
}
VOID CDirect3D::FullScreenMode()
{
	BOOL fullscreen;
	// [F5]キーで画面モードを切り替える
	if (m_pSwapChain != NULL)
	{
		m_pSwapChain->GetFullscreenState(&fullscreen, NULL);
		m_pSwapChain->SetFullscreenState(!fullscreen, NULL);
	}
}

void CDirect3D::destroyD3D()
{
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pSwapChain);
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(FALSE, NULL);

	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//



//シーンを画面にレンダリング
void CDirect3D::Rendering()
{
	float clearColor[4] = { 0,0,0,1 }; // クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f,0);//深度バッファクリア

	camera.UpDate();

	XMMATRIX matView = camera.getMatView();
	XMMATRIX matProj = camera.getMatProj();

	for (int i = 0; i < FBX_NUM; i++)
	{
		v_shMesh[i]->setViewMatrix(&matView);
		//プロジェクション行列
		v_shMesh[i]->setProjectionMatrix(&matProj);
	}

	//ライトベクトルをシェーダーに渡す
	//カメラ位置をシェーダーに渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&pData)))
	{
		SHADER_GLOBAL0 sg0;
		sg0.vLightDir = XMFLOAT4{ m_vLightDir.x, m_vLightDir.y, m_vLightDir.z, 0.0f };
		sg0.vEye = XMFLOAT4{ eye.f[0], eye.f[1],eye.f[2], 0 };
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg0, sizeof(SHADER_GLOBAL0));
		m_pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	m_pSwapChain->Present(0, 0);
}

