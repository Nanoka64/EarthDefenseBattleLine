#pragma once
#include "IState.h"

// PT:巡回中
// AT:戦闘態勢

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_PT_IdleState Class --- */
//
//  ★継承：IState ★
//
// 【?】[非アクティブ]
//		待機時ステート
//		
// ***************************************************************************************
class Ant_PT_IdleState : public IState<class EnemyController>
{
private:
	const float SEARCH_RANGE = 40.0f;				// 索敵範囲
	const float SEARCH_FIELD_OF_VIEW_DEG = 70.0f;	// 視界
	const float IDLE_MIN_TIME = 2.0f;				// 移動の最小時間
	const float IDLE_MAX_TIME = 5.0f;				// 移動の最大時間
	float m_IdleDuration = 0.0f;	// 待機状態の時間

public:
	void OnEnter(class EnemyController* pOwner) override;
	void OnExit(class EnemyController* pOwner)override;
	int Update(class EnemyController* pOwner)override;

};

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_PT_MoveState Class --- */
//
//  ★継承：IState ★
//
// 【?】[非アクティブ]
//		移動ステート
//		
// ***************************************************************************************
class Ant_PT_MoveState : public IState<class EnemyController>
{
private:
	const float SEARCH_RANGE = 40.0f;				// 索敵範囲
	const float SEARCH_FIELD_OF_VIEW_DEG = 70.0f;	// 視界
	const float MOVE_TIME_MIN = 3.0f;				// 移動の最小時間
	const float MOVE_TIME_MAX = 7.0f;				// 移動の最大時間
	const float MOVE_RANGE = 50.0f;					// 移動範囲
	const float DIR_RAND_MAX = Tool::G_PI_4_F;		// 移動の方向 ランダム最大 
	const float DIR_RAND_MIN = -Tool::G_PI_4_F;		// 移動の方向 ランダム最小 

	float m_MoveDuration = 0.0f;	// 移動時間
	VECTOR3::VEC3 m_MoveDir;		// 移動方向
	bool m_IsDirChange = false;		// 方向転換したか（生物感を出すために、一回方向転換させる）

public:
	void OnEnter(class EnemyController* pOwner) override;
	void OnExit(class EnemyController* pOwner)override;
	int Update(class EnemyController* pOwner)override;

};


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_AT_TrackingState Class --- */
//
//  ★継承：IState ★
//
// 【?】[アクティブ]
//		追従ステート
//		
// ***************************************************************************************
class Ant_AT_TrackingState : public IState<class EnemyController>
{
private:

	const float TRACKING_TIME_MAX = 7.0f;	// 追跡の最大時間
	const float TRACKING_TIME_MIN = 3.0f;	// 追跡の最小時間

	float m_TrackingDuration;	// 追従時間

public:
	void OnEnter(class EnemyController *pOwner) override;
	void OnExit(class EnemyController *pOwner)override;
	int Update(class EnemyController *pOwner)override;
};


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_AT_MoveState Class --- */
//
//  ★継承：IState ★
//
// 【?】[アクティブ]
//		移動ステート
//		
// ***************************************************************************************
class Ant_AT_MoveState : public IState<class EnemyController>
{
private:
	const float SEARCH_RANGE = 40.0f;				// 索敵範囲
	const float SEARCH_FIELD_OF_VIEW_DEG = 70.0f;	// 視界
	const float MOVE_TIME_MIN = 3.0f;				// 移動の最小時間
	const float MOVE_TIME_MAX = 7.0f;				// 移動の最大時間
	const float MOVE_RANGE = 50.0f;					// 移動範囲
	const float DIR_RAND_MAX = Tool::G_PI_4_F;		// 移動の方向 ランダム最大 
	const float DIR_RAND_MIN = -Tool::G_PI_4_F;		// 移動の方向 ランダム最小 

	float m_MoveDuration = 0.0f;	// 移動時間
	VECTOR3::VEC3 m_MoveDir;		// 移動方向
	bool m_IsDirChange;				// 方向転換したか

public:
	void OnEnter(class EnemyController *pOwner) override;
	void OnExit(class EnemyController *pOwner)override;
	int Update(class EnemyController *pOwner)override;
};


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_AT_AttackAcidState Class --- */
//
//  ★継承：IState ★
//
// 【?】[アクティブ]
//		酸攻撃ステート
//		
// ***************************************************************************************
class Ant_AT_AttackAcidState : public IState<class EnemyController>
{
private:
	const float PREATTACK_STUN_TIME_MAX = 4.0f;		// 攻撃前の硬直最大時間
	const float PREATTACK_STUN_TIME_MIN = 1.0f;		// 攻撃前の硬直最小時間 

	float m_PreAttackStunDuration = 0.0f;	// 攻撃前の硬直時間

public:
	void OnEnter(class EnemyController *pOwner) override;
	void OnExit(class EnemyController *pOwner)override;
	int Update(class EnemyController *pOwner)override;
};
