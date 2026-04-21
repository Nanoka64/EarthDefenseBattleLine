#pragma once
#include "CompositeState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Game_Play Class --- */
//
// 【?】ゲームシーンの子ステート
//		ゲームプレイ
//	
//		ポーズ画面の子ステートを持つ
//
// ***************************************************************************************
class c_Game_Play : public CompositeState<SceneManager>
{
private:
	int m_EnemyNum;	// 残り敵数

	/* オブジェクト */
	std::weak_ptr<class GameObject>m_pBomber[3];
	std::weak_ptr<class GameObject>m_pPlayerObj;
	class GameObject* m_pEnemyNumBackSpriteObj;	// 設定画面の背景スプライトオブジェクト

public:
	void OnEnter(SceneManager * pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;

	void BomberMove();
};


