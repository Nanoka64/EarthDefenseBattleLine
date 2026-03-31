#include "pch.h"
#include "c_Title_MissionSelect.h"
#include "Root_TitleSceneState.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "Component_ButtonUI.h"
#include "InputFactory.h"
#include "SceneStateEnums.h"
#include "GameObject.h"

using namespace VECTOR2;
using namespace VECTOR3;

// メニュー項目の衝突判定用サイズ
static const VECTOR2::VEC2 g_MissionItemSize = VECTOR2::VEC2(400.0f, 70.0f);

// メニュー項目の位置
static const VECTOR2::VEC2 g_MissionItemPosArray[MISSION_NUM] =
{
	VEC2(400.0f,600.0f),
};

/// <summary>
/// ミッション名
/// </summary>
static constexpr const char* g_MissionNames[MISSION_NUM]=
{
	"1.巨大生物襲来",
};

using namespace SceneStateEnums;
//*---------------------------------------------------------------------------------------
//* @:c_Title_MissionSelect Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MissionSelect::OnEnter(SceneManager *pOwner)
{
	m_NextState = c_TITLE_MISSION_SELECT;

	// メニュー項目のオブジェクトとコンポーネントの取得と設定
	for (int i = 0; i < static_cast<int>(MISSION_NUM); i++)
	{
		UIData::RectTransformData rectTrans;
		UIData::ButtonUIData buttonData;
		buttonData._imagePath = "Resource/Texture/Title/Line.png";
		buttonData._text = g_MissionNames[i];
		buttonData._layerRank = 105;
		buttonData._inputValidationState = UIData::STATE::PRESSED;
		buttonData._onClicFunction = [this, i]() { m_NextState = c_TITLE::c_GO_GAME_SCENE; };
		buttonData._textOffsetPos = VEC2(100.0f, 0.0f);
		rectTrans._size = g_MissionItemSize;
		rectTrans._pos = g_MissionItemPosArray[i];
		m_pButtonsObjArray[i]		 = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		m_pButtonArray[i]				 = m_pButtonsObjArray[i]->get_Component<ButtonUI>();
		m_pMenuItemRectTransformArray[i] = m_pButtonsObjArray[i]->get_RectTransform();

		m_ItemInfoArray[i]._pos = g_MissionItemPosArray[i];
		m_ItemInfoArray[i]._name = g_MissionNames[i];
	}

	m_PrevHoveredMIssionItem = 0;

	m_IsInit = true;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MissionSelect Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MissionSelect::OnExit(SceneManager *pOwner)
{
	// オブジェクトを非アクティブに（プールに返す）
	for (int i = 0; i < MISSION_NUM; i++)
	{
		m_pButtonsObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MissionSelect Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Title_MissionSelect::Update(SceneManager *pOwner)
{
	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT))
	{
		return c_TITLE::c_TITLE_MAIN_MENU;
	}	

	//m_NextState = c_TITLE_MISSION_SELECT;

	int i = 0;
	for (auto &button : m_pButtonArray)
	{
		UIData::STATE state = button.lock()->get_State();;

		//m_MissionItems[i]._isHovered = false;

		switch (state)
		{
		case UIData::STATE::NORMAL:
			break;
		case UIData::STATE::HIGH_LIGHTED:
			m_ItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			break;
		case UIData::STATE::PRESSED:
			m_ItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			break;
		case UIData::STATE::SELECTED:
			m_ItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			break;
		case UIData::STATE::DISABLED:
			break;
		default:
			break;
		}

		i++;
	}

	return m_NextState;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MissionSelect Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MissionSelect::Draw(SceneManager *pOwner)
{
	for (auto& item : m_ItemInfoArray)
	{
		// 項目の位置
		VEC2 menuItemPos = item._pos;

		// スプライトの位置（文字の少し左側になるよう補正している）
		VEC2 spritePos;
		spritePos.x = menuItemPos.x - 100.0f;
		spritePos.y = menuItemPos.y;


		// マウスが乗ってるならずらす
		if (item._isHovered)
		{
			menuItemPos.x += 50.0f;
			spritePos.x += 50.0f;
			if (m_PrevHoveredMIssionItem)
			{
				// ****************************************************
				//				カーソルが載った時のSE再生
				// ****************************************************
				Master::m_pSoundManager->Play(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::SYSTEM_MOVING_CURSOR01));
			}

			m_PrevHoveredMIssionItem = 0;	// なんの項目に乗ったか保持

			menuItemPos.x += 50.0f;
			spritePos.x += 50.0f;
		}

		m_pMenuItemRectTransformArray[0].lock()->set_RectPosition(VEC2(spritePos.x, spritePos.y));
		m_pMenuItemRectTransformArray[0].lock()->set_Size(500.0f, 100.0f);

		// 文字表示
		//Master::m_pDirectWriteManager->DrawString(item._name, menuItemPos, "White_40_STD");
	}


	Master::m_pDirectWriteManager->DrawString("☆ミッション選択", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");
}
