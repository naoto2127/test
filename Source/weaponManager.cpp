#include"weaponManager.h"

//�R���X�g���N�^
WeaponManager::WeaponManager()
{

}

//�f�X�g���N�^
WeaponManager::~WeaponManager()
{
	Clear();
}

//�X�V����
void WeaponManager::Update(float elapsedTime)
{
	//�X�V����
	for (Weapon* weapon : weapons)
	{
		weapon->Update(elapsedTime);
	}

	//�j������
	//��projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����
	for (Weapon* weapon : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Weapon*>::iterator it = std::find(weapons.begin(), weapons.end(), weapon);
		if (it != weapons.end())
		{
			weapons.erase(it);
		}

		//�e�ۂ̔j������
		delete weapon;
	}
	//�j�����X�g���N���A
	removes.clear();

}

//�`�揈��
void WeaponManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Weapon* weapon : weapons)
	{
		weapon->Render(context, shader);
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void WeaponManager::DrawDebugPrimitive()
{
	for (Weapon* weapon : weapons)
	{
		weapon->DrawDebugPrimitive();
	}
}

//�e�ۓo�^
void WeaponManager::Register(Weapon* weapon)
{
	weapons.emplace_back(weapon);
}

//�e�ۑS�폜
void WeaponManager::Clear()
{
	for (Weapon* weapon : weapons)
	{
		delete weapon;
	}

	weapons.clear();
}

//�e�ۍ폜
void WeaponManager::Remove(Weapon* weapon)
{
	//�j�����X�g�ɒǉ�
	removes.insert(weapon);
}