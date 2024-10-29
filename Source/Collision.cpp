#include"Collision.h"

//���Ƌ��̌�������
bool Collision::IntersectSphereVsSphre(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A��B�̒P�ʃx�N�g�����Z�o
	//XMFLOAT��XMVECTOR�ɕϊ�
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	//Vec=B-A
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	//XMVector3LengthSq�֐��Œ������v�Z�@sqrt���Ă��Ȃ�LengthSq�̂ق�������
	DirectX::XMVECTOR LengthSq= DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//��������(���aA+���aB)
	float range = radiusA + radiusB;
	//sqrt���Ȃ����������a��2�悷��
	if (lengthSq>range*range)
	{
		return false;
	}

	//A��B�������o��
	
	//�^�[�Q�b�g�����ւ̃x�N�g��(Vec)�𐳋K��
	Vec = DirectX::XMVector3Normalize(Vec);

	//���K�������x�N�g����range�X�P�[��(XMVectorScale)
	Vec = DirectX::XMVectorScale(Vec, range);

	//�X�P�[�������x�N�g����PositionA�̈ʒu���瑫��PositionB�͈Ӗ����Ȃ����ł��悢
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);

	//�������l��XMFLOAT3(outPositionB)�ɕϊ�
	DirectX::XMStoreFloat3(&outPositionB,PositionB );
	return true;
}

//�~���Ɖ~���̌�������
bool Collision::IntersectCylonderVsCylinder(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	float heightA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	float heightB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A�̑�����B�̓�����Ȃ瓖�����Ă��Ȃ�
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//A�̓���B�̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (positionA.y + heightA < positionB.y)
	{
		return false;

	}
	//XZ���ʂł͈̔̓`�F�b�N
	//X���m������
	float vx = positionB.x - positionA.x;

	//Z���m������
	float vz = positionB.z - positionA.z;

	//XZ�̒������v�Z����
	float distXZ = sqrtf(vx*vx+vz*vz);

	//���aA�Ɣ��aB�̒������v�Z����
	float range = radiusA+radiusB;

	//XZ�̒��������aA�Ɣ��aB�̒������傫�������瓖�����ĂȂ�
	if (distXZ>range)
	{
		return false;
	}
	//A��B�������o��
	vx /= distXZ;
	vz /= distXZ;

	outPositionB.x = vx * range + positionA.x;
	outPositionB.y = positionB.y;
	outPositionB.z = vz * range + positionA.z;

	return true;

}

bool Collision::IntersectSphereVsCylinder(
	const DirectX::XMFLOAT3& spherePosition,
	float sphereRadius, 
	const DirectX::XMFLOAT3& cylinderPosition,
	float cylinderRadius, 
	float cylinderHeight,
	DirectX::XMFLOAT3& outCylinderPosition)
{
	//���̑������~���̓�����Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y-sphereRadius > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//���̓����~���̑�����艺�Ȃ瓖�����Ă��Ȃ�
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ���ʂł͈̔̓`�F�b�N
	//X���m������
	float vx = cylinderPosition.x - spherePosition.x;

	//Z���m������
	float vz = cylinderPosition.z - spherePosition.z;

	//XZ�̒������v�Z����
	float distXZ = sqrtf(vx * vx + vz * vz);

	//���aA�Ɣ��aB�̒������v�Z����
	float range = sphereRadius + cylinderRadius;

	//XZ�̒��������aA�Ɣ��aB�̒������傫�������瓖�����ĂȂ�
	if (distXZ > range)
	{
		return false;
	}
	//�����~���������o��
	//vx��vz�𐳋K��
	vx /= distXZ;
	vz /= distXZ;

	outCylinderPosition.x = (vx * range) + spherePosition.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = vz * range + spherePosition.z;

	return true;

}

//���Ɖ~���Q
bool Collision::SphereCylinderIntersection(
	const DirectX::XMFLOAT3& sphereCenter, float sphereRadius,
	const DirectX::XMFLOAT3& cylinderBaseCenter, float cylinderRadius, float cylinderHeight,
	const DirectX::XMMATRIX& transform)
{
	// �t�s����v�Z���Č��̍��W�n�ɕϊ�
	DirectX::XMMATRIX inverseTransform = DirectX::XMMatrixInverse(nullptr, transform);

	// ���̒��S�ʒu�����̍��W�n�ɕϊ�
	DirectX::XMVECTOR sphereCenterVec = DirectX::XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1.0f);
	sphereCenterVec = DirectX::XMVector3TransformCoord(sphereCenterVec, inverseTransform);

	// �~���̒�ʒ��S�ʒu�����̍��W�n�ɕϊ�
	DirectX::XMVECTOR cylinderBaseCenterVec = DirectX::XMVectorSet(cylinderBaseCenter.x, cylinderBaseCenter.y, cylinderBaseCenter.z, 1.0f);
	cylinderBaseCenterVec = DirectX::XMVector3TransformCoord(cylinderBaseCenterVec, inverseTransform);

	// �~���̎������iy�������j�����̍��W�n�ɕϊ�
	DirectX::XMVECTOR cylinderAxisVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // y������
	cylinderAxisVec = DirectX::XMVector3TransformNormal(cylinderAxisVec, inverseTransform);

	// ���Ɖ~���̓����蔻��
	// ���̒��S���~���̒�ʂɎˉe�����_�����߂�
	DirectX::XMVECTOR sphereToBase =DirectX::XMVectorSubtract( sphereCenterVec , cylinderBaseCenterVec);
	DirectX::XMVECTOR projectionOnAxis =DirectX::XMVectorMultiply( DirectX::XMVector3Dot(sphereToBase, cylinderAxisVec) , cylinderAxisVec);
	DirectX::XMVECTOR closestPointOnCylinder =DirectX::XMVectorAdd( cylinderBaseCenterVec , projectionOnAxis);

	// �~���̎��ɉ������ŋߐړ_�����߂�
	DirectX::XMVECTOR cylinderAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR sphereToClosestPoint =DirectX::XMVectorSubtract( closestPointOnCylinder , sphereCenterVec);
	float distanceAlongAxis = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToClosestPoint, cylinderAxis));

	// �~���̒�ʂ����ʂ܂ł̍��������߂�
	float cylinderHalfHeight = cylinderHeight / 2.0f;

	// ���̒��S���~���̎��ɉ������ŋߐړ_�Ɖ~���̍����͈͓̔��ɂ��邩�𔻒�
	bool inHeightRange = (distanceAlongAxis >= -cylinderHalfHeight) && (distanceAlongAxis <= cylinderHalfHeight);

	// �ŋߐړ_�����̕\�ʂ���~���̔��a�ȓ��ɂ��邩�𔻒�
	DirectX::XMVECTOR sphereToClosestPointLengthSq = DirectX::XMVector3LengthSq(sphereToClosestPoint);
	float closestPointDistanceSq = DirectX::XMVectorGetX(sphereToClosestPointLengthSq);
	float combinedRadius = sphereRadius + cylinderRadius;
	bool withinRadius = closestPointDistanceSq <= combinedRadius * combinedRadius;

	// ����
	return inHeightRange && withinRadius;
}
