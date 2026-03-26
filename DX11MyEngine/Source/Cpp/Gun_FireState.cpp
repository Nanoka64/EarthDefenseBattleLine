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
}

//*---------------------------------------------------------------------------------------
//* @:Gun_FireState Class 
//*【?】終了
//* 引数：1.GunWeapon
//* 返値：void
//*----------------------------------------------------------------------------------------
void Gun_FireState::OnExit(GunWeapon* pOwner)
{

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

    auto player = Master::m_pGameObjectManager->get_ObjectByTag("Player");
    if (player->get_Component<PlayerController>()->get_AnimID() == PLAYER_ANIMATION_ID::ROLL)return GUN_STATE::GUN_STATE_IDLE;



	auto& weapon_param = pOwner->get_WeaponParameter();
	int ammoRemaining = pOwner->get_AmmoRemaining();
    
    auto flashLight = pOwner->get_FlashPointLight().lock();

    float c_AngleH = m_pRenderer->get_CameraComponent()->get_Angle_H();
    float c_AngleV = m_pRenderer->get_CameraComponent()->get_Angle_V();

    auto transform = pOwner->get_OwnerObj().lock()->get_Transform().lock();
    VEC3 pos = transform->get_WorldVEC3ToPos();

	// 左クリックで発射
	if (GetMouseClickHoldRepeat(MOUSE_BUTTON_STATE::LEFT, weapon_param._fireRate, weapon_param._fireRate))
	{
		// ****************************************************
		//				 発射音再生
		// ****************************************************
		Master::m_pSoundManager->Play_RandPitch(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::GUN_FIRE02), 300);

        // 同時発射
        for (int i = 0; i < weapon_param._bulletSimultaneousNum; i++)
        {
            // 親の向き等を参照
            VEC3 rad;
            rad.x = (c_AngleV) * -1;
            rad.y = (c_AngleH - 1.57f) * -1;
            rad.z = 0.0f;
            float accuracy = weapon_param._accuracy;

            // 弾のバラつき
            rad.x += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
            rad.y += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
            rad.z += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);

            // トランスフォームパラメータ
            BulletTransformData bulletTransform;
            bulletTransform._pos = pos;
            bulletTransform._rotRad = rad;
            bulletTransform._scale = VEC3(0.01f, 0.01f, 0.01f);

            BULLET_TYPE type = weapon_param._bulletType;

            // 弾データを共用体で持っているので、弾タイプにあったパラメータを入れるようにする
            std::visit([&](auto& param) {
                Master::m_pBulletManager->Shot(*m_pRenderer, bulletTransform, param);
                }, weapon_param._bulletParam);
        }

        // フラッシュ
        flashLight->set_Range(30.0f);
        flashLight->set_Intensity(5.5f);
        flashLight->set_LightColor(VEC3(1.0f, 1.0f, 1.0f));

        // 弾数減らす
        ammoRemaining--;
        pOwner->set_AmmoRemaining(ammoRemaining);


	}
	// リロード
	else if (GetInputDown(GAME_CONFIG::WEAPON_RELOAD) || ammoRemaining <= 0)
	{
		return GUN_STATE::GUN_STATE_RELOADING;
	}
	// 何もなし
	else
	{
		return GUN_STATE::GUN_STATE_IDLE;
	}

	return GUN_STATE::GUN_STATE_FIRE;
}
