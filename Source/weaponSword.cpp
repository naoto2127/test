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

//コンストラクタ
WeaponSword::WeaponSword(WeaponManager* manager)
	:Weapon(manager)
{

	model = new Model("Data/Model/Sword/Sword.mdl");

	//表示サイズを調整
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
WeaponSword::~WeaponSword()
{
	delete model;
}

//更新処理
void WeaponSword::Update(float elapsedTime)
{
	//寿命処理
	LifeTimer -= elapsedTime;
	if (LifeTimer <= 0)
	{
		//自分を削除
		Destroy();
	}

	//移動
	float speed = this->speed * elapsedTime;



	////位置＋＝前方向*速さ
	//position.x += direction.x * speed;
	//position.y += direction.y * speed;
	//position.z += direction.z * speed;


	//オブジェクト行列を更新
	UpdateTransform();


	//モデル行列更新処理
	model->UpdateTransform(transform);

	
		
	angle += 0.08745;
	
	angle = normalizeAngle(angle);

	
}

//描画処理
void WeaponSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

//発射
void WeaponSword::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
