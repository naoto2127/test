#include"weaponSword.h"

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
WeaponSword::WeaponSword(WeaponManager* manager)
	:Weapon(manager)
{

	model = new Model("Data/Model/Sword/Sword.mdl");

	//�\���T�C�Y�𒲐�
	scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
WeaponSword::~WeaponSword()
{
	delete model;
}

//�X�V����
void WeaponSword::Update(float elapsedTime)
{
	//��������
	LifeTimer -= elapsedTime;
	if (LifeTimer <= 0)
	{
		//�������폜
		Destroy();
	}

	//�ړ�
	float speed = this->speed * elapsedTime;



	////�ʒu�{���O����*����
	//position.x += direction.x * speed;
	//position.y += direction.y * speed;
	//position.z += direction.z * speed;


	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();


	//���f���s��X�V����
	model->UpdateTransform(transform);

	
		
	angle += 0.08745;
	
	angle = normalizeAngle(angle);

	
}

//�`�揈��
void WeaponSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

//����
void WeaponSword::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
