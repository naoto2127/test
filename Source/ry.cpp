#include"ry.h"

#include"Graphics/Graphics.h"




//コンストラクタ
Ry::Ry()
{
	
}

Ry::~Ry()
{
}

//デバッグプリミティブ描画
void Ry::DrawDebugPrimitive()
{

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();


	//衝突判定用のデバッグ球を描画
	/*DirectX::XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&direction)));*/
	// 四角形を描画する例
	DirectX::XMFLOAT2 center = DirectX::XMFLOAT2(10.0f, 0.0f); // 四角形の中心座標
	float width = 10.0f; // 四角形の幅
	float height = 1.0f; // 四角形の高さ
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f); // 四角形の色

	
	debugRenderer->DrawRectangle(center, width, height, color);

}

//行列更新処理
void Ry::UpdateTransform()
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


	// 算出した軸ベクトルから行列を作成
	DirectX::XMMATRIX transform2;

	transform2.r[0] = DirectX::XMVectorSet(right.x * scale.x, right.y * scale.x, right.z * scale.x, 0.0f);
	transform2.r[1] = DirectX::XMVectorSet(up.x * scale.y, up.y * scale.y, up.z * scale.y, 0.0f);
	transform2.r[2] = DirectX::XMVectorSet(front.x * scale.z, front.y * scale.z, front.z * scale.z, 0.0f);
	transform2.r[3] = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

	
	// 発射方向を設定する（frontベクトル）
	this->direction = front;

	DirectX::XMStoreFloat4x4(&transform, transform2);

	
}


//破棄
void Ry::Destroy()
{
	
}