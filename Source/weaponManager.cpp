#include"weaponManager.h"

//コンストラクタ
WeaponManager::WeaponManager()
{

}

//デストラクタ
WeaponManager::~WeaponManager()
{
	Clear();
}

//更新処理
void WeaponManager::Update(float elapsedTime)
{
	//更新処理
	for (Weapon* weapon : weapons)
	{
		weapon->Update(elapsedTime);
	}

	//破棄処理
	//※projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
	//更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する
	for (Weapon* weapon : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Weapon*>::iterator it = std::find(weapons.begin(), weapons.end(), weapon);
		if (it != weapons.end())
		{
			weapons.erase(it);
		}

		//弾丸の破棄処理
		delete weapon;
	}
	//破棄リストをクリア
	removes.clear();

}

//描画処理
void WeaponManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (Weapon* weapon : weapons)
	{
		weapon->Render(context, shader);
	}
}

//デバッグプリミティブ描画
void WeaponManager::DrawDebugPrimitive()
{
	for (Weapon* weapon : weapons)
	{
		weapon->DrawDebugPrimitive();
	}
}

//弾丸登録
void WeaponManager::Register(Weapon* weapon)
{
	weapons.emplace_back(weapon);
}

//弾丸全削除
void WeaponManager::Clear()
{
	for (Weapon* weapon : weapons)
	{
		delete weapon;
	}

	weapons.clear();
}

//弾丸削除
void WeaponManager::Remove(Weapon* weapon)
{
	//破棄リストに追加
	removes.insert(weapon);
}