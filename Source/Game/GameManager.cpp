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
// �� FBX�t�@�C���̓ǂݍ��݂���сA�e�I�u�W�F�N�g(�v���C���[�A�X�e�[�W�A�I�u�W�F�N�g)�̍쐬  
// **************************************************************************
//�@ �ǂݍ��ރt�@�C���̃p�X�ݒ�
//�A �v���C���[�A�X�e�[�W�A�I�u�W�F�N�g�̏��Ԃɓǂݍ���
// �� 1. FBX�̏������
// �� 2. �evector��pushback
// �� 3. �e�I�u�W�F�N�g��FBX������������B
// **************************************************************************
void CGameManager::createFBX(vector<shared_ptr<CStaticMesh>> _v_shMesh)
{
	int i = 0;

	_v_shMesh[i]->createFromFBX("../Resources/FBX/player.FBX");
	player = create<CPlayer>();
	player->SetMesh(_v_shMesh[i]);
	i++;

	// �o���b�g
	for (int j = 0; j < PLAYER_BULLET_NUM; i++, j++)
	{
		_v_shMesh[i]->createFromFBX("../Resources/FBX/playerBullet.FBX");
		playerBullet.push_back(create<CPlayerBullet>());
		playerBullet[j]->SetMesh(_v_shMesh[i]);
	}
	player->SetBullet(playerBullet);

	// �X�e�[�W
	for (int j = 0; j < STAGE_NUM; i++, j++)
	{
		_v_shMesh[i]->createFromFBX("../Resources/FBX/Stage01.FBX");
		stage.push_back(create<StageObject>());
		stage[j]->SetMesh(_v_shMesh[i]);
	}
	// �G�l�~�[
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
// ���͏���
// ****************************************************
void CGameManager:: playGame()
{
	auto& input = CInput::GetInstance();

	//���͈ړ�
	if (input.GetKey(KEY_W) || input.GetButtonDirection(PAD_UP))		player->Move(Up);
	if (input.GetKey(KEY_S) || input.GetButtonDirection(PAD_DOWN))		player->Move(Down);
	if (input.GetKey(KEY_D) || input.GetButtonDirection(PAD_RIGHT))		player->Move(Right);
	if (input.GetKey(KEY_A) || input.GetButtonDirection(PAD_LEFT))		player->Move(Left);

	// �U��
	if (input.GetKeyDown(KEY_SPACE) || input.GetKeyDown(KEY_RETURN) || input.GetButtonDown(PAD_B))
	{
		if (!player->IsDead())
		{
			player->Shot();
			pDS->Play(1);
		}
	}

	// ����
	if (input.GetKeyDown(KEY_R) || input.GetButtonDown(PAD_START))	player->Respawn();

	Render();
}
// ****************************************************
// �`�揈��
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
// ���
//**********************************************************
void CGameManager::Release()
{
	SAFE_DELETE(pDS);
}