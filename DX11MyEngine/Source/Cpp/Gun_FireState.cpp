#include "pch.h"
#include "Gun_StateHeader.h"
#include "Component_GunWeapon.h"
#include "Component_PointLight.h"
#include "Component_3DCamera.h"
#include "GameObject.h"
#include "RendererEngine.h"
#include "Component_PlayerController.h"

using namespace VECTOR3;
using namespace VECTOR2; 
using namespace BulletData;
using namespace WeaponData;

//*---------------------------------------------------------------------------------------
//* @:Gun_FireState Class 
//*【?】開始
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_FireState::OnEnter(GunWeapon* pOwner)
{
	pOwner->get_WeaponFlags().EnableFlag(WEAPON_STATUS::FIRE);

	// 発射
	pOwner->Fire(*m_pRenderer);
}

//*---------------------------------------------------------------------------------------
//* @:Gun_FireState Class 
//*【?】終了
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_FireState::OnExit(GunWeapon* pOwner)
{
	pOwner->get_WeaponFlags().DisableFlag(WEAPON_STATUS::FIRE);
}

//*---------------------------------------------------------------------------------------
//* @:Gun_FireState Class 
//*【?】更新
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
int Gun_FireState::Update(GunWeapon* pOwner)
{
    if (pOwner->get_IsStopFire())return GUN_STATE::GUN_STATE_IDLE;
	float deltaTime = Master::m_pTimeManager->get_DeltaTime();

    auto player = Master::m_pGameObjectManager->get_ObjectByTag("Player");
    if (player->get_Component<PlayerController>()->get_AnimID() == PlayerData::PLAYER_RANGER_ANIM_ID::RUNING_DIVE_ROLL)return GUN_STATE::GUN_STATE_IDLE;

	const auto& weapon_param = pOwner->get_GunWeaponData();
	int ammoRemaining = pOwner->get_AmmoRemaining();

	// 左クリックで発射続ける
	if (GetMouseClick(MOUSE_BUTTON_STATE::LEFT))
	{
		pOwner->Fire(*m_pRenderer);
	}
	// 何もなし
	else
	{
		return GUN_STATE::GUN_STATE_IDLE;
	}

	// リロード
	if (GetInputDown(GAME_CONFIG::WEAPON_RELOAD) || ammoRemaining <= 0)
	{
		return GUN_STATE::GUN_STATE_RELOADING;
	}


	return GUN_STATE::GUN_STATE_FIRE;
}
