#pragma once

#include"Graphics/Shader.h"



//���C
class Ry
{
public:
	Ry();
	~Ry();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

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

	

protected:
	//�s��X�V����
	void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	
	


	float height = 2.0f;

	float radius = 0.5f;
};
