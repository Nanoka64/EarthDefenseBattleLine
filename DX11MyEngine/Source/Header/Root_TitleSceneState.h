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

	class GameObject* m_pTitleLogoObj = nullptr;			// タイトルロゴオブジェクト
	class GameObject* m_pTitleBackObj = nullptr;			// タイトル背景の緑のスプライトオブジェクト
	class GameObject* m_pTitleBackGridObj_Red = nullptr;	// タイトル背景のグリッドスプライトオブジェクト - 赤
	class GameObject* m_pTitleBackGridObj_Blue = nullptr;	// タイトル背景のグリッドスプライトオブジェクト - 青

	VECTOR2::VEC2 m_UVScroll;
	VECTOR2::VEC2 m_UVScroll2;
public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};


