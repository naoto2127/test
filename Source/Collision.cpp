#include"Collision.h"

//球と球の交差判定
bool Collision::IntersectSphereVsSphre(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& outPositionB
)
{
	//A→Bの単位ベクトルを算出
	//XMFLOATをXMVECTORに変換
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
	//Vec=B-A
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB,PositionA);
	//XMVector3LengthSq関数で長さを計算　sqrtしていないLengthSqのほうが早い
	DirectX::XMVECTOR LengthSq= DirectX::XMVector3LengthSq(Vec);
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	//距離判定(半径A+半径B)
	float range = radiusA + radiusB;
	//sqrtしなかった分半径を2乗する
	if (lengthSq>range*range)
	{
		return false;
	}

	//AがBを押し出す
	
	//ターゲット方向へのベクトル(Vec)を正規化
	Vec = DirectX::XMVector3Normalize(Vec);

	//正規化したベクトルをrangeスケール(XMVectorScale)
	Vec = DirectX::XMVectorScale(Vec, range);

	//スケールしたベクトルをPositionAの位置から足すPositionBは意味がない何でもよい
	PositionB = DirectX::XMVectorAdd(PositionA, Vec);

	//足した値をXMFLOAT3(outPositionB)に変換
	DirectX::XMStoreFloat3(&outPositionB,PositionB );
	return true;
}

//円柱と円柱の交差判定
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
	//Aの足元がBの頭より上なら当たっていない
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当たっていない
	if (positionA.y + heightA < positionB.y)
	{
		return false;

	}
	//XZ平面での範囲チェック
	//X同士を引く
	float vx = positionB.x - positionA.x;

	//Z同士を引く
	float vz = positionB.z - positionA.z;

	//XZの長さを計算する
	float distXZ = sqrtf(vx*vx+vz*vz);

	//半径Aと半径Bの長さを計算する
	float range = radiusA+radiusB;

	//XZの長さが半径Aと半径Bの長さより大きかったら当たってない
	if (distXZ>range)
	{
		return false;
	}
	//AがBを押し出す
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
	//球の足元が円柱の頭より上なら当たっていない
	if (spherePosition.y-sphereRadius > cylinderPosition.y + cylinderHeight)
	{
		return false;
	}
	//球の頭が円柱の足元より下なら当たっていない
	if (spherePosition.y + sphereRadius < cylinderPosition.y)
	{
		return false;
	}
	//XZ平面での範囲チェック
	//X同士を引く
	float vx = cylinderPosition.x - spherePosition.x;

	//Z同士を引く
	float vz = cylinderPosition.z - spherePosition.z;

	//XZの長さを計算する
	float distXZ = sqrtf(vx * vx + vz * vz);

	//半径Aと半径Bの長さを計算する
	float range = sphereRadius + cylinderRadius;

	//XZの長さが半径Aと半径Bの長さより大きかったら当たってない
	if (distXZ > range)
	{
		return false;
	}
	//球が円柱を押し出す
	//vxとvzを正規化
	vx /= distXZ;
	vz /= distXZ;

	outCylinderPosition.x = (vx * range) + spherePosition.x;
	outCylinderPosition.y = cylinderPosition.y;
	outCylinderPosition.z = vz * range + spherePosition.z;

	return true;

}

//球と円柱２
bool Collision::SphereCylinderIntersection(
	const DirectX::XMFLOAT3& sphereCenter, float sphereRadius,
	const DirectX::XMFLOAT3& cylinderBaseCenter, float cylinderRadius, float cylinderHeight,
	const DirectX::XMMATRIX& transform)
{
	// 逆行列を計算して元の座標系に変換
	DirectX::XMMATRIX inverseTransform = DirectX::XMMatrixInverse(nullptr, transform);

	// 球の中心位置を元の座標系に変換
	DirectX::XMVECTOR sphereCenterVec = DirectX::XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1.0f);
	sphereCenterVec = DirectX::XMVector3TransformCoord(sphereCenterVec, inverseTransform);

	// 円柱の底面中心位置を元の座標系に変換
	DirectX::XMVECTOR cylinderBaseCenterVec = DirectX::XMVectorSet(cylinderBaseCenter.x, cylinderBaseCenter.y, cylinderBaseCenter.z, 1.0f);
	cylinderBaseCenterVec = DirectX::XMVector3TransformCoord(cylinderBaseCenterVec, inverseTransform);

	// 円柱の軸方向（y軸方向）を元の座標系に変換
	DirectX::XMVECTOR cylinderAxisVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // y軸方向
	cylinderAxisVec = DirectX::XMVector3TransformNormal(cylinderAxisVec, inverseTransform);

	// 球と円柱の当たり判定
	// 球の中心を円柱の底面に射影した点を求める
	DirectX::XMVECTOR sphereToBase =DirectX::XMVectorSubtract( sphereCenterVec , cylinderBaseCenterVec);
	DirectX::XMVECTOR projectionOnAxis =DirectX::XMVectorMultiply( DirectX::XMVector3Dot(sphereToBase, cylinderAxisVec) , cylinderAxisVec);
	DirectX::XMVECTOR closestPointOnCylinder =DirectX::XMVectorAdd( cylinderBaseCenterVec , projectionOnAxis);

	// 円柱の軸に沿った最近接点を求める
	DirectX::XMVECTOR cylinderAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR sphereToClosestPoint =DirectX::XMVectorSubtract( closestPointOnCylinder , sphereCenterVec);
	float distanceAlongAxis = DirectX::XMVectorGetX(DirectX::XMVector3Dot(sphereToClosestPoint, cylinderAxis));

	// 円柱の底面から上面までの高さを求める
	float cylinderHalfHeight = cylinderHeight / 2.0f;

	// 球の中心が円柱の軸に沿った最近接点と円柱の高さの範囲内にあるかを判定
	bool inHeightRange = (distanceAlongAxis >= -cylinderHalfHeight) && (distanceAlongAxis <= cylinderHalfHeight);

	// 最近接点が球の表面から円柱の半径以内にあるかを判定
	DirectX::XMVECTOR sphereToClosestPointLengthSq = DirectX::XMVector3LengthSq(sphereToClosestPoint);
	float closestPointDistanceSq = DirectX::XMVectorGetX(sphereToClosestPointLengthSq);
	float combinedRadius = sphereRadius + cylinderRadius;
	bool withinRadius = closestPointDistanceSq <= combinedRadius * combinedRadius;

	// 判定
	return inHeightRange && withinRadius;
}
