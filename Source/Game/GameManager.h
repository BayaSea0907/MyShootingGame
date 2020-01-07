//**********************************************************
// ファイル名		:GameManager.h
//**********************************************************

#pragma once
#include "../DirectX/DirectInput.h"
#include "../DirectX/DirectSound.h"
#include "../Engine/Camera.h"
#include "Text/TextLoader.h"
#include "GameObject/Enemy.h"
#include "GameObject/Player.h"
#include "GameObject/PlayerBullet.h"
#include "GameObject/StageObject.h"

class CGameManager
{

public:
	CGameManager();
	~CGameManager();
	void createFBX(vector<shared_ptr<CStaticMesh>>);

	void Title(){}
	void Opening(){}
	void ClearGame(){}
	void EndGame(){}

	void Initialize(){}
	void Update(){}
	void playGame();
	void Render();
	void Release();

	CDirectSound* pDS = NULL;		// ダイレクトサウンド
	CTextLoader* pTextLoader = NULL;

	shared_ptr<CPlayer> player;
	vector<shared_ptr<CPlayerBullet>> playerBullet;
	vector<shared_ptr<StageObject>> stage;
	vector<shared_ptr<CEnemy>> enemy;
	INT ObjectCount = 0;

};

