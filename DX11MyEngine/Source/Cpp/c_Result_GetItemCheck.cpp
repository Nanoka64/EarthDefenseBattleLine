#include "pch.h"
#include "ResultScene_StateHeader.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_SpriteRenderer.h"

using namespace SceneStateEnums;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;
//*---------------------------------------------------------------------------------------
//* @:c_Result_GetItemCheck Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Result_GetItemCheck::OnEnter(SceneManager *pOwner)
{
	m_ElapsedTime = 0.0f;


	// ワイプ用スプライトを作る **********************************************
	UIData::RectTransformData rectData;
	rectData._size = VEC2(0.0f, 0.0f);
	rectData._pivot = VEC2(0.5f, 0.5f);
	rectData._anchorMax = VEC2(0.5f, 0.5f);
	rectData._anchorMin = VEC2(0.5f, 0.5f);
	UIData::SpriteUIData spriteData;
	spriteData._tag = "WipeSprite";
	spriteData._imagePath = "Resource/Texture/Particle/Particle04_bokashi_hard.png";
	spriteData._layerRank = 998;
	spriteData._color = VEC4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pWipeSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
	m_pWipeSpriteRenderer = m_pWipeSpriteObj->get_Component<SpriteRenderer>();
}


//*---------------------------------------------------------------------------------------
//* @:c_Result_GetItemCheck Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Result_GetItemCheck::OnExit(SceneManager *pOwner)
{
	m_pWipeSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
}


//*---------------------------------------------------------------------------------------
//* @:c_Result_GetItemCheck Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Result_GetItemCheck::Update(SceneManager *pOwner)
{
	float deltaTime = Master::m_pTimeManager->get_DeltaTime();
	m_ElapsedTime += deltaTime;

	float t = m_ElapsedTime / WIPE_DURATION;
	float ease = Tool::Easing::EaseInCubic(t);
	auto spriteTrans = m_pWipeSpriteObj->get_RectTransform().lock();
	VEC2 crntSize = spriteTrans->get_SizeDelta();
	crntSize = VEC2::Lerp(crntSize, VEC2(5000.0f, 5000.0f), ease);
	spriteTrans->set_Size(crntSize);

	if (t > 1.0f)
	{
		return c_RESULT::c_GO_TITLE_SCENE;
	}

	// タイトルシーンへ
	if (GetInputHold(GAME_CONFIG::MOVE_JUMP, 60))
	{
		return c_RESULT::c_GO_TITLE_SCENE;
	}

	return c_RESULT::c_GETITEM_CHECK;
}


//*---------------------------------------------------------------------------------------
//* @:c_Result_GetItemCheck Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Result_GetItemCheck::Draw(SceneManager *pOwner)
{
	//Master::m_pDirectWriteManager->DrawString("クリアおめでとう！", VECTOR2::VEC2(740, 540), "White_50_STD");
	//Master::m_pDirectWriteManager->DrawString("Spaceを1秒以上長押しでタイトルに戻ります", VECTOR2::VEC2(540, 840), "White_40_STD");
}
