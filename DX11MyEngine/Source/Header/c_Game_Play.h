#pragma once
#include "IState.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:c_Game_Play Class --- */
//
// 【?】ゲームシーンの子ステート
//		ゲームプレイ
//
// ***************************************************************************************
class c_Game_Play : public IState<SceneManager>
{
private:
	int m_EnemyNum;	// 残り敵数

	/* オブジェクト */
	std::weak_ptr<class GameObject>m_pBomber[3];
	std::weak_ptr<class GameObject>m_pPlayerObj;

public:
	void OnEnter(SceneManager * pOwner) override;
	void OnExit(SceneManager* pOwner)override;
	int Update(SceneManager* pOwner)override;
	void Draw(SceneManager* pOwner)override;

	void BomberMove();
};


