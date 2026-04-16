#include "pch.h"
#include "c_Result_Failed.h"
#include "c_Result_Failed.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_SpriteRenderer.h"

using namespace SceneStateEnums;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;
//*---------------------------------------------------------------------------------------
//* @:c_Result_Failed Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Result_Failed::OnEnter(SceneManager* pOwner)
{
	m_ElapsedTime = 0.0f;

	// 失敗テキスト用スプライトを作る **********************************************
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
	spriteData._color = VEC4(1.0f, 1.0f, 1.0f, 0.0f);
	m_pFailedTextSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
	m_pFailedTextSpriteRenderer = m_pFailedTextSpriteObj->get_Component<SpriteRenderer>();

	// ワイプ用スプライトを作る **********************************************
	rectData._size = VEC2(0.0f, 0.0f);
	rectData._pivot = VEC2(0.5f, 0.5f);
	rectData._anchorMax = VEC2(0.5f, 0.5f);
	rectData._anchorMin = VEC2(0.5f, 0.5f);
	spriteData._tag = "WipeSprite";
	spriteData._imagePath = "Resource/Texture/Particle/Particle04_bokashi_hard.png";
	spriteData._layerRank = 1000;
	spriteData._color = VEC4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pFailedWipeSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
	m_pFailedWipeSpriteRenderer = m_pFailedWipeSpriteObj->get_Component<SpriteRenderer>();
}


//*---------------------------------------------------------------------------------------
//* @:c_Result_Failed Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Result_Failed::OnExit(SceneManager* pOwner)
{
	// スプライトをプールへ返す
	m_pFailedWipeSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	m_pFailedTextSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
}


//*---------------------------------------------------------------------------------------
//* @:c_Result_Failed Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Result_Failed::Update(SceneManager* pOwner)
{
	float deltaTime = Master::m_pTimeManager->get_DeltaTime();
	m_ElapsedTime += deltaTime;

	// 失敗テキストのフェード
	float text_t = m_ElapsedTime / TEXT_FADE_IN_DURATION;
	if (text_t <= 1.0f)
	{
		auto textSprite = m_pFailedTextSpriteRenderer.lock();
		textSprite->set_Color(VEC4(1.0f, 1.0f, 1.0f, text_t));
	}
	else
	{
		// ワイプのフェードイン
		if (FADE_OUT_START_DELAY < (m_ElapsedTime - TEXT_FADE_IN_DURATION))
		{
			// テキストとワイプのフェードに入るまでの時間の差分を打ち消す
			float wipeElapsed = m_ElapsedTime - FADE_OUT_START_DELAY - TEXT_FADE_IN_DURATION;

			// フェードイン
			float wipe_t = wipeElapsed / FADE_DURATION;
			if (wipe_t <= 1.0f)
			{
				// ワイプが徐々に大きくなる
				float ease = Tool::Easing::EaseInCubic(wipe_t);
				auto spriteTrans = m_pFailedWipeSpriteObj->get_RectTransform().lock();
				VEC2 crntSize = spriteTrans->get_SizeDelta();
				crntSize = VEC2::Lerp(crntSize, VEC2(5000.0f, 5000.0f), ease);
				spriteTrans->set_Size(crntSize);
			}
			//=========================================================================================
			//
			//						フェードが終了したら、タイトルへ
			//
			//=========================================================================================
			else
			{
				return c_RESULT::c_GO_TITLE_SCENE;
			}
		}
	}

	return c_RESULT::c_FAILED;
}
