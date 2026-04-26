#pragma once
#include "IComponent.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:BuildingController Class --- */
//
//  ★継承：IComponent ★
//
// 【?】建物の挙動を管理
//		実際の挙動はステートマシン内で行っている
//		それぞれのコンポーネントと値を行き来させる箱みたいな感じ
//		
// ***************************************************************************************
class BuildingController : public IComponent
{
private:
	
	std::weak_ptr<class Health>m_pHealthComp;	// 体力管理コンポーネント

	StateMachine<BuildingController> m_StateMachine;	// ステートマシン

	// ステートマシンの状態を管理するためのフラグ
	bool m_IsDestruction;	// 破壊されたかどうか


public:
	BuildingController(std::weak_ptr<GameObject> pOwner, int updateRank);
	~BuildingController();

	void Start(RendererEngine& renderer) override;	// 初期化
	void Update(RendererEngine& renderer) override;	// 更新
	void OnCollisionEnter(const class CollisionInfo& _other)override;

	bool get_IsDestruction()const { return m_IsDestruction; }	// 破壊されたかどうかの取得
};

