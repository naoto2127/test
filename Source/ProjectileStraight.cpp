#include"ProjectileStraight.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager*manager)
	:Projectile(manager)
{

	model = new Model("Data/Model/Sword/Sword.mdl");

	//表示サイズを調整
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
ProjectileStraight::~ProjectileStraight()
{
	delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	LifeTimer -= elapsedTime;
	if (LifeTimer<=0)
	{
		//自分を削除
		Destroy();
	}

	//移動
	float speed = this->speed * elapsedTime;

	//位置＋＝前方向*速さ
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;


	//オブジェクト行列を更新
	UpdateTransform();


	//モデル行列更新処理
	model->UpdateTransform(transform);


}

//描画処理
void ProjectileStraight::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}
