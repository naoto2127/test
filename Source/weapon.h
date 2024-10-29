#pragma once

#include"Graphics/Shader.h"

//前方宣言
class WeaponManager;

//弾丸
class Weapon
{
public:
	Weapon(WeaponManager* manager);
	virtual ~Weapon() {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition()const { return position; }

	//方向取得
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	//破棄
	void Destroy();

	//半径取得
	float GetRadius()const { return radius; }

	//高さ取得
	float GetHeight()const { return height; }

	// 回転行列取得
	DirectX::XMFLOAT4X4 GetRotationMatrix() const { return rotationMatrix; }



protected:
	//行列更新処理
	void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	DirectX::XMFLOAT4X4 rotationMatrix = {}; // 回転行列の初期化
	float angle = 90.0f;
	WeaponManager* manager = nullptr;

	float height = 2.0f;

	float radius = 0.5f;
};
