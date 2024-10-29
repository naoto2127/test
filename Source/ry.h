#pragma once

#include"Graphics/Shader.h"



//レイ
class Ry
{
public:
	Ry();
	~Ry();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

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

	

protected:
	//行列更新処理
	void UpdateTransform();

protected:
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMFLOAT3 direction = { 0,0,1 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	
	


	float height = 2.0f;

	float radius = 0.5f;
};
