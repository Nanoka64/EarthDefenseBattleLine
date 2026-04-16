#include "pch.h"
#include "c_Game_Config.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_SpriteRenderer.h"
#include "GameObjectManager.h"

using namespace SceneStateEnums;
using namespace UtilityData;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;

constexpr int MAX_BGM_VOLUME = 100;			// BGM音量の最大値
constexpr int MIN_BGM_VOLUME = 0;			// BGM音量の最小値
constexpr int MAX_SE_VOLUME = 100;			// SE音量の最大値
constexpr int MIN_SE_VOLUME = 0;			// SE音量の最小値
constexpr float MAX_MOUSE_SENSITIVITY = 100.0f;	// マウス感度の最大値
constexpr float MIN_MOUSE_SENSITIVITY = 0.0f;	// マウス感度の最小値


// 項目の衝突判定用サイズ
static const VECTOR2::VEC2 g_ConfigItemSize = VECTOR2::VEC2(1000.0f, 70.0f);		// 項目のサイズ
static const VECTOR2::VEC2 g_ConfigButtonSize = VECTOR2::VEC2(100.0f, 70.0f);		// ボタンのサイズ

// 項目の値を表示するテキストの位置は、項目の位置にこのオフセットを足した位置になるようにする
static const VECTOR2::VEC2 g_ConfigValueOffsetSize = VECTOR2::VEC2(250.0f, 70.0f);

// 項目の位置
static const VECTOR2::VEC2 g_ConfigItemPosArray[UINT_CAST(CONFIG_ITEM::NUM)] =
{
	VEC2(400.0f,400.0f),
	VEC2(400.0f,500.0f),
	VEC2(400.0f,600.0f),
	VEC2(400.0f,700.0f),
	VEC2(400.0f,800.0f),
};

// ボタンの位置（項目名の右側に配置）
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
	"カメラの上下反転",
	"シャドウの有無",
};



using namespace SceneStateEnums;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;
//*---------------------------------------------------------------------------------------
//* @:c_Game_Config Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Config::OnEnter(SceneManager* pOwner)
{
	// 現在の設定情報を入れる
	const auto &config = Master::m_pDataManager->get_UserConfigData();
	m_DefaultConfigItemInfoArray[0] = ConfigMenuItemInfo({ 100.0f, 100.0f }, "", CONFIG_ITEM::BGM_VOLUME, VALUE_TYPE::INT, config._BGMVolume);
	m_DefaultConfigItemInfoArray[1] = ConfigMenuItemInfo({ 100.0f, 150.0f }, "", CONFIG_ITEM::SE_VOLUME, VALUE_TYPE::INT, config._SEVolume);
	m_DefaultConfigItemInfoArray[2] = ConfigMenuItemInfo({ 100.0f, 200.0f }, "", CONFIG_ITEM::MOUSE_SENSITIVITY, VALUE_TYPE::FLOAT, config._mouseSensitivity);
	m_DefaultConfigItemInfoArray[3] = ConfigMenuItemInfo({ 100.0f, 250.0f }, "", CONFIG_ITEM::INVERT_Y, VALUE_TYPE::BOOL, config._isInvertY);
	m_DefaultConfigItemInfoArray[4] = ConfigMenuItemInfo({ 100.0f, 300.0f }, "", CONFIG_ITEM::SHADOW_ENABLED, VALUE_TYPE::BOOL, config._isShadowEnabled);

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
		m_ItemInfoArray[i]._value = m_DefaultConfigItemInfoArray[i]._value;
		m_ItemInfoArray[i]._valueType = m_DefaultConfigItemInfoArray[i]._valueType;

		//*****************************************************************************************
		//						ボタン
		//*****************************************************************************************

		/* 左側 */
		UIData::ButtonUIData buttonData;
		buttonData._imagePath = "Resource/Texture/Title/TriangleCursor_L.png";
		buttonData._layerRank = 111;
		buttonData._repeatInputInterval = 5;	// ボタンを押し続けたときの連続入力の間隔
		buttonData._inputWaitFrame = 10;		// ボタンを押してから次の入力を受け付けるまでのフレーム数
		buttonData._tag = "AdjustmentButton" + std::to_string(i) + "_1";
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() {this->ChangeConfigValue(static_cast<CONFIG_ITEM>(i), true); };	// クリックされたときに設定値を増やす関数を呼ぶ
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos = g_ConfigItemPosArray[i];
		rectTrans._pos.x += g_Button_OffsetPos_X;	// 項目の右側に配置するようX座標をずらす
		m_pConfigAdjustmentButtonObj[i][0] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);

		/* 右側 */
		buttonData._imagePath = "Resource/Texture/Title/TriangleCursor_R.png";
		buttonData._tag = "AdjustmentButton" + std::to_string(i) + "_2";
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() {this->ChangeConfigValue(static_cast<CONFIG_ITEM>(i), false); };	// クリックされたときに設定値を増やす関数を呼ぶ
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos.x += g_RangeBetweenButtons;	// ボタン同士の横方向の距離を空ける
		m_pConfigAdjustmentButtonObj[i][1] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);


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
	backRectTrans._pos = VEC2(300.0f, 370.0f);
	m_pConfigBackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, backRectTrans, backSpriteData);
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Config Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Config::OnExit(SceneManager* pOwner)
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

		// ボタン 左右
		for (int j = 0; j < 2; j++) {
			if (m_pConfigAdjustmentButtonObj[i][j])
			{
				m_pConfigAdjustmentButtonObj[i][j]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
			}
		}
	}

	if (m_pConfigBackSpriteObj != nullptr) {
		m_pConfigBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Config Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Game_Config::Update(SceneManager* pOwner)
{
	float deltaTime = Master::m_pTimeManager->get_DeltaTime();

	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT) || GetInputDown(GAME_CONFIG::PAUSE))
	{
		return c_GAME::c_GAME_PLAY;
	}

	return c_GAME::c_GAME_CONFIG;
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Config Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Config::Draw(SceneManager* pOwner)
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

//*---------------------------------------------------------------------------------------
//*【?】値の変更
//*
//* [引数]
//* _item : 何の項目か
//*_isLeftIndex : 左ボタンからの入力か（trueなら左、falseなら右）
//* 
//* [返値] なし
//*----------------------------------------------------------------------------------------
void c_Game_Config::ChangeConfigValue(UtilityData::CONFIG_ITEM _item, bool _isLeftIndex)
{
	int index = INT_CAST(_item);
	if (index < 0 || index >= INT_CAST(CONFIG_ITEM::NUM))
	{
		return;
	}

	switch (_item)
	{
		//=========================================================================================
		//						BGM調整
		//=========================================================================================
	case UtilityData::CONFIG_ITEM::BGM_VOLUME:
	{
		int crntValue = std::get<int>(m_ItemInfoArray[index]._value);
		crntValue += (_isLeftIndex ? -1 : 1);
		crntValue = std::clamp(crntValue, MIN_BGM_VOLUME, MAX_BGM_VOLUME);	// クランプして範囲内に収める
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_BGMVolume(crntValue);
	}
	break;
	//=========================================================================================
	//						SE調整
	//=========================================================================================
	case UtilityData::CONFIG_ITEM::SE_VOLUME:
	{
		int crntValue = std::get<int>(m_ItemInfoArray[index]._value);
		crntValue += (_isLeftIndex ? -1 : 1);
		crntValue = std::clamp(crntValue, MIN_SE_VOLUME, MAX_SE_VOLUME);
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_SEVolume(crntValue);
	}
	break;
	//=========================================================================================
	//						マウス感度調整
	//=========================================================================================
	case UtilityData::CONFIG_ITEM::MOUSE_SENSITIVITY:
	{
		float crntValue = std::get<float>(m_ItemInfoArray[index]._value);
		crntValue += (_isLeftIndex ? -0.5f : 0.5f);
		crntValue = std::clamp(crntValue, MIN_MOUSE_SENSITIVITY, MAX_MOUSE_SENSITIVITY);
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_MouseSensitivity(crntValue);
	}
	break;
	//=========================================================================================
	//						Y反転の有無
	//=========================================================================================
	case UtilityData::CONFIG_ITEM::INVERT_Y:
	{
		bool crntValue = std::get<bool>(m_ItemInfoArray[index]._value);
		crntValue = (_isLeftIndex ? false : true);
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_IsInvertY(crntValue);
	}
	break;
	//=========================================================================================
	//						シャドウの有無
	//=========================================================================================
	case UtilityData::CONFIG_ITEM::SHADOW_ENABLED:
	{
		bool crntValue = std::get<bool>(m_ItemInfoArray[index]._value);
		crntValue = (_isLeftIndex ? false : true);
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_IsShadowEnabled(crntValue);
	}
	break;
	case UtilityData::CONFIG_ITEM::NUM:
		break;
	default:
		break;
	}
};