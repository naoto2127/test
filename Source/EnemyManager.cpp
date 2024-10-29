#include"EnemyManager.h"
#include"Collision.h"

//�X�V����
void EnemyManager::Update(float elapsedTime)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}


	//�j������
	for (Enemy*enemy : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		//�e�ۂ̔j������
		delete enemy;
	}
	removes.clear();
	


	////�G���m�̏Փˏ���
	CollisionEnemyVsEnemies();
	

}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Enemy* enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (Enemy* enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy)
{
	enemies.emplace_back(enemy);
}

//�G�l�~�[�S�폜
void EnemyManager::Clear()
{
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}

	enemies.clear();
}

void EnemyManager::Remove(Enemy* enemy)
{
	//�j�����X�g�ɒǉ�
	removes.insert(enemy);
}

//�G�l�~�[���m�̏Փˏ���
void EnemyManager::CollisionEnemyVsEnemies()
{
	//�G�̐����擾
	int numEnemies = GetEnemyCount();
	//�G�̐����񂷁i�J�E���^�[i�j
	for (int i = 0; i < numEnemies;++i)
	{
		//�GA���擾
		Enemy* enemyA = GetEnemy(i);
		//�G�̐�����(�J�E���^�[j��i��j�͈Ⴄ���ɂȂ�悤��(�����G���m�͏Փ˂��Ȃ�����)�j
		for (int j = 0; j < numEnemies; ++j)
		{
			if (i == j)// �����G���m�͏Փ˂��Ȃ����߁A�����C���f�b�N�X�̏ꍇ�̓X�L�b�v           
				continue;
			//�GB���擾
			Enemy* enemyB = GetEnemy(j);
			//�����o����̈ʒu
			DirectX::XMFLOAT3 newPositionB;

			//�Փˏ���
			if (Collision::IntersectCylonderVsCylinder(
				//�GA�̈ʒu
				enemyA->GetPosition(),
				//�GA�̔��a
				enemyA->GetRadius(),
				//�GA�̍���
				enemyA->GetHeight(),
				//�GB�̈ʒu
				enemyB->GetPosition(),
				//�GB�̔��a
				enemyB->GetRadius(),
				//�GB�̍���
				enemyB->GetHeight(),
				//�����o����̈ʒu
				newPositionB))
			{
				//�GB�ɉ����o����̈ʒu��ݒ�
				enemyB->setPosition(newPositionB);
			}
		}
	}
}