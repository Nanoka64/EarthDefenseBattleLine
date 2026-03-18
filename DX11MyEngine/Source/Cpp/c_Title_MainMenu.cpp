#include "pch.h"
#include "c_Title_MainMenu.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "Component_SpriteRenderer.h"
#include "Component_Button.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "InputFactory.h"

using namespace SceneStateEnums;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;

// メニュー項目の衝突判定用サイズ
static const VECTOR2::VEC2 g_MenuItemSize = VECTOR2::VEC2(400.0f, 90.0f);

// メニュー項目の位置
static const VECTOR2::VEC2 g_MenuItemPosArray[static_cast<int>(TITLEMENU_ITEM::NUM)] =
{
	VEC2(400.0f,600.0f),
	VEC2(400.0f,700.0f),
	VEC2(400.0f,800.0f),
	VEC2(400.0f,900.0f),
};

/// <summary>
/// タイトル項目名
/// </summary>
static constexpr const char* g_TitleMenuItemNames[static_cast<int>(TITLEMENU_ITEM::NUM)] =
{
	"ミッション選択",
	"兵科選択",
	"設定",
	"終了",
};

//*---------------------------------------------------------------------------------------
//* @:c_Title_MainMenu Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MainMenu::OnEnter(SceneManager* pOwner)
{
	// ****************************************************
	//				タイトルBGMの再生
	// ****************************************************
	Master::m_pSoundManager->PlayBGM(BGM_ID::BGM_TITLE_01);

	m_NextState = c_TITLE_MAIN_MENU;

	// すでに初期化済みなら項目背景画像のみアクティブにして返す
	if (m_IsInit)
	{
		// 背景画像をアクティブに
		for (int i = 0; i < static_cast<int>(TITLEMENU_ITEM::NUM); i++)
		{
			auto obj = m_pMenuItem_RectTransform[i].lock()->get_OwnerObj();
			if (obj.expired())
			{
				assert(false);
				continue;
			}
			obj.lock()->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

			auto button = m_pButtons[i].lock();

			// ボタンにテキスト設定
			button->set_Text(g_TitleMenuItemNames[i]);

			// 処理設定
			button->OnClickFunc([this, i]() {m_NextState = m_MenuItemInfoArray[i]._nextState; });
		}

		return;
	}

	m_PrevHoveredMenuItem = TITLEMENU_ITEM::EXIT;


	// 遷移先シーンの設定
	m_MenuItemInfoArray[0]._nextState = c_TITLE::c_TITLE_MISSION_SELECT;
	m_MenuItemInfoArray[1]._nextState = c_TITLE::c_TITLE_SOLDIER_SELECT;
	m_MenuItemInfoArray[2]._nextState = c_TITLE::c_TITLE_CONFIG;
	m_MenuItemInfoArray[3]._nextState = c_TITLE::c_GO_EXIT;


	// アクティブにしてトランスフォームを取得
	for (int i = 0; i < static_cast<int>(TITLEMENU_ITEM::NUM); i++)
	{
		auto obj = Master::m_pGameObjectManager->get_ObjectByTag("MenuItem_Button_" + std::to_string(i + 1));
		if (obj)
		{
			obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
			m_pMenuItem_RectTransform[i] = obj->get_RectTransform();

			// ボタンコンポーネント
			auto button = obj->get_Component<Button>();
			button->set_Text(g_TitleMenuItemNames[i]);

			// シーンを遷移させる処理
			button->OnClickFunc([this, i]() {m_NextState = m_MenuItemInfoArray[i]._nextState; });	

			m_pButtons[i] = button;
		}

		m_MenuItemInfoArray[i]._pos = g_MenuItemPosArray[i];
		m_MenuItemInfoArray[i]._name = g_TitleMenuItemNames[i];
		m_MenuItemInfoArray[i]._type = static_cast<TITLEMENU_ITEM>(i);
	}

	m_IsInit = true;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MainMenu Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MainMenu::OnExit(SceneManager* pOwner)
{
	// オブジェクトを非アクティブに
	for (int i = 0; i < static_cast<int>(TITLEMENU_ITEM::NUM); i++)
	{
		auto obj = m_pMenuItem_RectTransform[i].lock()->get_OwnerObj();
		if (obj.expired())
		{
			assert(false);
			continue;
		}
		obj.lock()->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MainMenu Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Title_MainMenu::Update(SceneManager* pOwner)
{
	POINT mousePos = Master::m_pInputManager->GetMousePos();	// マウス座標

	//m_NextState = c_TITLE_MAIN_MENU;

	int i = 0;
	for (auto &button : m_pButtons)
	{
		Button::STATE state = button.lock()->get_State();
		
		m_MenuItemInfoArray[i]._isHovered = false;

		switch (state)
		{
		case Button::STATE::NORMAL:
			break;
		case Button::STATE::HIGH_LIGHTED:
			m_MenuItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			break;
		case Button::STATE::PRESSED:
			m_MenuItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			break;
		case Button::STATE::SELECTED:
			break;
		case Button::STATE::DISABLED:
			break;
		default:
			break;
		}
		
		i++;
	}

    return m_NextState;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_MainMenu Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_MainMenu::Draw(SceneManager* pOwner)
{
	for (auto& item : m_MenuItemInfoArray)
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
			if (m_PrevHoveredMenuItem != item._type)
			{
				// ****************************************************
				//				カーソルが載った時のSE再生
				// ****************************************************
				Master::m_pSoundManager->Play(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::SYSTEM_MOVING_CURSOR01));
			}

			m_PrevHoveredMenuItem = item._type;	// なんの項目に乗ったか保持
		}
		
		m_pMenuItem_RectTransform[static_cast<int>(item._type)].lock()->set_RectPosition(VEC2(spritePos.x, spritePos.y));
		m_pMenuItem_RectTransform[static_cast<int>(item._type)].lock()->set_Size(450.0f, 100.0f);
	}

	Master::m_pDirectWriteManager->DrawString("☆メインメニュー", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");

}
