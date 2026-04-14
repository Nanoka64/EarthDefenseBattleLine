#include "pch.h"
#include "Component_EnemyController.h"
#include "Ant_StateHeader.h"
#include "Component_MoveLogic.h"
#include "GameObject.h"

using namespace VECTOR3;
using namespace VECTOR2;
using namespace UtilityData;
using namespace EnemyData;

//*---------------------------------------------------------------------------------------
//* @:Ant_AT_AttackAcidState Class 
//*【?】開始
//* 引数：1.EnemyController
//* 返値：void
//*----------------------------------------------------------------------------------------
void Ant_AT_AttackAcidState::OnEnter(class EnemyController* pOwner)
{
	// 移動ベクトルは0
	pOwner->set_MoveVelocity(VEC3());

	pOwner->set_IsAnim(true);

	pOwner->set_MoveLogicState(MOVE_BEHAVIOUR_TYPE::HOMING);

	// 攻撃前の硬直時間
	m_PreAttackStunDuration = Master::m_pRandomManager->GetFloatRandom(PREATTACK_STUN_TIME_MIN, PREATTACK_STUN_TIME_MAX);
}

//*---------------------------------------------------------------------------------------
//* @:Ant_AT_AttackAcidState Class 
//*【?】終了
//* 引数：1.EnemyController
//* 返値：void
//*----------------------------------------------------------------------------------------
void Ant_AT_AttackAcidState::OnExit(class EnemyController* pOwner)
{
	pOwner->clear_StateTimer();
}

//*---------------------------------------------------------------------------------------
//* @:Ant_AT_AttackAcidState Class 
//*【?】更新
//* 引数：1.EnemyController
//* 返値：void
//*----------------------------------------------------------------------------------------
int Ant_AT_AttackAcidState::Update(class EnemyController* pOwner)
{
	auto target = pOwner->get_Target();	// 目標オブジェクト
	if (target == nullptr || pOwner->get_OwnerObj().expired())
	{
		MessageBox(NULL, "ターゲットがいません", "Ant_PT_MoveState", MB_OK);
		assert(false);
	}
	else
	{

		auto targetTransform = target->get_Transform().lock();
		VEC3 targetPos = targetTransform->get_VEC3ToPos();	// 目標位置
		auto myTransform = pOwner->get_OwnerObj().lock()->get_Transform().lock();
		VEC3 myPos = myTransform->get_VEC3ToPos();			// 自分の位置

		VEC3 targetDir = (targetPos - myPos).Normalize();	// 目標方向


		// 移動はさせずに、目標への方向だけ向かせる
		MoveParam movePram;
		movePram._moveSpeed = 1.0f;
		movePram._turnSpeed = 0.5f;
		movePram._targetPos = targetPos;		
		auto move = pOwner->get_MoveLogicComponent().lock();
		move->Calculate(movePram);

		/* 硬直時間を終えたら、攻撃 */
		if (pOwner->get_StateTimer() > m_PreAttackStunDuration)
		{
			// 攻撃処理
			BulletData::NormalBulletData bullet;
			BulletTransformData bulletTrans;
			bulletTrans._pos = targetPos;
			bulletTrans._scale = 0.01f;
			bulletTrans._rotQ = myTransform->get_RotationQuaternion();
			bullet._speed = 20.0f;
			bullet._range = 100.0f;
			Master::m_pBulletManager->Shot(*m_pRenderer, bulletTrans, bullet);


			//=========================================================================================
			//
			//						攻撃が終了したら、移動ステートへ
			//
			//=========================================================================================
			return ANT_STATE::ANT_STATE_ACTIVE_MOVE;
		}

	}

	return ANT_STATE::ANT_STATE_ACTIVE_ATTACK_ACID;
}
