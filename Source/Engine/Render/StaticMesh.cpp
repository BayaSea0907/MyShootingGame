#include "../../Temp/stdafx.h"
#include "StaticMesh.h"
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"wininet.lib")
//
//
//
CStaticMesh::CStaticMesh()
{
	ZeroMemory(this, sizeof(CStaticMesh));
	XMStoreFloat4x4(&m_matMyOrientation, XMMatrixIdentity());
}
CStaticMesh::~CStaticMesh()
{
	delete[] m_pMaterial;
	SAFE_RELEASE(m_pVertexBuffer);
	for (size_t i = 0; i < m_NumOfMaterial; i++)
	{
		SAFE_RELEASE(m_ppIndexBuffer[i]);
	}
	delete[] m_ppIndexBuffer;
	destroyFBX();
}
//
//
//
HRESULT CStaticMesh::initFBX(CHAR* szFileName_)
{
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "my importer");
	int iFormat = -1;
	m_pFbxImporter->Initialize((const char*)szFileName_, iFormat);
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "my scene");
	m_pFbxImporter->Import(m_pFbxScene, false/*0*/);
	return S_OK;
}
//
//
//
void CStaticMesh::destroyFBX()
{
	//IOSREF.FreeIOSettings();
	if (m_pFbxManager) m_pFbxManager->Destroy();
}
//___<<<UVbase Improve________________________________________________________________
//
//
//素材（FBX)のほうは、三角ポリゴンにすること
HRESULT CStaticMesh::createFromFBX(CHAR* szFileName_)
{
	//指定のファイルでFBX初期化
	if (FAILED(initFBX(szFileName_)))
	{
		MessageBox(0, L"FBXローダー初期化失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//読み込み
	FbxNode* pRootNode = m_pFbxScene->GetRootNode();
	FbxMesh* pFbxMesh = 0;
	FbxNodeAttribute::EType type;
	FbxNodeAttribute *pAttr = pRootNode->GetNodeAttribute();
	if (pAttr != NULL)
	{
		type = pAttr->GetAttributeType();
		//タイプがメッシュの場合　普通はルートがメッシュそのものであることは少ない
		if (type == FbxNodeAttribute::eMesh)
		{
			pFbxMesh = pRootNode->GetMesh();
		}
	}
	else//子がメッシュの可能性あり
	{
		int iNumChild = pRootNode->GetChildCount();
		for (int i = 0; i<iNumChild; i++)
		{
			FbxNode* pChildNode = pRootNode->GetChild(i);
			if (!pChildNode) continue;
			pAttr = pChildNode->GetNodeAttribute();
			type = pAttr->GetAttributeType();
			if (type == FbxNodeAttribute::eMesh)//タイプがメッシュの場合
			{
				pFbxMesh = pChildNode->GetMesh();
				break;
			}
			else
			{
				MessageBox(0, L"メッシュの読み込み失敗", NULL, MB_OK);
			}
		}
	}
	//事前に頂点数、ポリゴン数等を調べる
	m_NumOfVert = pFbxMesh->GetControlPointsCount();
	m_NumOfUV = pFbxMesh->GetTextureUVCount();
	m_NumOfFace = pFbxMesh->GetPolygonCount();
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_VERTEX* pvVB = NULL;
	m_NumOfVert<m_NumOfUV ? pvVB = new MY_VERTEX[m_NumOfUV] : pvVB = new
		MY_VERTEX[m_NumOfVert];
	////ポリゴンごとに　頂点読み込み 、法線読み込み、UV読み込み
	FbxVector4 Normal;
	for (size_t i = 0; i<m_NumOfFace; i++)
	{
		int iIndex0 = 0;
		int iIndex1 = 0;
		int iIndex2 = 0;
		int iStartIndex = pFbxMesh->GetPolygonVertexIndex(i);
		int* piIndex = pFbxMesh->GetPolygonVertices();//（頂点インデックス）読み込み
		iIndex0 = piIndex[iStartIndex];
		iIndex1 = piIndex[iStartIndex + 1];
		iIndex2 = piIndex[iStartIndex + 2];
		//UV数のほうが多い場合は本来の（頂点ベース）インデックスを利用しない。UVインデックスを基準にする
		if (m_NumOfVert<m_NumOfUV)
		{
			iIndex0 = pFbxMesh->GetTextureUVIndex(i, 0,
				FbxLayerElement::eTextureDiffuse);
			iIndex1 = pFbxMesh->GetTextureUVIndex(i, 1,
				FbxLayerElement::eTextureDiffuse);
			iIndex2 = pFbxMesh->GetTextureUVIndex(i, 2,
				FbxLayerElement::eTextureDiffuse);
		}
		//頂点
		FbxVector4* pCoord = pFbxMesh->GetControlPoints();
		int index = pFbxMesh->GetPolygonVertex(i, 0);
		pvVB[iIndex0].vPos.x = (FLOAT)pCoord[index][0];
		pvVB[iIndex0].vPos.z = (FLOAT)pCoord[index][1];
		pvVB[iIndex0].vPos.y = (FLOAT)pCoord[index][2];
		index = pFbxMesh->GetPolygonVertex(i, 1);
		pvVB[iIndex1].vPos.x = (FLOAT)pCoord[index][0];
		pvVB[iIndex1].vPos.z = (FLOAT)pCoord[index][1];
		pvVB[iIndex1].vPos.y = (FLOAT)pCoord[index][2];
		index = pFbxMesh->GetPolygonVertex(i, 2);
		pvVB[iIndex2].vPos.x = (FLOAT)pCoord[index][0];
		pvVB[iIndex2].vPos.z = (FLOAT)pCoord[index][1];
		pvVB[iIndex2].vPos.y = (FLOAT)pCoord[index][2];
		//法線
		pFbxMesh->GetPolygonVertexNormal(i, 0, Normal);
		pvVB[iIndex0].vNorm.x = (FLOAT)Normal[0];
		pvVB[iIndex0].vNorm.z = (FLOAT)Normal[1];
		pvVB[iIndex0].vNorm.y = (FLOAT)Normal[2];
		pFbxMesh->GetPolygonVertexNormal(i, 1, Normal);
		pvVB[iIndex1].vNorm.x = (FLOAT)Normal[0];
		pvVB[iIndex1].vNorm.z = (FLOAT)Normal[1];
		pvVB[iIndex1].vNorm.y = (FLOAT)Normal[2];
		pFbxMesh->GetPolygonVertexNormal(i, 2, Normal);
		pvVB[iIndex2].vNorm.x = (FLOAT)Normal[0];
		pvVB[iIndex2].vNorm.z = (FLOAT)Normal[1];
		pvVB[iIndex2].vNorm.y = (FLOAT)Normal[2];
		//テクスチャー座標
		int UVindex = pFbxMesh->GetTextureUVIndex(i, 0, FbxLayerElement::eTextureDiffuse);
		FbxLayerElementUV* pUV = 0;
		FbxLayerElementUV* uv = pFbxMesh->GetLayer(0)->GetUVs();
		if (m_NumOfUV && uv->GetMappingMode() == FbxLayerElement::eByPolygonVertex)
		{
			//この取得の仕方は、UVマッピングモードがeByPolygonVertexの時じゃないとできない
			UVindex = pFbxMesh->GetTextureUVIndex(i, 0,
				FbxLayerElement::eTextureDiffuse);
			pUV = pFbxMesh->GetLayer(0)->GetUVs();
			FbxVector2 v2 = pUV->GetDirectArray().GetAt(UVindex);
			pvVB[iIndex0].vTex.x = (FLOAT)v2[0];
			pvVB[iIndex0].vTex.y = 1.0f - (FLOAT)v2[1];
			UVindex = pFbxMesh->GetTextureUVIndex(i, 1,
				FbxLayerElement::eTextureDiffuse);
			v2 = pUV->GetDirectArray().GetAt(UVindex);
			pvVB[iIndex1].vTex.x = (FLOAT)v2[0];
			pvVB[iIndex1].vTex.y = 1.0f - (FLOAT)v2[1];
			UVindex = pFbxMesh->GetTextureUVIndex(i, 2,
				FbxLayerElement::eTextureDiffuse);
			v2 = pUV->GetDirectArray().GetAt(UVindex);
			pvVB[iIndex2].vTex.x = (FLOAT)v2[0];
			pvVB[iIndex2].vTex.y = 1.0f - (FLOAT)v2[1];
		}
	}
	FbxLayerElementUV* uv = pFbxMesh->GetLayer(0)->GetUVs();
	if (m_NumOfUV && uv->GetMappingMode() == FbxLayerElement::eByControlPoint)
	{
		FbxLayerElementUV* pUV = pFbxMesh->GetLayer(0)->GetUVs();
		for (size_t k = 0; k<m_NumOfUV; k++)
		{
			FbxVector2 v2;
			v2 = pUV->GetDirectArray().GetAt(k);
			pvVB[k].vTex.x = (FLOAT)v2[0];
			pvVB[k].vTex.y = 1.0f - (FLOAT)v2[1];
		}
	}
	//マテリアル読み込み
	FbxNode* pNode = pFbxMesh->GetNode();
	m_NumOfMaterial = pNode->GetMaterialCount();
	m_pMaterial = new MY_MATERIAL[m_NumOfMaterial];
	//マテリアルの数だけインデックスバッファーを作成
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumOfMaterial];
	for (size_t i = 0; i<m_NumOfMaterial; i++)
	{
		//フォンモデルを想定
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);
		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;
		//環境光
		m_pMaterial[i].vAmbient.x = (float)pPhong->Ambient.Get()[0];
		m_pMaterial[i].vAmbient.z = (float)pPhong->Ambient.Get()[1];
		m_pMaterial[i].vAmbient.y = (float)pPhong->Ambient.Get()[2];
		//拡散反射光
		m_pMaterial[i].vDiffuse.x = (float)pPhong->Diffuse.Get()[0];
		m_pMaterial[i].vDiffuse.z = (float)pPhong->Diffuse.Get()[1];
		m_pMaterial[i].vDiffuse.y = (float)pPhong->Diffuse.Get()[2];
		//鏡面反射光
		m_pMaterial[i].vSpecular.x = (float)pPhong->Specular.Get()[0];
		m_pMaterial[i].vSpecular.z = (float)pPhong->Specular.Get()[1];
		m_pMaterial[i].vSpecular.y = (float)pPhong->Specular.Get()[2];
		//テクスチャー（ディフューズテクスチャーのみ）
		FbxProperty fbxProperty;
		fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
		FbxTexture* texture = FbxCast<FbxTexture>(fbxProperty.GetSrcObject(0));
		// テクスチャ名取得用
		FbxFileTexture* fileTexture = fbxProperty.GetSrcObject<FbxFileTexture>();
		if (texture)
		{
			// テクスチャ取得
			strcpy_s(m_pMaterial[i].szTextureName, fileTexture->GetRelativeFileName());
			//strcpy_s(m_pMaterial[i].szTextureName, texture->GetName());
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice,
				m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))//絶対パスファイル名は、まず失敗すると思うが、、、
			{
				char ext[11];
				_splitpath_s(
					m_pMaterial[i].szTextureName,
					NULL,
					NULL,
					NULL,
					NULL,
					m_pMaterial[i].szTextureName,
					sizeof(m_pMaterial[i].szTextureName),
					ext,
					sizeof(ext));
				strcat_s(m_pMaterial[i].szTextureName, ext);
				if (FAILED(D3DX11CreateShaderResourceViewFromFileA(m_pDevice,
					m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))//ファイル名のみでトライ
				{
					// bMessageBox(0, L"テクスチャー読み込み失敗", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
		//マテリアルの数だけインデックスバッファーを作成
		//とりあえず、メッシュ内のポリゴン数でメモリ確保（個々のポリゴングループはかならずこれ以下になるが）
		int iCount = 0;
		int* pIndex = new int[m_NumOfFace * 3];
		//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる
		iCount = 0;
		for (size_t k = 0; k<m_NumOfFace; k++)
		{
			FbxLayerElementMaterial* mat = pFbxMesh->GetLayer(0)->GetMaterials();//レイヤーが1枚だけを想定
			int matId = mat->GetIndexArray().GetAt(k);
			if (matId == i)
			{
				if (m_NumOfVert<m_NumOfUV)
				{
					pIndex[iCount] = pFbxMesh->GetTextureUVIndex(k, 0,
						FbxLayerElement::eTextureDiffuse);
					pIndex[iCount + 1] = pFbxMesh->GetTextureUVIndex(k, 1,
						FbxLayerElement::eTextureDiffuse);
					pIndex[iCount + 2] = pFbxMesh->GetTextureUVIndex(k, 2,
						FbxLayerElement::eTextureDiffuse);
				}
				else
				{
					pIndex[iCount] = pFbxMesh->GetPolygonVertex(k, 0);
					pIndex[iCount + 1] = pFbxMesh->GetPolygonVertex(k, 1);
					pIndex[iCount + 2] = pFbxMesh->GetPolygonVertex(k, 2);
				}
				iCount += 3;
			}
		}
		createIndexBuffer(iCount * sizeof(int), pIndex, &m_ppIndexBuffer[i]);
		m_pMaterial[i].numOfFace = iCount / 3;//そのマテリアル内のポリゴン数
		delete[] pIndex;
	}
	//バーテックスバッファーを作成
	if (m_NumOfVert<m_NumOfUV) m_NumOfVert = m_NumOfUV;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_VERTEX) *m_NumOfVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return FALSE;
	//一時的な入れ物は、もはや不要
	delete[] pvVB;
	return S_OK;
}
//
//
//
HRESULT CStaticMesh::createIndexBuffer(DWORD size_, int* pIndex_, ID3D11Buffer**
	ppIndexBuffer_)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex_;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, ppIndexBuffer_)))
	{
		return FALSE;
	}
	return S_OK;
}
//
//
//
void CStaticMesh::render(XMMATRIX* pMatWorld_)
{
	XMStoreFloat4x4(&m_matFinalWorld, *pMatWorld_);
	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (size_t i = 0; i<m_NumOfMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (m_pMaterial[i].numOfFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//プリミティブ・トポロジーをセット
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//マテリアルの各要素と変換行列をシェーダーに渡す
		//ワールド行列をシェーダーに渡す
		//ワールド・ビュー・プロジェクション行列をシェーダーに渡す
		//ディフューズカラーをシェーダーに渡す
		//スペキュラーをシェーダーに渡す
		//スペキュラーのパワーをシェーダーに渡す
		//アンビエントをシェーダーに渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &pData)))
		{
			SHADER_GLOBAL1 sg1;
			sg1.matWorld = m_matFinalWorld;

			XMStoreFloat4x4(&sg1.matWorld, XMMatrixTranspose(XMLoadFloat4x4(&sg1.matWorld)));
			XMStoreFloat4x4(&sg1.matWorldViewPoint,
				XMMatrixTranspose(XMLoadFloat4x4(&m_matFinalWorld) *
					XMLoadFloat4x4(&m_matView) *
					XMLoadFloat4x4(&m_matProj)));
			sg1.vAmbient = m_pMaterial[i].vAmbient;
			sg1.vDiffuse = m_pMaterial[i].vDiffuse;
			sg1.vSpecular = m_pMaterial[i].vSpecular;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg1, sizeof(SHADER_GLOBAL1));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
		m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer);
		//テクスチャーをシェーダーに渡す
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pDeviceContext->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		//Draw
		m_pDeviceContext->DrawIndexed(m_pMaterial[i].numOfFace * 3, 0, 0);
	}
}
//___UVbase Improve>>>________________________________________________________________