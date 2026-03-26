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

public:
	void OnEnter(GunWeapon* pOwner) override;
	void OnExit(GunWeapon* pOwner)override;
	int Update(GunWeapon* pOwner)override;
};