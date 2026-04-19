#pragma once
#include "IState.h"
#include "SceneStateEnums.h"


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
	UtilityData::SOLDIER_TYPE m_CrntSelectedSoldierType;	// 現在選択している兵科
	int m_CrntSelectItem;					// 選択されている項目
	bool m_IsInit;							// 既に初期化済みか

	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[UINT_CAST(UtilityData::SOLDIER_TYPE::NUM)];
	std::weak_ptr<class ButtonUI> m_pButtonArray[UINT_CAST(UtilityData::SOLDIER_TYPE::NUM)];	// ボタン用
	class GameObject* m_pButtonsObjArray[UINT_CAST(UtilityData::SOLDIER_TYPE::NUM)];			// ボタンオブジェクトの配列
	class GameObject* m_pWeaponDescriptionBackSpriteObj;	//  武器説明用のスプライトオブジェクト
	
	int m_DecisionTextDrawCounter; // 装備決定時のテキストを表示するためのカウンター
	int m_DecisionSoldierTypeIndex; // 決定した兵科のインデックス
	
	UtilityData::MenuItemInfo m_ItemInfoArray[UINT_CAST(UtilityData::SOLDIER_TYPE::NUM)];	// メニュー項目の情報の配列

public:
	void OnEnter(SceneManager *pOwner) override;
	void OnExit(SceneManager *pOwner)override;
	int Update(SceneManager *pOwner)override;
	void Draw(SceneManager *pOwner)override;

private:

	/// <summary>
	/// パラメータの描画
	/// </summary>
	/// <param name="_label">ラベル名（ダメージ:とか連射速度:にあたるもの）</param>
	/// <param name="_value">値</param>
	/// <param name="_pos">位置</param>
	void DrawParam(const std::wstring& _label, const std::wstring& _value, const VECTOR2::VEC2& _pos);


	void DrawWeaponInfo(const WeaponData::GunWeaponData* weaponData, int weaponIndex, float startX, float startY);
};

