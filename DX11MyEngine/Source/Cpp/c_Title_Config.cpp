#include "pch.h"
#include "c_Title_Config.h"
#include "Root_TitleSceneState.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"

using namespace SceneStateEnums;
using namespace UtilityData;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;

// 項目の衝突判定用サイズ
static const VECTOR2::VEC2 g_ConfigItemSize = VECTOR2::VEC2(1000.0f, 70.0f);		// 項目のサイズ
static const VECTOR2::VEC2 g_ConfigButtonSize = VECTOR2::VEC2(100.0f, 70.0f);		// ボタンのサイズ

// 項目の値を表示するテキストの位置は、項目の位置にこのオフセットを足した位置になるようにする
static const VECTOR2::VEC2 g_ConfigValueOffsetSize = VECTOR2::VEC2(250.0f, 70.0f);	

// 項目の位置
static const VECTOR2::VEC2 g_ConfigItemPosArray[UINT_CAST(CONFIG_ITEM::NUM)] =
{
	VEC2(400.0f,500.0f),
	VEC2(400.0f,600.0f),
	VEC2(400.0f,700.0f),
	VEC2(400.0f,800.0f),
	VEC2(400.0f,900.0f),
};

// ボタンの位置（項目の右側に配置）
static const float g_Button_OffsetPos_X = 600.0f;

// ボタン同士の間の距離
static const float g_RangeBetweenButtons = 300.0f;

/// <summary>
/// 項目名
/// </summary>
static constexpr const char* g_ConfigItemNames[UINT_CAST(CONFIG_ITEM::NUM)] =
{
	"BGM音量",
	"SE音量",
	"マウス感度",
	"カメラのY反転",
	"シャドウの有無",
};

/// <summary>
/// デフォルトの項目の情報（位置、名前、種類、値の種類、値）
/// </summary>
static const ConfigMenuItemInfo g_DefaultConfigItemInfoArray[UINT_CAST(CONFIG_ITEM::NUM)] =
{
	ConfigMenuItemInfo({ 100.0f, 100.0f }, "BGM Volume",      CONFIG_ITEM::BGM_VOLUME,			VALUE_TYPE::INT,   30),
	ConfigMenuItemInfo({ 100.0f, 150.0f }, "SE Volume",       CONFIG_ITEM::SE_VOLUME,			VALUE_TYPE::INT,   50),
	ConfigMenuItemInfo({ 100.0f, 200.0f }, "Mouse Sensitivity",CONFIG_ITEM::MOUSE_SENSITIVITY,	VALUE_TYPE::FLOAT, 40.0f),
	ConfigMenuItemInfo({ 100.0f, 250.0f }, "Invert Y Axis",   CONFIG_ITEM::INVERT_Y,			VALUE_TYPE::BOOL,  false),
	ConfigMenuItemInfo({ 100.0f, 300.0f }, "Shadow Enable",   CONFIG_ITEM::SHADOW_ENABLED,      VALUE_TYPE::BOOL,  true)
};


//*---------------------------------------------------------------------------------------
//* @:c_Title_Config Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_Config::OnEnter(SceneManager *pOwner)
{
	// 項目のオブジェクトとコンポーネントの取得と設定
	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		//*****************************************************************************************
		//						項目の背景スプライト
		//*****************************************************************************************
		UIData::SpriteUIData spriteData;
		UIData::RectTransformData rectTrans;
		spriteData._tag = "ConfigMenuBackSprite";
		spriteData._color = VEC4(0.3f, 0.3f, 0.7f, 1.0f);
		spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;
		spriteData._layerRank = 110;
		rectTrans._size = g_ConfigItemSize;
		rectTrans._pos = g_ConfigItemPosArray[i];
		m_pConfigMenuSpriteObjArray[i] = Master::m_pUIManager->GetSprite(*m_pRenderer, rectTrans, spriteData);

		//*****************************************************************************************
		//						メニュー項目
		//*****************************************************************************************
		m_ItemInfoArray[i]._name = g_ConfigItemNames[i];
		m_ItemInfoArray[i]._pos = g_ConfigItemPosArray[i] + VEC2(g_Button_OffsetPos_X + 70.0f, 0.0f);
		m_ItemInfoArray[i]._type = static_cast<CONFIG_ITEM>(i);
		m_ItemInfoArray[i]._value = g_DefaultConfigItemInfoArray[i]._value;
		m_ItemInfoArray[i]._valueType = g_DefaultConfigItemInfoArray[i]._valueType;

		//*****************************************************************************************
		//						ボタン
		//*****************************************************************************************
		int buttonIndex = i * 2;	// ボタンのインデックス（1項目につき左右2つあるのでiに2を掛ける）

		/* 左側 */
		UIData::ButtonUIData buttonData;
		buttonData._imagePath = "Resource/Texture/Title/TriangleCursor_L.png";
		buttonData._layerRank = 111;
		buttonData._tag = "AdjustmentButton" + std::to_string(i) + "_1";
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() {; };
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos = g_ConfigItemPosArray[i];
		rectTrans._pos.x += g_Button_OffsetPos_X;	// 項目の右側に配置するようX座標をずらす
		m_pConfigAdjustmentButtonObj[buttonIndex] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);

		/* 右側 */
		buttonData._imagePath = "Resource/Texture/Title/TriangleCursor_R.png";
		buttonData._tag = "AdjustmentButton" + std::to_string(i) + "_2";
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() {; };
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos.x += g_RangeBetweenButtons;	// ボタン同士の横方向の距離を空ける
		m_pConfigAdjustmentButtonObj[buttonIndex + 1] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		
		
		//m_pButtonArray[i] = m_pConfigAdjustmentButtonObj[i]->get_Component<ButtonUI>();
		//m_pMenuItemRectTransformArray[i] = m_pConfigAdjustmentButtonObj[i]->get_RectTransform();
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
	backRectTrans._size = VEC2(1200.0f, 550.0f);
	backRectTrans._pos = VEC2(300.0f, 470.0f);
	m_pConfigBackSpriteObj= Master::m_pUIManager->GetSprite(*m_pRenderer, backRectTrans, backSpriteData);
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_Config Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_Config::OnExit(SceneManager* pOwner)
{
	// *****************************************************************************************
	// プールに返す
	// *****************************************************************************************
	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		// 拝啓スプライト
		if (m_pConfigMenuSpriteObjArray[i])
		{
			m_pConfigMenuSpriteObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
		}	
		
		int buttonIndex = i * 2;	// ボタンのインデックス（1項目につき左右2つあるのでiに2を掛ける）

		// ボタン
		if (m_pConfigAdjustmentButtonObj[buttonIndex])
		{
			m_pConfigAdjustmentButtonObj[buttonIndex]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
		}
		if (m_pConfigAdjustmentButtonObj[buttonIndex + 1])
		{
			m_pConfigAdjustmentButtonObj[buttonIndex + 1]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
		}
	}

	if (m_pConfigBackSpriteObj != nullptr) {
		m_pConfigBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_Config Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Title_Config::Update(SceneManager *pOwner)
{
	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT))
	{
		return c_TITLE::c_TITLE_MAIN_MENU;
	} 

	return c_TITLE::c_TITLE_CONFIG;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_Config Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_Config::Draw(SceneManager* pOwner)
{
	Master::m_pDirectWriteManager->DrawString("☆設定", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");

	// 文字を水色に
	Master::m_pDirectWriteManager->SetColor(D2D1::ColorF(D2D1::ColorF::Aqua));
	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		// 項目名
		Master::m_pDirectWriteManager->DrawStringToAligment(g_ConfigItemNames[i], g_ConfigItemPosArray[i], "White_30_STD", H_ALIGNMENT::LEADING, V_ALIGNMENT::CENTER, g_ConfigItemSize);

		// 項目の値
		Master::m_pDirectWriteManager->DrawStringToAligment(m_ItemInfoArray[i].ConvertValueAsString(), m_ItemInfoArray[i]._pos, "White_30_STD", H_ALIGNMENT::CENTER, V_ALIGNMENT::CENTER, g_ConfigValueOffsetSize);
	}
	Master::m_pDirectWriteManager->SetColor(D2D1::ColorF(D2D1::ColorF::White));
}
