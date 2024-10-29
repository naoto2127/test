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

//コンストラクタ
Weapon::Weapon(WeaponManager* manager) :manager(manager)
{
	manager->Register(this);
}

//デバッグプリミティブ描画
void Weapon::DrawDebugPrimitive()
{

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//衝突判定用のデバッグ球を描画
	DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&direction)));
	debugRenderer->DrawSphere(pos, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	
}

//行列更新処理
void Weapon::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// 前方向ベクトルを計算（すでに持っている前方向ベクトルをXMVECTORに変換する）
	Front = DirectX::XMLoadFloat3(&direction);

	// 正規化する
	Front = DirectX::XMVector3Normalize(Front);

	// 仮の上ベクトルを算出(Yにだけ値の入っているXMVECTORを作成する)
	Up = DirectX::XMVectorSet(0.001f, 1.0f, 0.0f, 0.0f);

	// 正規化する
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトルを算出(前のベクトルと仮の上ベクトルの外積)
	Right = DirectX::XMVector3Cross(Up, Front);

	// 正規化する
	Right = DirectX::XMVector3Normalize(Right);

	// 上ベクトルを計算(前ベクトルと右ベクトルの外積から正しい上ベクトルを計算)
	Up = DirectX::XMVector3Cross(Front, Right);

	// 結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// X軸回りの回転行列を作成
	float angleX = DirectX::XM_PIDIV2; // 90度をラジアンで表現した定数
	DirectX::XMMATRIX XRotation = DirectX::XMMatrixRotationX(angle);
	DirectX::XMMATRIX X2Rotation = DirectX::XMMatrixRotationX(normalizeAngle(90));
	DirectX::XMMATRIX ZRotation = DirectX::XMMatrixRotationZ(normalizeAngle(90));

	// 算出した軸ベクトルから行列を作成
	DirectX::XMMATRIX transform2;

	transform2.r[0] = DirectX::XMVectorSet(right.x * scale.x, right.y * scale.x, right.z * scale.x, 0.0f);
	transform2.r[1] = DirectX::XMVectorSet(up.x * scale.y, up.y * scale.y, up.z * scale.y, 0.0f);
	transform2.r[2] = DirectX::XMVectorSet(front.x * scale.z, front.y * scale.z, front.z * scale.z, 0.0f);
	transform2.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

	// X軸回りの回転を適用
	transform2 =ZRotation *XRotation * X2Rotation * transform2;

	// 発射方向を設定する（frontベクトル）
	this->direction = front;

	DirectX::XMStoreFloat4x4(&transform, transform2);

	// 回転行列を更新する
	DirectX::XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixTranspose(XRotation ));

}


//破棄
void Weapon::Destroy()
{
	manager->Remove(this);
}