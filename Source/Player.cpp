#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/graphics.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileStraight.h"
#include "ProjectileHoming.h"
#include"weaponSword.h"
#include"ry.h"

#include <Audio/Audio.h>
#include "Audio/AudioSource.h"


//コンストラクタ
Player::Player()
{
	model = new Model("Data/model/Mr.Incredible/Mr.Incredible.mdl");

	//モデルが大きいのでスケーリング
	scale.x = scale.y = scale.z = 0.01f;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Hit.efk");
	

}

//デストラクタ
Player::~Player()
{
	delete hitEffect;

	delete model;
}

//更新処理
void Player::Update(float elapsedTime)
{
	

	//移動入力処理
	InputMove(elapsedTime);

	//ジャンプ入力処理
	InputJump();

	//弾丸入力処理
	InputProjectile();

	//武器入力情報
	InputWeapon();

	//速力処理更新
	UpdateVelocity(elapsedTime);

	//弾丸更新処理
	projectileManager.Update(elapsedTime);

	//武器更新処理
	weaponManager.Update(elapsedTime);



	//プレイヤーと敵との衝突処理
	CollisionPlayerVsEnemies();

	//弾丸と敵の衝突処理
	CollisionProjectilesVsEnemies();

	//武器と敵の衝突処理
	CollisionWeaponsVsEnemies();

	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新処理
	model->UpdateTransform(transform);
}


//移動入力処理
void Player::InputMove(float elapsedTime)
{
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	//移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);

	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);

	//弾丸描画処理
	projectileManager.Render(dc,shader);

	//武器描画処理
	weaponManager.Render(dc, shader);
}

//デバッグ用GUI描画
void Player::DrawdebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		//トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//位置
			ImGui::InputFloat3("Position", &position.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y= DirectX::XMConvertToDegrees(angle.y);
			a.z= DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &a.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			ImGui::InputFloat3("Scale", &scale.x);

		}
	}
	ImGui::End();
}

//デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//弾丸デバッグプリミティブ描画
	projectileManager.DrawDebugPrimitive();

	//武器デバッグプリミティブ描画
	weaponManager.DrawDebugPrimitive();

	ry.DrawDebugPrimitive();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
	//入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティック入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはZX平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength= sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//スティックの水平入力値をカメラ右方向に反映し、
	//スティックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = (cameraRightX*ax)+(cameraFrontX*ay);
	vec.z = (cameraRightZ*ax)+(cameraFrontZ*ay);
	//Y軸方向には移動しない
	vec.y = 0.0f;
	return vec;

}



//プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		//衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectCylonderVsCylinder(
			//プレイヤーの位置
			position,
            //プレイヤーの半径
			radius,
			//プレイヤーの高さ
			height,
			//敵の位置
			enemy->GetPosition(),
			//敵の半径
			enemy->GetRadius(),
			//敵の高さ
			enemy->GetHeight(),
			//押し出し後の位置
			outPosition
			))
		{
			//敵の真上付近に当たったかをチェック
			//プレイヤーの位置をXMVECTORに変換
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			//敵の位置をXMVECTORに変換
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//プレイヤー方向へのベクトルを取得し
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			//そのベクトルを正規化
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);
			//上から踏んづけた場合は小ジャンプする
			if (normal.y > 0.8f)
			{
				//小ジャンプする
				Jump(jumpSpeed * 0.5f);

				//1ダメージを与える
				enemy->ApplyDamage(1,0.5f);
			}
			else
			{
				//敵の位置に押し出し後の位置設定
				enemy->setPosition(outPosition);
			}
			////敵の真上付近に当たったかをチェック
			//if (0.5f>=(enemy->GetHeight()+enemy->Getposition().y- position.y))
			//{
			//	//真上付近だったら小ジャンプ（半分の力でジャンプ）
			//	Jump(jumpSpeed * 0.5f);
			//}
			
			  

			
		}
	}

}

//着地した時に呼ばれる
void Player::OnLanding()
{
	//ジャンプ回数リセット
	jumpCount = 0;
}


//ジャンプ入力処理
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_A)
	{
		//現在のジャンプ回数が最大のジャンプ回数より少なかったら
		if (jumpCount < jumpLimit)
		{
			//現在のジャンプ回数を1増やす
			jumpCount++;

			//ジャンプ
			Jump(jumpSpeed);
		}

		
	}
}

//弾丸入力処理
void Player::InputProjectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//直進弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//transform._31
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//transform._33
		//発射位置（プレイヤー腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height/2.0f;
		pos.z = position.z;
		//発射
		ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
		projectile->Launch(dir, pos);
		

	}
	//追尾弾丸発射
	if (gamePad.GetButtonDown() & GamePad::BTN_Y)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0.0f;
		dir.z = cosf(angle.y);

		//発射位置(プレイヤーの腰あたり)
		DirectX::XMFLOAT3 pos;
		pos.x = position.x;
		pos.y = position.y+height*0.5f;
		pos.z = position.z;

		//ターゲット（デフォルトではプレイヤーの前方)
		DirectX::XMFLOAT3 target;
		target.x = position.x + dir.x * 1000.0f;
		target.y = position.y + dir.y * 1000.0f;
		target.z = position.z + dir.z * 1000.0f;

		//一番近くの敵をターゲットにする
		//一番遠い場所を設定しておく
		float dist = FLT_MAX;

		//敵マネジャーを取得
		EnemyManager& enemyManager = EnemyManager::Instance();

		//敵の数を取得
		int enemyCount = enemyManager.GetEnemyCount();

		//一番近い敵を敵の数だけチェックする
		for (int i=0;i<enemyCount;++i)
		{
			//敵との距離判定
			//1番目の敵を取得
			Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
			//プレイヤー（自分)の位置をXMVECTORに変換
			DirectX::XMVECTOR playerPosVec = DirectX::XMLoadFloat3(&position);
			//敵の位置をXMVECTORに変換
			DirectX::XMVECTOR enemyPosVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
			//敵方向へのベクトルを計算
			DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(enemyPosVec, playerPosVec);
			//そのベクトルの長さを計算し
			DirectX::XMVECTOR D = DirectX::XMVector3Length(directionVec);
			//単一の値にする
			float d;
			DirectX::XMStoreFloat(&d, D);

			//今まで計算した距離の中で一番小さい距離なら
			if (d < dist)
			{
				dist = d;

				//targetの位置を更新(敵の位置を設定)
				target = enemy->GetPosition();

				//targetのYだけ腰の高さにしておく
				target.y += enemy->GetHeight() * 0.5f;
			}
		}


		//発射
		ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
		projectile->Launch(dir, pos, target);

	}
}

//武器入力処理
void Player::InputWeapon()
{
	Mouse& mouse = Input::Instance().GetMouse();

	//剣
	if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);//transform._31
		dir.y = 0.0f;
		dir.z = cosf(angle.y);//transform._33
		//発射位置（プレイヤー腰あたり）
		DirectX::XMFLOAT3 pos;
		pos.x = position.x+0.2f;
		pos.y = position.y + height / 2.0f;
		pos.z = position.z;
		//発射
		WeaponSword* weapon = new WeaponSword(&weaponManager);
		weapon->Launch(dir, pos);


	}
	
	
}

//弾丸と敵との衝突処理
void Player::CollisionProjectilesVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	//全ての弾丸と全ての敵を総当たりで衝突処理
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);

			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				//弾丸の位置
				projectile->GetPosition(),
				//弾丸の半径
				projectile->GetRadius(),
				//敵の位置
				enemy->GetPosition(),
				//敵の半径
				enemy->GetRadius(),
				//敵の高さ
				enemy->GetHeight(),			
				//押し出し後の位置
				outPosition
			))
			{
				//ダメージを与える
				if (enemy->ApplyDamage(1, 0.5f))
				{
					//	吹き飛ばす
					{
						DirectX::XMFLOAT3 impulse;
						const float power = 10.0f;//吹き飛ばす力

						///敵の位置を取得
						const DirectX::XMFLOAT3& e = enemy->GetPosition();

						//弾の位置を取得
						const DirectX::XMFLOAT3& p = projectile->GetPosition();

						//弾から敵の位置へのベクトルを取得
						float vx = e.x-p.x;
						float vz = e.z-p.z;

						//そのベクトルを正規化(長さを計算し、長さで割る)
						float lengthXZ =sqrtf(vx*vx+vz*vz);
						vx /= lengthXZ;
						vz /= lengthXZ;

						//衝撃の値を設定(xzは正規化したベクトルをpower分スケーリング)
						impulse.x = vx*power;
						impulse.y = power * 0.5f;//yはちょっと浮かせる
						impulse.z = vz * power;

						enemy->AddImpulse(impulse);
					}

					//ヒットエフェクト再生
					{
						DirectX::XMFLOAT3 e = enemy->GetPosition();
						e.y += enemy->GetHeight() * 0.5f;
						hitEffect->Play(e);
					}

					//弾丸破棄
					projectile->Destroy();
				}




			}
		}
	}
	

}

//武器と敵の衝突処理
void Player::CollisionWeaponsVsEnemies()
{
	//EnemyManager& enemyManager = EnemyManager::Instance();

	////全ての武器と全ての敵を総当たりで衝突処理
	//int weaponCount = weaponManager.GetWeaponCount();
	//int enemyCount = enemyManager.GetEnemyCount();

	//for (int i = 0; i < weaponCount; ++i)
	//{
	//	Weapon* weapon= weaponManager.GetWeapon(i);
	//	for (int j = 0; j < enemyCount; ++j)
	//	{
	//		Enemy* enemy = enemyManager.GetEnemy(j);
	//		DirectX::XMFLOAT3 pos;
	//		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&weapon->GetPosition()), DirectX::XMLoadFloat3(&weapon->GetDirection())));
	//		//衝突処理
	//		DirectX::XMFLOAT3 outPosition;
	//		if (Collision::IntersectSphereVsCylinder(
	//			//武器の位置
	//			pos,
	//			//武器の半径
	//			weapon->GetRadius(),
	//	
	//			//敵の位置
	//			enemy->GetPosition(),
	//			//敵の半径
	//			enemy->GetRadius(),
	//			//敵の高さ
	//			enemy->GetHeight(),
	//			//押し出し後の位置
	//			outPosition
	//		))
	//		{
	//			//ダメージを与える
	//			if (enemy->ApplyDamage(3, 0.5f))
	//			{
	//				//	吹き飛ばす
	//				{
	//					DirectX::XMFLOAT3 impulse;
	//					const float power = 10.0f;//吹き飛ばす力

	//					///敵の位置を取得
	//					const DirectX::XMFLOAT3& e = enemy->GetPosition();

	//					//弾の位置を取得
	//					const DirectX::XMFLOAT3& w = weapon->GetPosition();

	//					//弾から敵の位置へのベクトルを取得
	//					float vx = e.x - w.x;
	//					float vz = e.z - w.z;

	//					//そのベクトルを正規化(長さを計算し、長さで割る)
	//					float lengthXZ = sqrtf(vx * vx + vz * vz);
	//					vx /= lengthXZ;
	//					vz /= lengthXZ;

	//					//衝撃の値を設定(xzは正規化したベクトルをpower分スケーリング)
	//					impulse.x = vx * power;
	//					impulse.y = power * 0.5f;//yはちょっと浮かせる
	//					impulse.z = vz * power;

	//					enemy->AddImpulse(impulse);
	//				}

	//				//ヒットエフェクト再生
	//				{
	//					DirectX::XMFLOAT3 e = enemy->GetPosition();
	//					e.y += enemy->GetHeight() * 0.5f;
	//					hitEffect->Play(e);
	//				}

	//				//弾丸破棄
	//				weapon->Destroy();
	//			}




	//		}
	//	}
	//}

	EnemyManager& enemyManager = EnemyManager::Instance();
	int weaponCount = weaponManager.GetWeaponCount();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < weaponCount; ++i)
	{
		Weapon* weapon = weaponManager.GetWeapon(i);
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&weapon->GetPosition()), DirectX::XMLoadFloat3(&weapon->GetDirection())));
		DirectX::XMFLOAT3 weaponPosition = pos;

		// 武器の中心位置を武器の位置とする
		DirectX::XMVECTOR weaponCenter = DirectX::XMLoadFloat3(&weaponPosition);

		// 剣のモデルの回転行列を取得
		DirectX::XMMATRIX weaponRotationMatrix = DirectX::XMLoadFloat4x4(&weapon->GetRotationMatrix());

		// 武器の向き（方向ベクトル）を計算する
		DirectX::XMVECTOR weaponDirection = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // ローカル空間のz軸方向
		weaponDirection = DirectX::XMVector3TransformNormal(weaponDirection, weaponRotationMatrix);

		// 衝突判定用の出力位置
		DirectX::XMFLOAT3 outPosition;

		for (int j = 0; j < enemyCount; ++j)
		{
			Enemy* enemy = enemyManager.GetEnemy(j);
			DirectX::XMFLOAT3 enemyPosition = enemy->GetPosition();
			float enemyRadius = enemy->GetRadius();
			float enemyHeight = enemy->GetHeight();

			// 球と円柱の衝突判定
			if (Collision::IntersectSphereVsCylinder(
				weaponPosition, // 武器の位置を中心とする
				weapon->GetRadius(),
				enemyPosition, enemyRadius, enemyHeight,
				outPosition))
			{
				// ダメージを与える
				if (enemy->ApplyDamage(2, 0.5f))
				{
					// 吹き飛ばす
					DirectX::XMFLOAT3 impulse;
					const float power = 10.0f;

					// 敵から武器の位置へのベクトルを求める
					DirectX::XMVECTOR vecToEnemy = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&enemyPosition), weaponCenter);
					DirectX::XMFLOAT3 vecToEnemyFloat;
					DirectX::XMStoreFloat3(&vecToEnemyFloat, vecToEnemy);

					// ベクトルを正規化
					float lengthXZ = sqrtf(vecToEnemyFloat.x * vecToEnemyFloat.x + vecToEnemyFloat.z * vecToEnemyFloat.z);
					float vx = vecToEnemyFloat.x / lengthXZ;
					float vz = vecToEnemyFloat.z / lengthXZ;

					// 衝撃ベクトルを設定
					impulse.x = vx * power;
					impulse.y = power * 0.5f; // y軸方向に少し浮かせる
					impulse.z = vz * power;

					enemy->AddImpulse(impulse);

					// ヒットエフェクト再生
					DirectX::XMFLOAT3 hitEffectPosition = enemyPosition;
					hitEffectPosition.y += enemyHeight * 0.5f;
					hitEffect->Play(hitEffectPosition);

					
				}
			}
		}
	}
}
