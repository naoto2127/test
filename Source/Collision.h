#pragma once

#include<DirectXMath.h>

//�R���W����
class Collision
{
public:
	//���Ƌ��̌�������
	static bool IntersectSphereVsSphre(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& PositionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	//�~���Ɖ~���̌�������
	static bool IntersectCylonderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	//���Ɖ~���̌�������
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);


	//���Ɖ~���Q
	static bool SphereCylinderIntersection(
		const DirectX::XMFLOAT3& sphereCenter, float sphereRadius,
		const DirectX::XMFLOAT3& cylinderBaseCenter, float cylinderRadius, float cylinderHeight,
		const DirectX::XMMATRIX& transform);
};
