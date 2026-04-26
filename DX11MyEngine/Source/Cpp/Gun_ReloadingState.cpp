#include "pch.h"
#include "Gun_StateHeader.h"
#include "Component_GunWeapon.h"
#include "Component_SpriteRenderer.h"
#include "GameObject.h"

using namespace VECTOR4;
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

	//// スプライトをプールへ返す
	//m_pReloadBarSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	//m_pReloadBarBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
}

//*---------------------------------------------------------------------------------------
//* @:Gun_ReloadingState Class 
//*【?】更新
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
int Gun_ReloadingState::Update(GunWeapon* pOwner)
{
	const auto& weapon_param = pOwner->get_GunWeaponData();
	m_ReloadElapsed += Master::m_pTimeManager->get_DeltaTime();

	//float t = m_ReloadElapsed / weapon_param->_reloadTime;

	//auto rect = m_pReloadBarSpriteObj->get_RectTransform().lock();
	//rect->set_Size(RELOAD_BAR_SIZE.x * t, RELOAD_BAR_SIZE.y);	// 横幅を動かす

	pOwner->set_CrntReloadTime(m_ReloadElapsed);	// オーナーに現在のリロード時間渡す

	// リロード終了
	if (m_ReloadElapsed >= weapon_param->_reloadTime)
	{
		pOwner->set_AmmoRemaining(weapon_param->_bulletMaxNum); // 弾込め

		return GUN_STATE::GUN_STATE_IDLE;
	}
	//float width = Master::m_pDataManager->get_ScreenWidth();
	//float height = Master::m_pDataManager->get_ScreenHeight();;
	//Master::m_pDirectWriteManager->DrawStringToAligment("リロード中", RELOAD_TEXT_POS, "White_20_STD", H_ALIGNMENT::CENTER, V_ALIGNMENT::CENTER, VEC2(width, height));
	return GUN_STATE::GUN_STATE_RELOADING;
}
