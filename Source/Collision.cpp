#include"Collision.h"

//‹…‚Æ‹…‚ÌŒð·”»’è
bool Collision::IntersectSphereVsSphre(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A¨B‚Ì’PˆÊƒxƒNƒgƒ‹‚ðŽZo
	//XMFLOAT‚ðXMVECTOR‚É•ÏŠ·
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	//Vec=B-A
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	//XMVector3LengthSqŠÖ”‚Å’·‚³‚ðŒvŽZ@sqrt‚µ‚Ä‚¢‚È‚¢LengthSq‚Ì‚Ù‚¤‚ª‘‚¢
	DirectX::XMVECTOR LengthSq= DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//‹——£”»’è(”¼ŒaA+”¼ŒaB)
	float range = radiusA + radiusB;
	//sqrt‚µ‚È‚©‚Á‚½•ª”¼Œa‚ð2æ‚·‚é
	if (lengthSq>range*range)
	{
		return false;
	}

	//A‚ªB‚ð‰Ÿ‚µo‚·
	
	//ƒ^[ƒQƒbƒg•ûŒü‚Ö‚ÌƒxƒNƒgƒ‹(Vec)‚ð³‹K‰»
	Vec = DirectX::XMVector3Normalize(Vec);

	//³‹K‰»‚µ‚½ƒxƒNƒgƒ‹‚ðrangeƒXƒP[ƒ‹(XMVectorScale)
	Vec = DirectX::XMVectorScale(Vec, range);

	//ƒXƒP[ƒ‹‚µ‚½ƒxƒNƒgƒ‹‚ðPositionA‚ÌˆÊ’u‚©‚ç‘«‚·PositionB‚ÍˆÓ–¡‚ª‚È‚¢‰½‚Å‚à‚æ‚¢
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);

	//‘«‚µ‚½’l‚ðXMFLOAT3(outPositionB)‚É•ÏŠ·
	DirectX::XMStoreFloat3(&outPositionB,PositionB );
	return true;
}

//‰~’Œ‚Æ‰~’Œ‚ÌŒð·”»’è
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
	//A‚Ì‘«Œ³‚ªB‚Ì“ª‚æ‚èã‚È‚ç“–‚½‚Á‚Ä‚¢‚È‚¢
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//A‚Ì“ª‚ªB‚Ì‘«Œ³‚æ‚è‰º‚È‚ç“–‚½‚Á‚Ä‚¢‚È‚¢
	if (positionA.y + heightA < positionB.y)
	{
		return false;

	}
	//XZ•½–Ê‚Å‚Ì”ÍˆÍƒ`ƒFƒbƒN
	//X“¯Žm‚ðˆø‚­
	float vx = positionB.x - positionA.x;

	//Z“¯Žm‚ðˆø‚­
	float vz = positionB.z - positionA.z;

	//XZ‚Ì’·‚³‚ðŒvŽZ‚·‚é
	float distXZ = sqrtf(vx*vx+vz*vz);

	//”¼ŒaA‚Æ”¼ŒaB‚Ì’·‚³‚ðŒvŽZ‚·‚é
	float range = radiusA+radiusB;

	//XZ‚Ì’·‚³‚ª”¼ŒaA‚Æ”¼ŒaB‚Ì’·‚³‚æ‚è‘å‚«‚©‚Á‚½‚ç“–‚½‚Á‚Ä‚È‚¢
	if (distXZ>range)
	{
		return false;
	}
	//A‚ªB‚ð‰Ÿ‚µo‚·
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
	//‹…‚Ì‘«Œ³‚ª‰~’Œ‚Ì“ª‚æ‚èã‚È‚ç“–‚½‚Á‚Ä‚¢‚È‚¢
	if (spherePosition.y-sphereRadius > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//‹…‚Ì“ª‚ª‰~’Œ‚Ì‘«Œ³‚æ‚è‰º‚È‚ç“–‚½‚Á‚Ä‚¢‚È‚¢
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ•½–Ê‚Å‚Ì”ÍˆÍƒ`ƒFƒbƒN
	//X“¯Žm‚ðˆø‚­
	float vx = cylinderPosition.x - spherePosition.x;

	//Z“¯Žm‚ðˆø‚­
	float vz = cylinderPosition.z - spherePosition.z;

	//XZ‚Ì’·‚³‚ðŒvŽZ‚·‚é
	float distXZ = sqrtf(vx * vx + vz * vz);

	//”¼ŒaA‚Æ”¼ŒaB‚Ì’·‚³‚ðŒvŽZ‚·‚é
	float range = sphereRadius + cylinderRadius;

	//XZ‚Ì’·‚³‚ª”¼ŒaA‚Æ”¼ŒaB‚Ì’·‚³‚æ‚è‘å‚«‚©‚Á‚½‚ç“–‚½‚Á‚Ä‚È‚¢
	if (distXZ > range)
	{
		return false;
	}
	//‹…‚ª‰~’Œ‚ð‰Ÿ‚µo‚·
	//vx‚Ævz‚ð³‹K‰»
	vx /= distXZ;
	vz /= distXZ;

	outCylinderPosition.x = (vx * range) + spherePosition.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = vz * range + spherePosition.z;

	return true;

}

//‹…‚Æ‰~’Œ‚Q
bool Collision::SphereCylinderIntersection(
	const DirectX::XMFLOAT3& sphereCenter, float sphereRadius,
	const DirectX::XMFLOAT3& cylinderBaseCenter, float cylinderRadius, float cylinderHeight,
	const DirectX::XMMATRIX& transform)
{
	// ‹ts—ñ‚ðŒvŽZ‚µ‚ÄŒ³‚ÌÀ•WŒn‚É•ÏŠ·
	DirectX::XMMATRIX inverseTransform = DirectX::XMMatrixInverse(nullptr, transform);

	// ‹…‚Ì’†SˆÊ’u‚ðŒ³‚ÌÀ•WŒn‚É•ÏŠ·
	DirectX::XMVECTOR sphereCenterVec = DirectX::XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1.0f);
	sphereCenterVec = DirectX::XMVector3TransformCoord(sphereCenterVec, inverseTransform);

	// ‰~’Œ‚Ì’ê–Ê’†SˆÊ’u‚ðŒ³‚ÌÀ•WŒn‚É•ÏŠ·
	DirectX::XMVECTOR cylinderBaseCenterVec = DirectX::XMVectorSet(cylinderBaseCenter.x, cylinderBaseCenter.y, cylinderBaseCenter.z, 1.0f);
	cylinderBaseCenterVec = DirectX::XMVector3TransformCoord(cylinderBaseCenterVec, inverseTransform);

	// ‰~’Œ‚ÌŽ²•ûŒüiyŽ²•ûŒüj‚ðŒ³‚ÌÀ•WŒn‚É•ÏŠ·
	DirectX::XMVECTOR cylinderAxisVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // yŽ²•ûŒü
	cylinderAxisVec = DirectX::XMVector3TransformNormal(cylinderAxisVec, inverseTransform);

	// ‹…‚Æ‰~’Œ‚Ì“–‚½‚è”»’è
	// ‹…‚Ì’†S‚ð‰~’Œ‚Ì’ê–Ê‚ÉŽË‰e‚µ‚½“_‚ð‹‚ß‚é
	DirectX::XMVECTOR sphereToBase =DirectX::XMVectorSubtract( sphereCenterVec , cylinderBaseCenterVec);
	DirectX::XMVECTOR projectionOnAxis =DirectX::XMVectorMultiply( DirectX::XMVector3Dot(sphereToBase, cylinderAxisVec) , cylinderAxisVec);
	DirectX::XMVECTOR closestPointOnCylinder =DirectX::XMVectorAdd( cylinderBaseCenterVec , projectionOnAxis);

	// ‰~’Œ‚ÌŽ²‚É‰ˆ‚Á‚½Å‹ßÚ“_‚ð‹‚ß‚é
	DirectX::XMVECTOR cylinderAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR sphereToClosestPoint =DirectX::XMVectorSubtract( closestPointOnCylinder , sphereCenterVec);
	float distanceAlongAxis = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToClosestPoint, cylinderAxis));

	// ‰~’Œ‚Ì’ê–Ê‚©‚çã–Ê‚Ü‚Å‚Ì‚‚³‚ð‹‚ß‚é
	float cylinderHalfHeight = cylinderHeight / 2.0f;

	// ‹…‚Ì’†S‚ª‰~’Œ‚ÌŽ²‚É‰ˆ‚Á‚½Å‹ßÚ“_‚Æ‰~’Œ‚Ì‚‚³‚Ì”ÍˆÍ“à‚É‚ ‚é‚©‚ð”»’è
	bool inHeightRange = (distanceAlongAxis >= -cylinderHalfHeight) && (distanceAlongAxis <= cylinderHalfHeight);

	// Å‹ßÚ“_‚ª‹…‚Ì•\–Ê‚©‚ç‰~’Œ‚Ì”¼ŒaˆÈ“à‚É‚ ‚é‚©‚ð”»’è
	DirectX::XMVECTOR sphereToClosestPointLengthSq = DirectX::XMVector3LengthSq(sphereToClosestPoint);
	float closestPointDistanceSq = DirectX::XMVectorGetX(sphereToClosestPointLengthSq);
	float combinedRadius = sphereRadius + cylinderRadius;
	bool withinRadius = closestPointDistanceSq <= combinedRadius * combinedRadius;

	// ”»’è
	return inHeightRange && withinRadius;
}
