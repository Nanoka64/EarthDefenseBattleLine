#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Game_Pause Class --- */
//
// 【?】ゲームプレイ中の設定画面
//
// ***************************************************************************************
class c_Game_Pause : public IState<SceneManager>
{
private:
	class GameObject* m_pPauseBackSpriteObj;	// ポーズ背景スプライト
	std::array<class GameObject*, UINT_CAST(UtilityData::PAUSE_ITEM::NUM)> m_pPauseItemBackButtonObjArray;								// 項目背景のボタンオブジェクト
	std::array<std::weak_ptr<class RectTransform>, UINT_CAST(UtilityData::PAUSE_ITEM::NUM)> m_pPauseItemBackRectTransformArray;			// 項目背景のRectTransform
	
	
	SceneStateEnums::c_GAME m_NextState;	
public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;

	// 設定値を変更する
	void ChangeConfigValue(UtilityData::CONFIG_ITEM _item, bool _isLeftIndex);
};

