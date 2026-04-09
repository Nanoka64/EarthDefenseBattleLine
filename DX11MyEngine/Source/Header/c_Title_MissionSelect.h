#pragma once
#include "IState.h"
#include "SceneStateEnums.h"

/// <summary>
/// ミッション選択中のどの状態か
/// </summary>
enum class MISSION_SELECT_PHASE
{
	SELECT_MISSION,			// ミッション選択
	SELECT_DIFFCULTY,		// 難易度選択
};

constexpr UINT MISSION_NUM = 1;	// ミッション数

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Title_MissionSelect Class --- */
//
// 【?】タイトルシーンの子ステート
//		ミッション選択をする
//
// ***************************************************************************************
class c_Title_MissionSelect : public IState<SceneManager>
{
private:
	UtilityData::MenuItemInfo m_ItemInfoArray[MISSION_NUM];	// メニュー項目の情報配列
	SceneStateEnums:: c_TITLE m_NextState = SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU;
	int m_PrevHoveredMIssionItem;	// 前にマウスが乗っていた項目（SE用）
	bool m_IsInit;	// 既に初期化済みか
	
	class GameObject* m_pButtonsObjArray[MISSION_NUM];							// ミッション項目オブジェクト配列
	std::weak_ptr<class ButtonUI> m_pButtonArray[MISSION_NUM];						// ミッション項目のButtonUI配列  
	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[MISSION_NUM];  // ミッション項目のRectTransform配列


public:
	void OnEnter(SceneManager *pOwner) override;
	void OnExit(SceneManager *pOwner)override;
	int Update(SceneManager *pOwner)override;
	void Draw(SceneManager *pOwner)override;
};

