//**********************************************************
// �t�@�C����		:StaticMesh.h
//**********************************************************

#pragma once
#include "../../Temp/stdafx.h"
//�V�F�[�_�[�ɓn���l
struct SHADER_GLOBAL1
{
	XMFLOAT4X4 matWorld;				//���[���h�s��
	XMFLOAT4X4 matWorldViewPoint;		//���[���h����ˉe�܂ł̕ϊ��s��
	XMFLOAT4 vAmbient;					//�A���r�G���g��
	XMFLOAT4 vDiffuse;					//�f�B�t���[�Y�F
	XMFLOAT4 vSpecular;					//���ʔ���
};
//�I���W�i���@�}�e���A���\����
struct MY_MATERIAL
{
	CHAR szName[100];
	XMFLOAT4 vAmbient;					//�A���r�G���g
	XMFLOAT4 vDiffuse;					//�f�B�t���[�Y
	XMFLOAT4 vSpecular;					//�X�y�L�����[
	CHAR szTextureName[100];			//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD numOfFace;					//���̃}�e���A���ł���|���S����
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};
//���_�\����
struct MY_VERTEX
{
	XMFLOAT4 vPos;
	XMFLOAT4 vNorm;
	XMFLOAT2 vTex;
};
// �X�^�e�B�b�N�E���b�V��
class CStaticMesh
{
public:
	CStaticMesh();
	~CStaticMesh();
	//���\�b�h
	HRESULT createFromFBX(CHAR* szFileName_);
	void render(XMMATRIX* pMatWorld_);
	//�A�N�Z�T���\�b�h
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
	//���\�b�h
	HRESULT initFBX(CHAR* _szFileName);
	void destroyFBX();
	HRESULT createIndexBuffer(DWORD size_, int* pIndex_, ID3D11Buffer** ppIndexBuffer_);
	//FBX
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	//�O���̃f�o�C�X�����
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11Buffer* m_pConstantBuffer;
	XMFLOAT4X4 m_matView;
	XMFLOAT4X4 m_matProj;
	//���b�V���֘A
	DWORD m_NumOfVert;
	DWORD m_NumOfFace;
	DWORD m_NumOfUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	MY_MATERIAL* m_pMaterial;
	DWORD m_NumOfMaterial;
	//�A�j���[�V�����֘A
	XMFLOAT4X4 m_matParentOrientation; //�e�̎p���s��
	XMFLOAT4X4 m_matMyOrientation; //�����̎p���s��i�e���猩�����Ύp���j
	XMFLOAT4X4 m_matFinalWorld; //�ŏI�I�ȃ��[���h�s��i���̎p���Ń����_�����O����j
	FbxNode* m_pFbxNode; //FBX����p���s������o���ۂɎg��FBX�|�C���^�[
};

