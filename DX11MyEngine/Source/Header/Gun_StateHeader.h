#pragma once
#include "IState.h"


//////////////////////////////////////////////////////////////////////////////////////////
//
//
//						Gunステートのクラス定義をまとめる
//						クラス外定義はファイルごとに分ける
// 
//
//////////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 銃系武器のステート
/// </summary>
enum GUN_STATE
{
	GUN_STATE_IDLE,			// 待機
	GUN_STATE_RELOADING,	// リロード中
	GUN_STATE_FIRE,			// 発車

	GUN_STATE_NUM,
};

class GunWeapon;

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Gun_IdleState Class --- */
//  ★継承：IState ★
// 
// 【?】[待機時]ステート
// ***************************************************************************************
class Gun_IdleState : public IState<GunWeapon>
{
private:

public:
	void OnEnter(GunWeapon* pOwner) override;
	void OnExit(GunWeapon* pOwner)override;
	int Update(GunWeapon* pOwner)override;
};

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Gun_ReloadingState Class --- */
//  ★継承：IState ★
// 
// 【?】[リロード中]ステート
// ***************************************************************************************
class Gun_ReloadingState : public IState< GunWeapon>
{
private:
	float m_ReloadElapsed = 0.0f;	// リロード経過時間
	class GameObject* m_pReloadBarSpriteObj;		// リロードバー
	class GameObject* m_pReloadBarBackSpriteObj;	// リロードバー背景
	const VECTOR2::VEC2 RELOAD_BAR_POS = VECTOR2::VEC2(-100.0f, -10.0f);	 // リロードバー位置（真ん中が中心にしてるので、サイズの半分引く）
	const VECTOR2::VEC2 RELOAD_BAR_SIZE = VECTOR2::VEC2(200.0f, 20.0f);		 // リロードバーサイズ
	const VECTOR2::VEC2 RELOAD_BAR_BACK_POS = VECTOR2::VEC2(0.0f, 0.0f);	 // リロードバー背景位置
	const VECTOR2::VEC2 RELOAD_BAR_BACK_SIZE = VECTOR2::VEC2(210.0f, 30.0f); // リロードバー背景サイズ
	const VECTOR2::VEC2 RELOAD_TEXT_POS = VECTOR2::VEC2(0.0f, 0.0f);	 // リロードテキスト位置

public:
	void OnEnter(GunWeapon* pOwner) override;
	void OnExit(GunWeapon* pOwner)override;
	int Update(GunWeapon* pOwner)override;
};

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Gun_FireState Class --- */
//  ★継承：IState ★
// 
// 【?】[弾発射]ステート
// ***************************************************************************************
class Gun_FireState : public IState< GunWeapon>
{
private:
	float m_FireInterval = 0.0f;	// 発射間隔
	float m_ShootTimer = 9999.0f;   // 発射タイマー

public:
	void OnEnter(GunWeapon* pOwner) override;
	void OnExit(GunWeapon* pOwner)override;
	int Update(GunWeapon* pOwner)override;
};