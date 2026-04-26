#pragma once
#include "CompositeState.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//						リザルトシーンのステートのヘッダクラス定義をまとめたヘッダ
// 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Root_ResultSceneState Class --- */
//
// 【?】リザルトシーン親ステート
//
// ***************************************************************************************
class Root_ResultSceneState : public CompositeState<SceneManager>
{
private:

public:
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};


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
	void OnEnter(SceneManager* pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;
};

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
	const float TEXT_FADE_IN_DURATION = 5.0f;	// テキストのフェードインにかける時間
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

