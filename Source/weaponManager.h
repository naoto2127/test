#pragma once

#include<vector>
#include"weapon.h"
#include<set>

//�e�ۃ}�l�W���[
class WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�e�ۓo�^
	void Register(Weapon* weapon);

	//�e�ۑS�폜
	void Clear();

	//�e�ې��擾
	int GetWeaponCount()const { return static_cast<int>(weapons.size()); }

	//����擾
	Weapon* GetWeapon(int index) { return weapons.at(index); }

	//�e�ۍ폜
	void Remove(Weapon* weapon);

private:
	std::vector<Weapon*> weapons;
	std::set<Weapon*> removes;
};

