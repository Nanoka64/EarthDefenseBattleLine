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

	// すでに初期化済みなら返す
	if (m_IsInit)
	{
		// 背景画像をアクティブに
		for (int i = 0; i < MISSION_NUM; i++)
		{
			auto obj = m_pMenuItem_RectTransform[i]->get_OwnerObj();
			if (obj.expired())
			{
				assert(false);
				continue;
			}
			obj.lock()->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

			auto button = m_pButtons[i].lock();

			// ボタンにテキスト設定
			button->set_Text(g_MissionNames[i]);

			// 処理設定
			button->OnClickFunc([this, i]() {m_NextState = c_TITLE::c_GO_GAME_SCENE; });

		}
		return;
	}

	m_PrevHoveredMIssionItem = 0;

	// アクティブにしてトランスフォームを取得
	for (int i = 0; i < MISSION_NUM; i++)
	{
		auto obj = Master::m_pGameObjectManager->get_ObjectByTag("MenuItem_Button_" + std::to_string(i + 1));
		if (obj)
		{
			obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
			m_pMenuItem_RectTransform[i] = obj->get_RectTransform().lock();

			// ボタンコンポーネント
			auto button = obj->get_Component<ButtonUI>();
			button->set_Text(g_MissionNames[i]);

			// シーンを遷移させる処理
			button->OnClickFunc([this, i]() {m_NextState = c_TITLE::c_GO_GAME_SCENE; });

			m_pButtons[i] = button;
		}

		m_MissionItems[i]._pos = g_MissionItemPosArray[i];
		m_MissionItems[i]._name = g_MissionNames[i];
	}
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
	// オブジェクトを非アクティブに
	for (int i = 0; i < MISSION_NUM; i++)
	{
		auto obj = m_pMenuItem_RectTransform[i]->get_OwnerObj();
		if (obj.expired())
		{
			assert(false);
			continue;
		}
		obj.lock()->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
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
	for (auto &button : m_pButtons)
	{
		UIData::STATE state = button.lock()->get_State();;

		//m_MissionItems[i]._isHovered = false;

		switch (state)
		{
		case UIData::STATE::NORMAL:
			break;
		case UIData::STATE::HIGH_LIGHTED:
			m_MissionItems[i]._isHovered = true;	// マウスが乗ってる
			break;
		case UIData::STATE::PRESSED:
			m_MissionItems[i]._isHovered = true;	// マウスが乗ってる
			break;
		case UIData::STATE::SELECTED:
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
	for (auto& item : m_MissionItems)
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

		m_pMenuItem_RectTransform[0]->set_RectPosition(VEC2(spritePos.x, spritePos.y));
		m_pMenuItem_RectTransform[0]->set_Size(500.0f, 100.0f);

		// 文字表示
		//Master::m_pDirectWriteManager->DrawString(item._name, menuItemPos, "White_40_STD");
	}


	Master::m_pDirectWriteManager->DrawString("☆ミッション選択", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");
}
