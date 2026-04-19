#pragma once
#include "IState.h"


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_MissionSelect_DifficultSelect Class --- */
//
// 【?】ミッション選択の子シーン
//		難易度選択
//
// ***************************************************************************************
class c_MissionSelect_DifficultSelect : public IState<SceneManager>
{
private:
	class GameObject* m_pBackSpriteObj;	// 背景スプライトオブジェクト
	std::array<class GameObject*, UINT_CAST(UtilityData::DIFFICULTY_LEVEL::NUM)> m_pButtonsObjArray;	// 難易度選択のボタン
	std::array<std::weak_ptr<class ButtonUI>, UINT_CAST(UtilityData::DIFFICULTY_LEVEL::NUM)>m_pButtonArray;	// 難易度選択のButtonUI配列  
	std::array<std::weak_ptr<class RectTransform>, UINT_CAST(UtilityData::DIFFICULTY_LEVEL::NUM)>m_pItemRectTransformArray;	// 難易度選択のButtonUI配列  
	SceneStateEnums::c_TITLE m_NextState = SceneStateEnums::c_TITLE::c_TITLE_DIFFICULT_SELECT;	// 次のステート
	UtilityData::DIFFICULTY_LEVEL m_CrntSelectItem;	// 現在選択中の項目


public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;

	void DifficultSelectButton_OnClicFunction(SceneManager* pOwner, int _i);		// ボタンが押された際の処理
};

