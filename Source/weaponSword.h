#pragma once

#include"Graphics/Model.h"
#include"Weapon.h"

//Œ•
class WeaponSword :public Weapon
{
public:
	WeaponSword(WeaponManager* manager);
	~WeaponSword()override;

	//XVˆ—
	void Update(float elapsedTime)override;

	void Render(ID3D11DeviceContext* dc, Shader* shader)override;

	//”­Ë
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
	Model* model = nullptr;
	float speed = 10.0f;

	float LifeTimer = 0.3f;

};
