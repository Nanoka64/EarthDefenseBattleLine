#pragma once
#include "IState.h"
#include "SceneStateEnums.h"

/// <summary>
/// 兵科の種類
/// </summary>
enum class SOLDIER_TYPE
{
	RANGER,	// 歩兵
	WING,	// 歩兵
	AIR,	// 歩兵
	FEN,	// 歩兵

	NUM,
};

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Title_SoldierSelect Class --- */
//
// 【?】タイトルシーンの子ステート
//		兵科選択をする
//
// ***************************************************************************************
class c_Title_SoldierSelect : public IState<SceneManager>
{
private:
	SceneStateEnums::c_TITLE m_NextState = SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU;	// 次のステート
	SOLDIER_TYPE m_CrntSelectedSoldierType;	// 現在選択している兵科
	int m_CrntSelectItem;					// 選択されている項目
	bool m_IsInit;							// 既に初期化済みか

	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[UINT_CAST(SOLDIER_TYPE::NUM)];
	std::weak_ptr<class ButtonUI> m_pButtonArray[UINT_CAST(SOLDIER_TYPE::NUM)];	// ボタン用
	class GameObject* m_pButtonsObjArray[UINT_CAST(SOLDIER_TYPE::NUM)];			// ボタンオブジェクトの配列
	class GameObject* m_pWeaponDescriptionBackSpriteObj;	//  武器説明用のスプライトオブジェクト
	
	int m_DecisionTextDrawCounter; // 装備決定時のテキストを表示するためのカウンター
	int m_DecisionSoldierTypeIndex; // 決定した兵科のインデックス
	
	UtilityData::MenuItemInfo m_ItemInfoArray[UINT_CAST(SOLDIER_TYPE::NUM)];	// メニュー項目の情報の配列

public:
	void OnEnter(SceneManager *pOwner) override;
	void OnExit(SceneManager *pOwner)override;
	int Update(SceneManager *pOwner)override;
	void Draw(SceneManager *pOwner)override;
};

