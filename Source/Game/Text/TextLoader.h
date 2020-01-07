//**********************************************************
// �t�@�C����		:TextLoader.h
//**********************************************************
// �쐬��			:2017.4.26
// �X�V���e			:�e�L�X�g�t�@�C�������[�h���āAPosition���擾����
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

