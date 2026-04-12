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
	//WeaponData::GunWeaponData m_WeaponParameter;
	float m_Range;	// 射程距離
	int m_AmmoRemaining;	// 残弾数
	bool m_IsNowZoom;		// ズーム中かどうか
	bool m_IsStopFire;		// 発射止める
	StateMachine<GunWeapon>m_StateMachine;

public:
	GunWeapon( std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	~GunWeapon();

	void Start(RendererEngine& renderer) override;		// 初期化
	void LateUpdate(RendererEngine& renderer) override;		// 更新処理
	void Draw(RendererEngine& renderer)override;		// 描画処理

	void Shoot(RendererEngine& renderer);	// 弾の発射
	bool Setup(const WeaponData::BaseWeaponData* _pWeaponData)override;
	void SwicthReset() override;	// 武器切り替え時のリセット

	std::weak_ptr<class PointLight> get_FlashPointLight() { return m_pFlashPointLight; }	// フラッシュライト取得
	const WeaponData::GunWeaponData* get_GunWeaponParameter()const;		// 武器のパラメータ取得


	bool get_IsNowZoom()const { return m_IsNowZoom; }								// ズームフラグの取得
	bool get_IsStopFire()const { return m_IsStopFire; }								// 攻撃中止フラグの取得
	void set_IsStopFire(bool _flag) { m_IsStopFire = _flag; }						// 攻撃中止フラグの設定
	int get_AmmoRemaining()const { return m_AmmoRemaining; }						// 残弾数の取得
	void set_AmmoRemaining(int _ammo) { m_AmmoRemaining = _ammo; }					// 残弾数の設定

	WeaponData::WeaponUIData get_WeaponUIData()const override;
};



