//**********************************************************
// ファイル名		:TextLoader.h
//**********************************************************
// 作成日			:2017.4.26
// 更新内容			:テキストファイルをロードして、Positionを取得する
//**********************************************************
#pragma once
#include "../../Temp/stdafx.h"
#include "../ObjectInfo.h"

class CTextLoader
{
	vector<Pos>	  m_pos;
	vector<Angle> m_angle;
	vector<Scale> m_scale;
	string	m_fileName;	
public:
	CTextLoader();
	CTextLoader(string fileName_);
	~CTextLoader();

	VOID SetFileName(string fileName_);
	VOID LoadPosition();

	Pos GetPosition(UINT index_)const;
};

