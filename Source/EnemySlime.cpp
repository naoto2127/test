#include"EnemySlime.h"

//コンストラクタ
EnemySlime::EnemySlime()
{
	model = new Model("Data/Model/Slime/Slime.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y=scale.z = 0.01f;

	//幅、高さ設定
	radius = 0.5f;
	height = 1.0f;
}

EnemySlime::~EnemySlime()
{
	delete model;
}

//更新処理
void EnemySlime::Update(float elapsedTime)
{
	//速力処理更新
	UpdateVelocity(elapsedTime);

	//無敵時間更新
	UpdateInvincibleTimer(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform(transform);
}

//描画処理
void EnemySlime::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

//死亡したときに呼ばれる
void EnemySlime::OnDead()
{
	//自分自身を削除
	Destroy();
}