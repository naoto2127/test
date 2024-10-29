#pragma once

#include<vector>
#include"weapon.h"
#include<set>

//弾丸マネジャー
class WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//弾丸登録
	void Register(Weapon* weapon);

	//弾丸全削除
	void Clear();

	//弾丸数取得
	int GetWeaponCount()const { return static_cast<int>(weapons.size()); }

	//武器取得
	Weapon* GetWeapon(int index) { return weapons.at(index); }

	//弾丸削除
	void Remove(Weapon* weapon);

private:
	std::vector<Weapon*> weapons;
	std::set<Weapon*> removes;
};

