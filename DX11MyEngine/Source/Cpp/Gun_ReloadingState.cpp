#include "pch.h"
#include "Gun_StateHeader.h"
#include "Component_GunWeapon.h"
#include "GameObject.h"

using namespace VECTOR3;
using namespace VECTOR2;

//*---------------------------------------------------------------------------------------
//* @:Gun_ReloadingState Class 
//*【?】開始
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_ReloadingState::OnEnter(GunWeapon* pOwner)
{
	pOwner->get_WeaponFlags().EnableFlag(WEAPON_STATUS::RELODING);
	pOwner->set_AmmoRemaining(0); // リロード開始時、弾を0に

	m_ReloadElapsed = 0.0f;
}

//*---------------------------------------------------------------------------------------
//* @:Gun_ReloadingState Class 
//*【?】終了
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_ReloadingState::OnExit(GunWeapon* pOwner)
{
	pOwner->get_WeaponFlags().DisableFlag(WEAPON_STATUS::RELODING);

	m_ReloadElapsed = 0.0f;
}

//*---------------------------------------------------------------------------------------
//* @:Gun_ReloadingState Class 
//*【?】更新
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
int Gun_ReloadingState::Update(GunWeapon* pOwner)
{
	const auto& weapon_param = pOwner->get_GunWeaponParameter();
	m_ReloadElapsed += Master::m_pTimeManager->get_DeltaTime();


	float t = m_ReloadElapsed / weapon_param->_reloadTime;

	// リロード終了
	if (m_ReloadElapsed >= weapon_param->_reloadTime)
	{
		pOwner->set_AmmoRemaining(weapon_param->_bulletMaxNum); // 弾込め

		return GUN_STATE::GUN_STATE_IDLE;
	}

	return GUN_STATE::GUN_STATE_RELOADING;
}
