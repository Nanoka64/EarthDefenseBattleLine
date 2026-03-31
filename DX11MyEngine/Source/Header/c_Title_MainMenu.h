#pragma once
#include "IState.h"
#include "SceneStateEnums.h"


/// <summary>
/// タイトル項目
/// </summary>
enum class TITLEMENU_ITEM
{
	MISSION_SELECT,	// ミッション選択
	SOLDER_SELECT,	// 兵科選択
	CONFIG,			// 設定
	EXIT,			// 終了

	NUM,
};


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Title_MainMenu Class --- */
//
// 【?】タイトルシーンの子ステート
//		タイトル項目の選択をする
//
// ***************************************************************************************
class c_Title_MainMenu : public IState<SceneManager>
{
private:
	/// <summary>
	/// メニュー項目情報
	/// </summary>
	struct MenuItemInfo
	{
		VECTOR2::VEC2 _pos;
		const char* _name;
		TITLEMENU_ITEM _type;
		SceneStateEnums::c_TITLE _nextState;// 選択した際の遷移先ステート
		bool _isHovered;	// マウスが上に乗っているか

		MenuItemInfo() :
			_pos(VECTOR2::VEC2()),
			_name(),
			_type(TITLEMENU_ITEM::NUM),
			_nextState(SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU),
			_isHovered(false)
		{
		};
	};

	SceneStateEnums::c_TITLE m_NextState = SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU;	// 次のステート

	MenuItemInfo m_MenuItemInfoArray[static_cast<int>(TITLEMENU_ITEM::NUM)];				// メニュー項目情報配列
	class GameObject* m_pButtonsObjArray[static_cast<int>(TITLEMENU_ITEM::NUM)];		// メニュー項目オブジェクト配列
	std::weak_ptr<class ButtonUI> m_pButtonArray[static_cast<int>(TITLEMENU_ITEM::NUM)];	// メニュー項目のButtonUI配列  
	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[static_cast<int>(TITLEMENU_ITEM::NUM)]; // メニュー項目のRectTransform配列


	TITLEMENU_ITEM m_PrevHoveredMenuItem;	// 前にマウスが乗っていた項目（SE用）
	bool m_IsInit;	// 既に初期化済みか

public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};

