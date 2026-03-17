#pragma once
#include "CompositeState.h"


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Root_TitleSceneState Class --- */
//
// 【?】タイトルシーンの親ステート
//
// ***************************************************************************************
class Root_TitleSceneState : public CompositeState<SceneManager>
{
private:
	bool m_IsLoad;	// ロード完了しているか
	
	// 緑背景
	std::shared_ptr<class SpriteRenderer> m_pBackSprite;

	// 赤いグリッド背景
	std::shared_ptr<class SpriteRenderer> m_pBackSprite2;		
	std::shared_ptr<class RectTransform> m_pBackSprite2_Rect;	

	// 青いグリッド背景
	std::shared_ptr<class SpriteRenderer> m_pBackSprite3;		
	std::shared_ptr<class RectTransform> m_pBackSprite3_Rect;	

	VECTOR2::VEC2 m_UVScroll;
	VECTOR2::VEC2 m_UVScroll2;
public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};


