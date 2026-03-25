#pragma once
#include "IComponent.h"
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponBase Class --- */
//
//  ★継承：IComponent ★
//
// 【?】武器の基底コンポーネント
//		
// ***************************************************************************************
class WeaponBase : public IComponent
{
private:
	std::weak_ptr<class LineRenderer> m_pLineRendererComp;	// レーザー用のライン
	std::weak_ptr<class PointLight> m_pFlashPointLight;	// 発射時のフラッシュ用
	bool m_IsExplosionBullet = true;	// 弾の切り替えフラグ（一時的なもの　後で消す）
	int m_FireRate;						// 発射レート

public:
	WeaponBase(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	virtual ~WeaponBase() = default;
};