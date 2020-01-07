//**********************************************************
// ファイル名		:StaticMesh.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"
//シェーダーに渡す値
struct SHADER_GLOBAL1
{
	XMFLOAT4X4 matWorld;				//ワールド行列
	XMFLOAT4X4 matWorldViewPoint;		//ワールドから射影までの変換行列
	XMFLOAT4 vAmbient;					//アンビエント光
	XMFLOAT4 vDiffuse;					//ディフューズ色
	XMFLOAT4 vSpecular;					//鏡面反射
};
//オリジナル　マテリアル構造体
struct MY_MATERIAL
{
	CHAR szName[100];
	XMFLOAT4 vAmbient;					//アンビエント
	XMFLOAT4 vDiffuse;					//ディフューズ
	XMFLOAT4 vSpecular;					//スペキュラー
	CHAR szTextureName[100];			//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD numOfFace;					//そのマテリアルであるポリゴン数
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};
//頂点構造体
struct MY_VERTEX
{
	XMFLOAT4 vPos;
	XMFLOAT4 vNorm;
	XMFLOAT2 vTex;
};
// スタティック・メッシュ
class CStaticMesh
{
public:
	CStaticMesh();
	~CStaticMesh();
	//メソッド
	HRESULT createFromFBX(CHAR* szFileName_);
	void render(XMMATRIX* pMatWorld_);
	//アクセサメソッド
	void setDeviceInfo(ID3D11Device* pDevice_, ID3D11DeviceContext* pDeviceContext_,
		ID3D11SamplerState* pSampleLinear_, ID3D11Buffer* pConstantBuffer_)
	{
		m_pDevice = pDevice_;
		m_pDeviceContext = pDeviceContext_;
		m_pSampleLinear = pSampleLinear_;
		m_pConstantBuffer = pConstantBuffer_;
	};
	void setViewMatrix(XMMATRIX* pView_) {
		XMStoreFloat4x4(&m_matView,
			*pView_);
	};
	void setProjectionMatrix(XMMATRIX* pProj_) {
		XMStoreFloat4x4(&m_matProj,
			*pProj_);
	};
private:
	//メソッド
	HRESULT initFBX(CHAR* _szFileName);
	void destroyFBX();
	HRESULT createIndexBuffer(DWORD size_, int* pIndex_, ID3D11Buffer** ppIndexBuffer_);
	//FBX
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	//外部のデバイス等情報
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11Buffer* m_pConstantBuffer;
	XMFLOAT4X4 m_matView;
	XMFLOAT4X4 m_matProj;
	//メッシュ関連
	DWORD m_NumOfVert;
	DWORD m_NumOfFace;
	DWORD m_NumOfUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	MY_MATERIAL* m_pMaterial;
	DWORD m_NumOfMaterial;
	//アニメーション関連
	XMFLOAT4X4 m_matParentOrientation; //親の姿勢行列
	XMFLOAT4X4 m_matMyOrientation; //自分の姿勢行列（親から見た相対姿勢）
	XMFLOAT4X4 m_matFinalWorld; //最終的なワールド行列（この姿勢でレンダリングする）
	FbxNode* m_pFbxNode; //FBXから姿勢行列を取り出す際に使うFBXポインター
};

