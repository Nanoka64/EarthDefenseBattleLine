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


	// リロードバー用スプライトを作る **********************************************
	UIData::RectTransformData rectData;
	UIData::SpriteUIData spriteData;
	rectData._size = VEC2(0.0f, 0.0f);
	rectData._pos = RELOAD_BAR_POS;
	rectData._anchorMax = VEC2(0.5f, 0.5f);		// 中心を画面の真ん中に
	rectData._anchorMin = VEC2(0.5f, 0.5f);
	rectData._pivot = VEC2(0.0f, 0.0f);		// ピボットは左上
	spriteData._tag = "ReloadBar";
	spriteData._layerRank = 108;
	spriteData._color = VEC4(0.0f, 1.0f, 0.0f, 1.0f);
	spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;
	m_pReloadBarSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
	
	// リロードバー背景用スプライトを作る **********************************************
	rectData._size = RELOAD_BAR_BACK_SIZE;
	rectData._pos = RELOAD_BAR_BACK_POS;
	rectData._anchorMax = VEC2(0.5f, 0.5f);		// 中心を画面の真ん中に
	rectData._anchorMin = VEC2(0.5f, 0.5f);
	rectData._pivot = VEC2(0.5f, 0.5f);
	spriteData._tag = "ReloadBarBack";
	spriteData._layerRank = 107;
	spriteData._color = VEC4(0.1f, 0.1f, 0.1f, 1.0f);	// 黒背景
	spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;
	m_pReloadBarBackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
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

	// スプライトをプールへ返す
	m_pReloadBarSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	m_pReloadBarBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
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

	auto rect = m_pReloadBarSpriteObj->get_RectTransform().lock();
	rect->set_Size(RELOAD_BAR_SIZE.x * t, RELOAD_BAR_SIZE.y);	// 横幅を動かす

	// リロード終了
	if (m_ReloadElapsed >= weapon_param->_reloadTime)
	{
		pOwner->set_AmmoRemaining(weapon_param->_bulletMaxNum); // 弾込め

		return GUN_STATE::GUN_STATE_IDLE;
	}
	float width = Master::m_pDataManager->get_ScreenWidth();
	float height = Master::m_pDataManager->get_ScreenHeight();;
	Master::m_pDirectWriteManager->DrawStringToAligment("リロード中", RELOAD_TEXT_POS, "White_20_STD", H_ALIGNMENT::CENTER, V_ALIGNMENT::CENTER, VEC2(width, height));
	return GUN_STATE::GUN_STATE_RELOADING;
}
