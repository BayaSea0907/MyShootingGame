#include "TextLoader.h"

CTextLoader::CTextLoader()
{
}

CTextLoader::CTextLoader(string fileName_)
{
	m_fileName = fileName_;
	LoadPosition();
}


CTextLoader::~CTextLoader()
{
}

VOID CTextLoader::SetFileName(string fileName_)
{
	m_fileName = fileName_;
}


VOID CTextLoader::LoadPosition()
{
	Pos pos;
	string road;

	// �t�@���ǂݍ���
	ifstream ifs(m_fileName);

	if (ifs.fail())
	{
		MessageBox(0, L"�e�L�X�g�t�@�C���ǂݍ��ݎ��s", NULL, MB_OK);
	}
	else
	{
		while (getline(ifs, road))
		{
			string tmp;
			istringstream stream(road);
			stream >> pos.X;

			if (getline(stream, tmp, ','))
			{
				stream >> pos.Y;
				if (getline(stream, tmp, ','))
				{
					stream >> pos.Z;
				}
			}
			m_pos.push_back(pos);
		}

		ifs.close();
		cout << "�t�@�C���ǂݍ��ݏI��" << endl;
	}
}

Pos CTextLoader::GetPosition(UINT index_) const
{
	return m_pos[index_];
}