#pragma once
#include "IComponent.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:EnemyController Class --- */
//
//  ★継承：IComponent ★
//
// 【?】敵の挙動を管理
//		実際の挙動はステートマシン内で行っている
//		それぞれのコンポーネントと値を行き来させる箱みたいな感じ
//		
// ***************************************************************************************
class EnemyController :  public IComponent
{
private:
	std::weak_ptr<class Health>m_pHealthComp;	// 体力管理コンポーネント
	std::weak_ptr<class SkinnedMeshAnimator> m_pAnimatorComp;	// アニメータコンポーネント
	std::weak_ptr<class Collider> m_pColliderComp;	// コライダーコンポーネント
	std::weak_ptr<class MoveLogic> m_pMoveLogicComp;	// 移動コンポーネント
	std::weak_ptr<GameObject> m_pTarget;	// 攻撃目標
	StateMachine<EnemyController> m_StateMachine;

	VECTOR3::VEC3 m_MoveVelocity;	// 移動
	int m_CrntAnimID;				// 現在のアニメーション番号
	float m_MoveSpeed;				// 移動速度
	int m_StateTimer;				// ステート時間
	bool m_IsDead;
	bool m_IsAnim;					// アニメーション中かどうか


public:
	EnemyController(std::weak_ptr<GameObject> pOwner, int updateRank);
	~EnemyController();

	void Start(RendererEngine& renderer) override;	// 初期化
	void Update(RendererEngine& renderer) override;	// 更新
	void Draw(RendererEngine& renderer) override;	// 描画
	void OnCollisionEnter(const class CollisionInfo &_other)override;
	
	/// <summary>
	/// ステートの変更
	/// </summary>
	/// <param name="_state"></param>
	void ChangeState(const int _state);

	/// <summary>
	/// アニメーションの変更
	/// </summary>
	/// <param name="id"></param>
	void ChangeAnimation(const int _newId);

	/* アニメーションID */
	void set_AnimID(const int _id) { m_CrntAnimID = _id; }
	int get_AnimID()const { return m_CrntAnimID; }

	/* 移動速度 */
	float get_MoveSpeed() const { return m_MoveSpeed; }
	void set_MoveSpeed(float _spd) { m_MoveSpeed = _spd; }

	/* 移動ベクトル */
	VECTOR3::VEC3 get_MoveVelocity() const { return m_MoveVelocity; }
	void set_MoveVelocity(const VECTOR3::VEC3 &_vel) { m_MoveVelocity = _vel; }

	/* アニメーションの再生させるかどうか */
	bool get_IsAnim() const { return m_IsAnim; }
	void set_IsAnim(bool _flag) { m_IsAnim = _flag; }

	/* 攻撃目標 */
	std::weak_ptr<GameObject> get_Target() const { return m_pTarget; }
	void set_Target(std::shared_ptr<GameObject> _pObj) { m_pTarget = _pObj; }

	/* 攻撃目標のトランスフォームを取得 */
	std::shared_ptr<class MyTransform> get_TargetTransform() const;

	/* ステート時間 */
	int get_StateTimer()const { return m_StateTimer; }

};

