#include "pch.h"
#include "Root_ResultSceneState.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "Component_WeaponController.h"
#include "InputFactory.h"

using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;
using namespace SceneStateEnums;
using namespace Tool::UV;


//*---------------------------------------------------------------------------------------
//* @:TitleSceneState Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void Root_ResultSceneState::OnEnter(SceneManager *pOwner)
{
	this->SetInitChildState(pOwner, c_RESULT::c_GETITEM_CHECK);

	// ワイプ用スプライトを作る **********************************************
	UIData::RectTransformData rectData;
	UIData::SpriteUIData spriteData;
	rectData._size = VEC2(1280.0f, 256.0f);
	rectData._pivot = VEC2(0.5f, 0.5f);
	rectData._anchorMax = VEC2(0.5f, 0.5f);
	rectData._anchorMin = VEC2(0.5f, 0.5f);
	rectData._pos = VEC2(0.0f, 0.0f);
	spriteData._tag = "ClearedTextSprite";
	spriteData._imagePath = "Resource/Texture/UI/MissionCleared.png";
	spriteData._layerRank = 999;
	spriteData._color = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pClearedTextSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
}


//*---------------------------------------------------------------------------------------
//* @:TitleSceneState Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void Root_ResultSceneState::OnExit(SceneManager *pOwner)
{
	auto player = Master::m_pGameObjectManager->get_ObjectByTag("Player");
	if (player)
	{
		// プレイヤーの武器をクリアする
		player->get_Component<WeaponController>()->ClearWeapon();
		player->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}
	Master::m_pGameObjectManager->clear_NotIsDontDestroyObject();

	Master::m_pEffectManager->StopAllEffects();	// 全てのエフェクトを停止する

	// スプライトをプールへ返す
	m_pClearedTextSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
}


//*---------------------------------------------------------------------------------------
//* @:Root_ResultSceneState Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int Root_ResultSceneState::Update(SceneManager *pOwner)
{
	if (m_CrntChildStateID == -1)return -1;

	int newState = m_pChildStateMap[m_CrntChildStateID]->Update(pOwner);

	// タイトルシーンへ
	if (newState == c_RESULT::c_GO_TITLE_SCENE)
	{
		m_pChildStateMap[m_CrntChildStateID]->OnExit(pOwner);
		return SCENE_STATE::SCENE_STATE_TITLE;
	}

	// ステートが変更されていたら切り替え処理を行う
	// ※ c_GO_TITLE_SCENE以外
	if (newState != m_CrntChildStateID)
	{
		ChangeChildState(pOwner, newState);
	}

	return SCENE_STATE::SCENE_STATE_RESULT;
}


//*---------------------------------------------------------------------------------------
//* @:Root_ResultSceneState Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void Root_ResultSceneState::Draw(SceneManager *pOwner)
{
	if (m_CrntChildStateID == -1)return;

	m_pChildStateMap[m_CrntChildStateID]->Draw(pOwner);
}
