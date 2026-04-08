#pragma once
#include "IComponent.h"


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:MiniMapRader Class --- */
//
//  ★継承：IComponent ★
//
// 【?】ミニマップ用
//		
// ***************************************************************************************
class MiniMapRader :  public IComponent
{
private:
	class GameObject* m_pRaderBackSpriteObj;		// レーダー背景オブジェクト
	std::vector<class GameObject*> m_pRaderEnemySpriteObjArray;   // レーダーに表示するオブジェクト
	std::weak_ptr<class GameObject>m_pPlayerObj;

	int m_MaxEnemyNum;	// レーダーに表示する最大敵数
	int m_CrntEnemyNum;	// 現在レーダーに表示している敵数

public:
	MiniMapRader(std::weak_ptr<GameObject> pOwner, int updateRank);
	~MiniMapRader();

	void Start(RendererEngine& renderer) override;	// 初期化
	void Update(RendererEngine& renderer) override;	// 更新

private:
	void ClampToMinimap(int& _Out_X, int& _Out_Y, int _centerX, int _centerY, float _radius);
};

