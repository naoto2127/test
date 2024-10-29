#pragma once

#include"Graphics/Shader.h"

//�O���錾
class WeaponManager;

//�e��
class Weapon
{
public:
	Weapon(WeaponManager* manager);
	virtual ~Weapon() {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition()const { return position; }

	//�����擾
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }

	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	//�j��
	void Destroy();

	//���a�擾
	float GetRadius()const { return radius; }

	//�����擾
	float GetHeight()const { return height; }

	// ��]�s��擾
	DirectX::XMFLOAT4X4 GetRotationMatrix() const { return rotationMatrix; }



protected:
	//�s��X�V����
	void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	DirectX::XMFLOAT4X4 rotationMatrix = {}; // ��]�s��̏�����
	float angle = 90.0f;
	WeaponManager* manager = nullptr;

	float height = 2.0f;

	float radius = 0.5f;
};
