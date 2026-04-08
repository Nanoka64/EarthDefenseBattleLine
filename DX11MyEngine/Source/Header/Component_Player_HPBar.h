#pragma once
#include "IComponent.h"
class Player_HPBar :  public IComponent
{
private:
	std::weak_ptr<class GameObject>m_pPlayerObj;

	/* スプライト */
	class GameObject* m_pPlayerHPBarBackSpriteObj;	// プレイヤーのHPバー背景オブジェクト
	class GameObject* m_pPlayerHPBarSpriteObj;		// プレイヤーのHPバーオブジェクト
public:
	Player_HPBar(std::weak_ptr<GameObject> pOwner, int updateRank);
	~Player_HPBar();

	void Start(RendererEngine& renderer) override;	// 初期化
	void Update(RendererEngine& renderer) override;	// 更新

};

