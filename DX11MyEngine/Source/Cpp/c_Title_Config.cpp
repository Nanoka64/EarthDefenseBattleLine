#include "pch.h"
#include "c_Title_Config.h"
#include "Root_TitleSceneState.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_SpriteRenderer.h"
#include "Component_ButtonUI.h"

using namespace SceneStateEnums;
using namespace UtilityData;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;


static const VEC2 g_ConfigItemSize = VEC2(500.0f, 35.0f);		// 項目のサイズ
static const VEC2 g_ConfigButtonSize = VEC2(50.0f, 35.0f);		// ボタンのサイズ
static const VEC4 g_ConfigItemColor = VEC4(0.3f, 0.3f, 0.7f, 1.0f);

static const VEC2 g_BackSpriteSize = VEC2(700.0f, 500.0f);		// 最奥背景スプライトをのサイズ
static const VEC2 g_BackSpritePos = VEC2(550.0f, 500.0f);		// 最奥背景スプライトをの位置

// 項目の値を表示するテキストの位置は、項目の位置にこのオフセットを足した位置になるようにする
static const VEC2 g_ConfigValueOffsetSize = VEC2(250.0f, 70.0f);	


static const VEC2 g_ConfigItemStartPos = VEC2(650.0f, 550.0f);  // 項目の開始位置
static const float g_ConfigItemStepPosY = 70.0f;				// 項目と項目とのY座標間の距離


// ボタンの位置（項目名の右側に配置）
static constexpr float g_Button_OffsetPos_X = 350.0f;

// ボタン同士の間の距離
static constexpr float g_RangeBetweenButtons = 100.0f;

/// <summary>
/// 項目名
/// </summary>
static constexpr const char* g_ConfigItemNames[UINT_CAST(CONFIG_ITEM::NUM)] =
{
	"BGM音量",
	"SE音量",
	"マウス感度",
	"カメラの上下反転",
	"画面振動",
	"シャドウの有無",
};

///// <summary>
///// デフォルトの項目の情報（位置、名前、種類、値の種類、値）
///// </summary>
//static const ConfigMenuItemInfo g_DefaultConfigItemInfoArray[UINT_CAST(CONFIG_ITEM::NUM)] =
//{
//	ConfigMenuItemInfo({ 100.0f, 100.0f }, "", CONFIG_ITEM::BGM_VOLUME,			VALUE_TYPE::INT,   30),
//	ConfigMenuItemInfo({ 100.0f, 150.0f }, "", CONFIG_ITEM::SE_VOLUME,			VALUE_TYPE::INT,   50),
//	ConfigMenuItemInfo({ 100.0f, 200.0f }, "", CONFIG_ITEM::MOUSE_SENSITIVITY,	VALUE_TYPE::FLOAT, 40.0f),
//	ConfigMenuItemInfo({ 100.0f, 250.0f }, "", CONFIG_ITEM::INVERT_Y,			VALUE_TYPE::BOOL,  false),
//	ConfigMenuItemInfo({ 100.0f, 300.0f }, "", CONFIG_ITEM::SHADOW_ENABLED,     VALUE_TYPE::BOOL,  true)
//};


//*---------------------------------------------------------------------------------------
//* @:c_Title_Config Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_Config::OnEnter(SceneManager *pOwner)
{

	// 現在の設定情報を入れる
	const auto& config = Master::m_pDataManager->get_UserConfigData();
	m_DefaultConfigItemInfoArray[0] = ConfigMenuItemInfo({ 100.0f, 100.0f }, "", CONFIG_ITEM::BGM_VOLUME, VALUE_TYPE::INT, config._BGMVolume);
	m_DefaultConfigItemInfoArray[1] = ConfigMenuItemInfo({ 100.0f, 150.0f }, "", CONFIG_ITEM::SE_VOLUME, VALUE_TYPE::INT, config._SEVolume);
	m_DefaultConfigItemInfoArray[2] = ConfigMenuItemInfo({ 100.0f, 200.0f }, "", CONFIG_ITEM::MOUSE_SENSITIVITY, VALUE_TYPE::FLOAT, config._mouseSensitivity);
	m_DefaultConfigItemInfoArray[3] = ConfigMenuItemInfo({ 100.0f, 250.0f }, "", CONFIG_ITEM::INVERT_Y, VALUE_TYPE::BOOL, config._isInvertY);
	m_DefaultConfigItemInfoArray[4] = ConfigMenuItemInfo({ 100.0f, 300.0f }, "", CONFIG_ITEM::CAMERA_SHAKE, VALUE_TYPE::BOOL, config._isCameraShake);
	m_DefaultConfigItemInfoArray[5] = ConfigMenuItemInfo({ 100.0f, 350.0f }, "", CONFIG_ITEM::SHADOW_ENABLED, VALUE_TYPE::BOOL, config._isShadowEnabled);



	// 項目のオブジェクトとコンポーネントの取得と設定
	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		UIData::SpriteUIData spriteData;
		UIData::RectTransformData rectTrans;

		VEC2 itemPos = g_ConfigItemStartPos;
		itemPos.y += g_ConfigItemStepPosY * i;


		//*****************************************************************************************
		//						メニュー項目
		//*****************************************************************************************
		m_ItemInfoArray[i]._name = g_ConfigItemNames[i];
		m_ItemInfoArray[i]._pos = itemPos + VEC2(g_Button_OffsetPos_X - 50.0f, -15.0f);	// TODO:マジックナンバー（項目の値の位置がそのまま出はずれてしまうため）
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
		buttonData._inputSoundID = SOUND_ID_TO_INT(SOUND_ID::SYSTEM_MOVING_CURSOR01);
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos = itemPos;
		rectTrans._pos.x += g_Button_OffsetPos_X;	// 項目の右側に配置するようX座標をずらす
		m_pConfigAdjustmentButtonObj[i][0] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		auto leftButton = m_pConfigAdjustmentButtonObj[i][0]->get_Component<ButtonUI>();
		leftButton->set_Color(VEC4(0.0f), UIData::STATE::DISABLED);		// 無効の際は表示しない
		leftButton->set_IsInteractable(false);

		/* 右側 */
		buttonData._imagePath = "Resource/Texture/Title/TriangleCursor_R.png";
		buttonData._tag = "AdjustmentButton" + std::to_string(i) + "_2";
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() {this->ChangeConfigValue(static_cast<CONFIG_ITEM>(i), false); };	// クリックされたときに設定値を増やす関数を呼ぶ
		rectTrans._size = g_ConfigButtonSize;
		rectTrans._pos.x += g_RangeBetweenButtons;	// ボタン同士の横方向の距離を空ける
		m_pConfigAdjustmentButtonObj[i][1] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		auto rightButton = m_pConfigAdjustmentButtonObj[i][1]->get_Component<ButtonUI>();
		rightButton->set_Color(VEC4(0.0f), UIData::STATE::DISABLED);	// 無効の際は表示しない
		rightButton->set_IsInteractable(false);

		
		//m_pButtonArray[i] = m_pConfigAdjustmentButtonObj[i]->get_Component<ButtonUI>();
		//m_pMenuItemRectTransformArray[i] = m_pConfigAdjustmentButtonObj[i]->get_RectTransform();
	}

	// 最初は一番上の項目のみ、有効状態
	m_pConfigAdjustmentButtonObj[0][0]->get_Component<ButtonUI>()->set_IsInteractable(true);
	m_pConfigAdjustmentButtonObj[0][1]->get_Component<ButtonUI>()->set_IsInteractable(true);

	//*****************************************************************************************
	//						選択した項目をハイライトするスプライト
	//*****************************************************************************************
	UIData::SpriteUIData selectItemSpriteData;
	UIData::RectTransformData selectItemRectTrans;
	selectItemSpriteData._imagePath = "Resource/Texture/Title/Decoration01.png";
	selectItemSpriteData._tag = "ConfigMenuBackSprite";
	selectItemSpriteData._color = VEC4(1.0f, 10.0f, 10.0f, 1.0f);
	//selectItemSpriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;
	selectItemSpriteData._layerRank = 110;
	selectItemRectTrans._size = g_ConfigItemSize;
	selectItemRectTrans._pos = g_ConfigItemStartPos;
	m_pConfigSelectItemSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, selectItemRectTrans, selectItemSpriteData);


	//*****************************************************************************************
	//						最奥背景スプライト
	//*****************************************************************************************
	UIData::SpriteUIData backSpriteData;
	UIData::RectTransformData backRectTrans;
	backSpriteData._imagePath = "Resource/Texture/Title/ConfigBackGround.png";
	backSpriteData._layerRank = 100;
	backSpriteData._tag = "ConfigSceneBackSprite";
	backRectTrans._size = g_BackSpriteSize;
	backRectTrans._pos = g_BackSpritePos;
	m_pConfigBackSpriteObj= Master::m_pUIManager->GetSprite(*m_pRenderer, backRectTrans, backSpriteData);


	m_Timer = 0.0f;
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
	// 	// オブジェクトを非アクティブに（プールに返す）
	// *****************************************************************************************
	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		// ボタン 左右
		for (int j = 0; j < 2; j++) {
			if (m_pConfigAdjustmentButtonObj[i][j])
			{
				m_pConfigAdjustmentButtonObj[i][j]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
			}
		}
	}
	if (m_pConfigSelectItemSpriteObj != nullptr) {
		m_pConfigSelectItemSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
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
int c_Title_Config::Update(SceneManager* pOwner)
{
	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT))
	{
		return c_TITLE::c_TITLE_MAIN_MENU;
	}

	float deltaTime = Master::m_pTimeManager->get_DeltaTime();
	m_Timer += deltaTime;


	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		VEC2 itemPos = g_ConfigItemStartPos;
		itemPos.y += g_ConfigItemStepPosY * i;

		POINT mousePos = Master::m_pInputManager->GetMousePos();	// マウス座標

		// スプライトの衝突判定情報
		CollInData2D_AABB colData;
		colData._min = itemPos;
		colData._max = itemPos + g_ConfigItemSize;

		auto selectItemRect = m_pConfigSelectItemSpriteObj->get_RectTransform().lock();
		auto selectItemSpriteComp = m_pConfigSelectItemSpriteObj->get_Component<SpriteRenderer>();
		selectItemSpriteComp->set_UVOffset(VEC2(0.0f,m_Timer ));

		float t = sinf(m_Timer * 5.0f) * 0.5f + 0.5f;
		float ease = Tool::Easing::EaseOutSin(t);
		selectItemSpriteComp->set_Color(VEC4(1.0f * ease, 10.0f, 10.0f, 1.0f));


		// マウスとボタンの判定
		if (Master::m_pCollisionManager->HitCheck2D_BoxVsPoint(colData, VEC2(FLOAT_CAST(mousePos.x), FLOAT_CAST(mousePos.y))))
		{
			// 衝突時、黄色くする
			//spriteComp->set_Color(VEC4(1.0f, 1.0f, 1.0f, 1.0f));

			selectItemRect->set_RectPosition(itemPos);


			m_pConfigAdjustmentButtonObj[i][0]->get_Component<ButtonUI>()->set_IsInteractable(true);
			m_pConfigAdjustmentButtonObj[i][1]->get_Component<ButtonUI>()->set_IsInteractable(true);
		}
		else
		{
			m_pConfigAdjustmentButtonObj[i][0]->get_Component<ButtonUI>()->set_IsInteractable(false);
			m_pConfigAdjustmentButtonObj[i][1]->get_Component<ButtonUI>()->set_IsInteractable(false);
		}
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

	for (int i = 0; i < INT_CAST(CONFIG_ITEM::NUM); i++)
	{
		VEC2 itemPos = g_ConfigItemStartPos;
		itemPos.y += g_ConfigItemStepPosY * i;

		// 項目名
		Master::m_pDirectWriteManager->SetOutLine(2.0f,D2D1::ColorF(D2D1::ColorF::Black));	 // 縁取り
		Master::m_pDirectWriteManager->SetColor(D2D1::ColorF(D2D1::ColorF::White));			 // 白
		Master::m_pDirectWriteManager->DrawStringToAligment(g_ConfigItemNames[i], itemPos, "White_20_STD", H_ALIGNMENT::LEADING, V_ALIGNMENT::CENTER, g_ConfigItemSize);

		// 項目の値
		Master::m_pDirectWriteManager->SetColor(D2D1::ColorF(D2D1::ColorF::Aqua));	// 水色
		Master::m_pDirectWriteManager->DrawStringToAligment(m_ItemInfoArray[i].ConvertValueAsString(), m_ItemInfoArray[i]._pos, "White_20_STD", H_ALIGNMENT::CENTER, V_ALIGNMENT::CENTER, g_ConfigValueOffsetSize);
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
void c_Title_Config::ChangeConfigValue(UtilityData::CONFIG_ITEM _item, bool _isLeftIndex)
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
	//						カメラシェイクの有無
	//=========================================================================================
	case UtilityData::CONFIG_ITEM::CAMERA_SHAKE:
	{
		bool crntValue = std::get<bool>(m_ItemInfoArray[index]._value);
		crntValue = (_isLeftIndex ? false : true);
		m_ItemInfoArray[index].SetValue(crntValue);
		Master::m_pDataManager->set_IsCameraShake(crntValue);
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