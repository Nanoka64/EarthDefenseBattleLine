#include "pch.h"
#include "Gun_StateHeader.h"
#include "Component_GunWeapon.h"
#include "Component_PlayerController.h"
#include "Component_PointLight.h"
#include "GameObject.h"
#include "RendererEngine.h"
#include "Component_3DCamera.h"

using namespace VECTOR3;
using namespace VECTOR2;
using namespace BulletData;
using namespace WeaponData;

//*---------------------------------------------------------------------------------------
//* @:Gun_IdleState Class 
//*【?】開始
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_IdleState::OnEnter(GunWeapon* pOwner)
{
    pOwner->get_WeaponFlags().EnableFlag(WEAPON_STATUS::IDLE);
}

//*---------------------------------------------------------------------------------------
//* @:Gun_IdleState Class 
//*【?】終了
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_IdleState::OnExit(GunWeapon* pOwner)
{
    pOwner->get_WeaponFlags().DisableFlag(WEAPON_STATUS::IDLE);
}

//*---------------------------------------------------------------------------------------
//* @:Gun_IdleState Class 
//*【?】更新
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
int Gun_IdleState::Update(GunWeapon* pOwner)
{
    auto player = Master::m_pGameObjectManager->get_ObjectByTag("Player");
    if (player->get_Component<PlayerController>()->get_AnimID() == PlayerData::PLAYER_RANGER_ANIM_ID::RUNING_DIVE_ROLL)return GUN_STATE::GUN_STATE_IDLE;



	// 弾を発射してないときはフラッシュライトをオフ
    if (pOwner->get_FlashPointLight().lock()) {
        auto flashLight = pOwner->get_FlashPointLight().lock();
        flashLight->set_Intensity(0.0f);
    }
    const auto& weapon_param = pOwner->get_GunWeaponParameter();

    int ammoRemaining = pOwner->get_AmmoRemaining();    // 残弾

	// 左クリックで発射 (LShiftが押されて無いとき)
	if (GetMouseClick(MOUSE_BUTTON_STATE::LEFT) && GetInput(GAME_CONFIG::MOVE_DASH) == false)
	{
		return GUN_STATE::GUN_STATE_FIRE;
	}
    // 残弾が0以下なら強制リロード
    else if (ammoRemaining <= 0)
    {
        return GUN_STATE::GUN_STATE_RELOADING;
    }
    // 対応キーが押されたらリロード
    else if (GetInputDown(GAME_CONFIG::WEAPON_RELOAD))
    {
        if (ammoRemaining < weapon_param->_bulletMaxNum)
        {
            return GUN_STATE::GUN_STATE_RELOADING;
        }
    }

	return GUN_STATE::GUN_STATE_IDLE;
}
