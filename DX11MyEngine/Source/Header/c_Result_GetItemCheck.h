#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Result_GetItemCheck Class --- */
//
// 【?】リザルトシーンの子ステート
//		取得したアイテムの確認（※まだアイテム無いのでそのまま終了）
//
// ***************************************************************************************
class c_Result_GetItemCheck : public IState<SceneManager>
{
private:
	const float WIPE_DURATION = 5.0f;
	float m_ElapsedTime = 0.0f;

	class GameObject* m_pWipeSpriteObj;
	std::weak_ptr<class SpriteRenderer> m_pWipeSpriteRenderer;

public:
	void OnEnter(SceneManager *pOwner) override;
	void OnExit(SceneManager *pOwner)override;
	int Update(SceneManager *pOwner)override;
	void Draw(SceneManager *pOwner)override;
};

