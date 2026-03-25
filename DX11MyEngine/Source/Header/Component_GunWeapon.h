#pragma once
#include "Component_WeaponBase.h"
#include "ConstantWeaponData.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:GunWeapon Class --- */
//
//  ★継承：IComponent ★
//
// 【?】アサルトライフル
//		
// ***************************************************************************************
class GunWeapon : public WeaponBase
{
private:
	std::weak_ptr<class LineRenderer> m_pLineRendererComp;	// レーザー用のライン
	std::weak_ptr<class PointLight> m_pFlashPointLight;		// 発射時のフラッシュ用
	WeaponData::GunWeaponData m_WeaponParameter;
	BulletData::BULLET_TYPE m_BulletType;

public:
	GunWeapon(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	~GunWeapon();

	void Start(RendererEngine& renderer) override;		// 初期化
	void Update(RendererEngine& renderer) override;		// 更新処理
	void Draw(RendererEngine& renderer)override;		// 描画処理
};



