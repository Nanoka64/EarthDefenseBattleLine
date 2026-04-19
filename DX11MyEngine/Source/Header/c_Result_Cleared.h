#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Result_GetItemCheck Class --- */
//
// 【?】フェードアウト
//
// ***************************************************************************************
class c_Result_Cleared : public IState<SceneManager>
{
private:
	const float TEXT_FADE_IN_DURATION = 3.0f;	// テキストのフェードインにかける時間
	const float WHITE_FADE_DURATION = 3.0f;		// フェードアウトにかける時間
	const float BLACK_FADE_DURATION = 2.0f;		// フェードアウトにかける時間
	const float FADE_OUT_START_DELAY = 2.0f;	// フェードアウトを開始するまでの時間（BGMに合わせる）
	float m_ElapsedTime = 0.0f;					// 経過時間

	/*スプライト*/
	class GameObject* m_pClearedTextSpriteObj;	// クリアテキスト
	class GameObject* m_pWhiteWipeSpriteObj;	// 段々白くするためのスプライト
	class GameObject* m_pBlackWipeSpriteObj;	// 最後、だんだん暗くするためのスプライト
	std::weak_ptr<class SpriteRenderer> m_pClearedTextSpriteRenderer;
	std::weak_ptr<class SpriteRenderer> m_pWhiteWipeSpriteRenderer;
	std::weak_ptr<class SpriteRenderer> m_pBlackWipeSpriteRenderer;
public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};

