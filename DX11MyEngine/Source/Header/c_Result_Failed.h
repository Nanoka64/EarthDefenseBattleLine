#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Result_GetItemCheck Class --- */
//
// 【?】ミッション失敗
//
// ***************************************************************************************
class c_Result_Failed : public IState<SceneManager>
{
private:
	const float FADE_IN_TEXT_DURATION = 3.0f;	// テキストのフェードインにかける時間
	const float FADE_DURATION = 2.0f;			// フェードアウトにかける時間
	const float FADE_IN_WIPE_START_DELAY = 1.0f;	// フェードアウトを開始するまでの時間（BGMに合わせる）

	float m_ElapsedTime = 0.0f;					// 経過時間
	class GameObject* m_pFailedWipeSpriteObj;	// ミッション失敗時のワイプスプライト
	class GameObject* m_pFailedTextSpriteObj;	// ミッション失敗時のワイプスプライト
	std::weak_ptr<class SpriteRenderer> m_pFailedTextSpriteRenderer;	// テキスト用スプライトのコンポーネント
	std::weak_ptr<class SpriteRenderer> m_pFailedWipeSpriteRenderer;	// ワイプ用スプライトのコンポーネント
public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
};

