#include"Character.h"

//�s��X�V����
void Character::UpdateTransform()
{
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);

	//��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);

	//�ʒu�s����쐬
	DirectX::XMMATRIX T=DirectX::XMMatrixTranslation(position.x,position.y,position.z);

	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;

	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

//�ړ�����
void Character::Move(float vx, float vz, float speed)
{

	//�ړ������x�N�g����ݒ�(�ړ������̖���ۑ����Ă���)
	moveVecX = vx;
	moveVecZ = vz;

	//�ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	//�i�s�����x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f)return;

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	//���g�̉�]�l����O���������߂�
	float frontX = sin(angle.y);
	float frontZ = cosf(angle.y);

	//��]�p�����߂邽�߁A2�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * vx) + (frontZ * vz);

	//���ϒl��-1.0~1.0�ŕ\������Ă���A2�̒P�ʃx�N�g���̊p�x��
	//�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rot > speed)rot = speed;

	//�E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z��

	float cross = (frontZ * vx) - (frontX * vz);

	//�QD�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	//���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		//���̏ꍇ�͍���]
		angle.y -= rot;
	}
	else
	{
		//���̏ꍇ�͉E��]
		angle.y += rot;
	}



}

//�W�����v����
void Character::Jump(float speed)
{
	//������̗͂�ݒ�
	velocity.y = speed;
}

//���͏����X�V
void Character::UpdateVelocity(float elapsedTime)
{
	//�o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	//�������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);

	//�������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);


	//�����ړ��X�V����
	UpdateVerticalMove(elapsedTime);
 
	//�����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);

	
}

//�_���[�W��^����
bool Character::ApplyDamage(int damage,float invincibleTime)
{
	//�_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage<=0)return false;

	//���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health<=0)return false;
	
	//���G���Ԓ��̓_���[�W��^���Ȃ�
	if (this->invincibleTimer > 0.0f)return false;

	//���G���Ԑݒ�
	this->invincibleTimer = invincibleTime;

	//�_���[�W����
	health -= damage;

	//���S�ʒm
	if (health<=0)
	{
		OnDead();
	}
	//�_���[�W�ʒm
	else
	{
		OnDamaged();
	}

	//���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;

}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//���͂ɗ͂�������
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;

}

//�������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	//�d�͏����i�d�͂��o�߃t���[��������̒P�ʂȂ̂Ōo�߃t���[��������ŏd�͌v�Z���s���j
	velocity.y += gravity * elapsedFrame;
}

//�����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
	//�ړ�����
	position.y += velocity.y * elapsedTime;

	//�n�ʔ���
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
		velocity.y = 0.0f;
		//���n���Ă��Ȃ�������
		if (!IsGround())
		{
			//���n�������ɌĂяo���֐����Ăяo��
			OnLanding();
		}

		//���n���Ă���t���O��true�ɂ���
		isGround = true;
	}
	else
	{
		isGround = false;
	}
}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ���ʂ̑��͂���������
	float length =sqrtf(velocity.x*velocity.x+velocity.z*velocity.z) ;
	if (length > 0.0f)
	{
		//���C��
		float friction = this->friction * elapsedFrame;

		//�󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!IsGround())
		{
			friction = friction* airControl;
		}

		//���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			//���͂�P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.z / length;


			//�P�ʃx�N�g�����������͂𖀎C�W�����X�P�[�����O�����l�𑬗͂������
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;

		}
		//�������̑��͂𖀎C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	//XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		//�ړ��x�N�g�����[���x�N�g���ׂȂ��Ȃ��������
		float moveVecLength = sqrtf(moveVecX*moveVecX+moveVecZ*moveVecZ);
		if (moveVecLength > 0.0f)
		{
			//������			
			float acceleration = this->acceleration * elapsedFrame;
			
			//�󒆂ɂ���Ƃ��͉����͂����炷
			if (!IsGround())
			{
				acceleration = acceleration* airControl;
			}

			//�ړ��x�N�g���ɂ���������
			velocity.x+= moveVecX* acceleration;
			velocity.z+= moveVecZ* acceleration;


			//�ő呬�x����
			float length = sqrtf(velocity.x*velocity.x+velocity.z*velocity.z);
			if (length > maxMoveSpeed)
			{

				//���x�x�N�g���𐳋K��
				float vx = velocity.x/length;
				float vz = velocity.z/length;
				//�ő呬�����X�P�[�����O�����l�𑬓x�x�N�g���ɑ��
				velocity.x = vx*maxMoveSpeed;
				velocity.z = vz*maxMoveSpeed;

			}
		}
	}
	//�ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;




}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//�ړ�����
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;

}

//���G���ԍX�V
void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
}