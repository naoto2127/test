#include"weapon.h"
#include"weaponManager.h"
#include"Graphics/Graphics.h"



inline float normalizeAngle(float radian)
{

	while (radian > DirectX::XM_PI)
	{
		radian -= DirectX::XM_PI * 2.0f;
	}
	while (radian < -DirectX::XM_PI)
	{
		radian += DirectX::XM_PI * 2.0f;
	}
	return radian;
}

//�R���X�g���N�^
Weapon::Weapon(WeaponManager* manager) :manager(manager)
{
	manager->Register(this);
}

//�f�o�b�O�v���~�e�B�u�`��
void Weapon::DrawDebugPrimitive()
{

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//�Փ˔���p�̃f�o�b�O����`��
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&direction)));
	debugRenderer->DrawSphere(pos, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
}

//�s��X�V����
void Weapon::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// �O�����x�N�g�����v�Z�i���łɎ����Ă���O�����x�N�g����XMVECTOR�ɕϊ�����j
	Front = DirectX::XMLoadFloat3(&direction);

	// ���K������
	Front = DirectX::XMVector3Normalize(Front);

	// ���̏�x�N�g�����Z�o(Y�ɂ����l�̓����Ă���XMVECTOR���쐬����)
	Up = DirectX::XMVectorSet(0.001f, 1.0f, 0.0f, 0.0f);

	// ���K������
	Up = DirectX::XMVector3Normalize(Up);

	// �E�x�N�g�����Z�o(�O�̃x�N�g���Ɖ��̏�x�N�g���̊O��)
	Right = DirectX::XMVector3Cross(Up, Front);

	// ���K������
	Right = DirectX::XMVector3Normalize(Right);

	// ��x�N�g�����v�Z(�O�x�N�g���ƉE�x�N�g���̊O�ς��琳������x�N�g�����v�Z)
	Up = DirectX::XMVector3Cross(Front, Right);

	// ���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// X�����̉�]�s����쐬
	float angleX = DirectX::XM_PIDIV2; // 90�x�����W�A���ŕ\�������萔
	DirectX::XMMATRIX XRotation = DirectX::XMMatrixRotationX(angle);
	DirectX::XMMATRIX X2Rotation = DirectX::XMMatrixRotationX(normalizeAngle(90));
	DirectX::XMMATRIX ZRotation = DirectX::XMMatrixRotationZ(normalizeAngle(90));

	// �Z�o�������x�N�g������s����쐬
	DirectX::XMMATRIX transform2;

	transform2.r[0] = DirectX::XMVectorSet(right.x * scale.x, right.y * scale.x, right.z * scale.x, 0.0f);
	transform2.r[1] = DirectX::XMVectorSet(up.x * scale.y, up.y * scale.y, up.z * scale.y, 0.0f);
	transform2.r[2] = DirectX::XMVectorSet(front.x * scale.z, front.y * scale.z, front.z * scale.z, 0.0f);
	transform2.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

	// X�����̉�]��K�p
	transform2 =ZRotation *XRotation * X2Rotation * transform2;

	// ���˕�����ݒ肷��ifront�x�N�g���j
	this->direction = front;

	DirectX::XMStoreFloat4x4(&transform, transform2);

	// ��]�s����X�V����
	DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixTranspose(XRotation ));

}


//�j��
void Weapon::Destroy()
{
	manager->Remove(this);
}