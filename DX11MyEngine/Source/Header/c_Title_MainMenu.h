#pragma once
#include "IState.h"
#include "SceneStateEnums.h"


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
	SceneStateEnums::c_TITLE m_NextState = SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU;	// 次のステート

	UtilityData::MainMenuItemInfo m_MenuItemInfoArray[static_cast<int>(UtilityData::TITLEMENU_ITEM::NUM)];				// メニュー項目情報配列
	class GameObject* m_pButtonsObjArray[static_cast<int>(UtilityData::TITLEMENU_ITEM::NUM)];		// メニュー項目オブジェクト配列
	std::weak_ptr<class ButtonUI> m_pButtonArray[static_cast<int>(UtilityData::TITLEMENU_ITEM::NUM)];	// メニュー項目のButtonUI配列  
	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[static_cast<int>(UtilityData::TITLEMENU_ITEM::NUM)]; // メニュー項目のRectTransform配列


	UtilityData::TITLEMENU_ITEM m_PrevHoveredMenuItem;	// 前にマウスが乗っていた項目（SE用）
	bool m_IsInit;	// 既に初期化済みか

public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};

