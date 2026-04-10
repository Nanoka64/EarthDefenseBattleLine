#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Title_Config Class --- */
//
// 【?】タイトルシーンの子ステート
//		設定をする
//
// ***************************************************************************************
class c_Title_Config : public IState<SceneManager>
{
private:
	class GameObject* m_pConfigBackSpriteObj;	// 設定画面の背景スプライトオブジェクト
	class GameObject* m_pConfigMenuSpriteObjArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];			// 設定項目のオブジェクト配列
	class GameObject* m_pConfigAdjustmentButtonObj[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)][2];		// 設定項目の調整カーソルのオブジェクト[0]が左、[1]が右
	std::weak_ptr<class ButtonUI> m_pButtonArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];							// ButtonUI配列  
	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];		// RectTransform配列
	UtilityData::ConfigMenuItemInfo m_ItemInfoArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];				// 項目の情報配列

public:
	void OnEnter(SceneManager *pOwner) override;
	void OnExit(SceneManager *pOwner)override;
	int Update(SceneManager *pOwner)override;
	void Draw(SceneManager *pOwner)override;

	// 設定値を変更する
	void ChangeConfigValue(UtilityData::CONFIG_ITEM _item, bool _isLeftIndex);

};
