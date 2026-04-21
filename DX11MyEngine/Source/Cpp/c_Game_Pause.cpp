#include "pch.h"
#include "c_Game_Pause.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_SpriteRenderer.h"
#include "Component_ButtonUI.h"
#include "GameObjectManager.h"

using namespace SceneStateEnums;
using namespace UtilityData;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;

static const VEC2 g_PauseItemSize = VEC2(500.0f, 80.0f);		// 項目のサイズ

// 項目の位置（画面の中央を中心）
static const VEC2 g_PauseItemPosArray[UINT_CAST(PAUSE_ITEM::NUM)] =
{
	VEC2(670.0f, 440.0f),
	VEC2(670.0f, 540.0f),
	VEC2(670.0f, 640.0f),
};

static const VEC2 g_BackSpriteSize = VEC2(550.0f, 350.0f);		// 最奥背景スプライトをのサイズ
static const VEC2 g_BackSpritePos = VEC2(660.0f, 400.0f);		// 最奥背景スプライトをの位置


//*---------------------------------------------------------------------------------------
//* @:c_Game_Pause Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Pause::OnEnter(SceneManager* pOwner)
{
	// 連続入力防止のため、1フレーム止める
	Master::m_pInputManager->StopInput(1);

	Master::m_pDataManager->set_IsPause(true);

	// ボタンが押されたときに遷移する先のステート
	const c_GAME onClickNextStateArray[UINT_CAST(UtilityData::PAUSE_ITEM::NUM)] =
	{
		c_GAME::c_GAME_PLAY,			
		c_GAME::c_GAME_LOAD,
		c_GAME::c_GO_RESULT_SCENE,
	};

	m_NextState = c_GAME::c_GAME_PAUSE;

	// メニュー項目のオブジェクトとコンポーネントの取得と設定
	for (int i = 0; i < static_cast<int>(PAUSE_ITEM::NUM); i++)
	{
		UIData::RectTransformData rectTrans;
		UIData::ButtonUIData buttonData;
		buttonData._imagePath = "Resource/Texture/Title/Line.png";
		buttonData._text = g_PauseItemNames[i];
		buttonData._layerRank = 105;
		buttonData._inputValidationState = UIData::STATE::SELECTED;
		buttonData._onClicFunction = [this, i, onClickNextStateArray]() { m_NextState = onClickNextStateArray[i]; };
		buttonData._textOffsetPos = VEC2(100.0f, 0.0f);
		rectTrans._size = g_PauseItemSize;
		rectTrans._pos = g_PauseItemPosArray[i];
		m_pPauseItemBackButtonObjArray[i] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		auto button = m_pPauseItemBackButtonObjArray[i]->get_Component<ButtonUI>();
		button->set_Color(VEC4(10.0f, 10.0f, 0.0f, 1.0f), UIData::STATE::HIGH_LIGHTED);	// 選択されている状態で「黄色」に
		button->set_Color(VEC4(5.0f, 5.0f, 0.0f, 1.0f), UIData::STATE::PRESSED);
		button->set_Color(VEC4(10.0f, 10.0f, 0.0f, 1.0f), UIData::STATE::SELECTED);
		m_pPauseItemBackRectTransformArray[i] = m_pPauseItemBackButtonObjArray[i]->get_RectTransform();
	}

	//*****************************************************************************************
	//						最奥背景スプライト
	//*****************************************************************************************
	UIData::SpriteUIData backSpriteData;
	UIData::RectTransformData backRectTrans;
	backSpriteData._color = VEC4(0.0f, 0.0f, 0.0f, 0.85f);
	backSpriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;
	backSpriteData._layerRank = 100;
	backSpriteData._tag = "ConfigSceneBackSprite";
	backRectTrans._size = g_BackSpriteSize;
	backRectTrans._pos = g_BackSpritePos;
	m_pPauseBackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, backRectTrans, backSpriteData);
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Pause Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Pause::OnExit(SceneManager* pOwner)
{	
	Master::m_pDataManager->set_IsPause(false);

	m_pPauseBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	for (int i = 0; i < static_cast<int>(PAUSE_ITEM::NUM); i++)
	{
		m_pPauseItemBackButtonObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}

	// 連続入力防止のため、1フレーム止める
	Master::m_pInputManager->StopInput(1);
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Pause Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Game_Pause::Update(SceneManager* pOwner)
{
	float deltaTime = Master::m_pTimeManager->get_DeltaTime();

	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT) || GetInputDown(GAME_CONFIG::PAUSE))
	{
		return c_GAME::c_GAME_PLAY;
	}

	return m_NextState;
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Pause Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Pause::Draw(SceneManager* pOwner)
{
	float width = Master::m_pDataManager->get_ScreenWidth();
	float height = Master::m_pDataManager->get_ScreenHeight();

	Master::m_pDirectWriteManager->SetOutLine(3.0f, D2D1::ColorF(0.0f, 0.0f, 0.0f));
	Master::m_pDirectWriteManager->DrawStringToAligment("Ｐａｕｓｅ", VECTOR2::VEC2(0.0f, -150.0f), "White_40_STD", H_ALIGNMENT::CENTER, V_ALIGNMENT::CENTER, VEC2(width, height));
	Master::m_pDirectWriteManager->SetOutLine(0.0f);
}
