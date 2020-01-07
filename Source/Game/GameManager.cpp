#include "../Temp/stdafx.h"
#include "GameManager.h"

CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
}
// **************************************************************************
// VOID createFBX(vector<shared_ptr<CStaticMesh>>)
// → FBXファイルの読み込みおよび、各オブジェクト(プレイヤー、ステージ、オブジェクト)の作成  
// **************************************************************************
//① 読み込むファイルのパス設定
//② プレイヤー、ステージ、オブジェクトの順番に読み込み
// → 1. FBXの情報を取る
// → 2. 各vectorのpushback
// → 3. 各オブジェクトにFBX情報を合成する。
// **************************************************************************
void CGameManager::createFBX(vector<shared_ptr<CStaticMesh>> _v_shMesh)
{
	int i = 0;

	_v_shMesh[i]->createFromFBX("../Resources/FBX/player.FBX");
	player = create<CPlayer>();
	player->SetMesh(_v_shMesh[i]);
	i++;

	// バレット
	for (int j = 0; j < PLAYER_BULLET_NUM; i++, j++)
	{
		_v_shMesh[i]->createFromFBX("../Resources/FBX/playerBullet.FBX");
		playerBullet.push_back(create<CPlayerBullet>());
		playerBullet[j]->SetMesh(_v_shMesh[i]);
	}
	player->SetBullet(playerBullet);

	// ステージ
	for (int j = 0; j < STAGE_NUM; i++, j++)
	{
		_v_shMesh[i]->createFromFBX("../Resources/FBX/Stage01.FBX");
		stage.push_back(create<StageObject>());
		stage[j]->SetMesh(_v_shMesh[i]);
	}
	// エネミー
	pTextLoader = new CTextLoader("../Resources/TextFile/Position.txt");
	
	for (int j = 0; j < ENEMY_NUM; i++, j++)
	{
		_v_shMesh[i]->createFromFBX("../Resources/FBX/PinkBox.FBX"); 
		enemy.push_back(create<CEnemy>());
		enemy[j]->SetMesh(_v_shMesh[i]);
		enemy[j]->SetPosition(pTextLoader->GetPosition(j));
	}
	
	SAFE_DELETE(pTextLoader);
}


// ****************************************************
// 入力処理
// ****************************************************
void CGameManager:: playGame()
{
	auto& input = CInput::GetInstance();

	//入力移動
	if (input.GetKey(KEY_W) || input.GetButtonDirection(PAD_UP))		player->Move(Up);
	if (input.GetKey(KEY_S) || input.GetButtonDirection(PAD_DOWN))		player->Move(Down);
	if (input.GetKey(KEY_D) || input.GetButtonDirection(PAD_RIGHT))		player->Move(Right);
	if (input.GetKey(KEY_A) || input.GetButtonDirection(PAD_LEFT))		player->Move(Left);

	// 攻撃
	if (input.GetKeyDown(KEY_SPACE) || input.GetKeyDown(KEY_RETURN) || input.GetButtonDown(PAD_B))
	{
		if (!player->IsDead())
		{
			player->Shot();
			pDS->Play(1);
		}
	}

	// 復活
	if (input.GetKeyDown(KEY_R) || input.GetButtonDown(PAD_START))	player->Respawn();

	Render();
}
// ****************************************************
// 描画処理
// ****************************************************
void CGameManager::Render()
{
	if (player->HitCheck(enemy))
	{
		pDS->Play(2);
	}
	player->Render();

	for (int i = 0; i < STAGE_NUM; i++)
	{
		stage[i]->Render();
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i]->Render();
	}
}

//**********************************************************
// 解放
//**********************************************************
void CGameManager::Release()
{
	SAFE_DELETE(pDS);
}