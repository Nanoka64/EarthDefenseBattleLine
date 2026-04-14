#include "pch.h"
#include "EnemyStateFactory.h"
#include "Component_EnemyController.h"
#include "RendererEngine.h"

// アリ
#include "Ant_StateHeader.h"

using namespace EnemyData;


//*---------------------------------------------------------------------------------------
//*【?】ステートの作成
//*
//* [引数]
//* &_out : 出力先
//* &_createState : 作成する敵ステート
//* &renderer : 描画エンジンの参照
//*
//* [返値]
//* なし 
//*----------------------------------------------------------------------------------------
void EnemyStateFactory::Create(StateMachine<EnemyController>& _out, int _createState, RendererEngine& _renderer)
{
	switch (_createState)
	{
	case ENEMY_TYPE_ANT_Normal:
		CreateAntState(_out, _renderer);
		break;
	default:
		break;
	};
}


//*---------------------------------------------------------------------------------------
//*【?】蟻ステートの作成
//*
//* [引数]
//* &_out : 出力先
//* &renderer : 描画エンジンの参照
//*
//* [返値]
//* true : 成功
//* false : 失敗
//*----------------------------------------------------------------------------------------
void EnemyStateFactory::CreateAntState(StateMachine<class EnemyController>& _out, RendererEngine& _renderer)
{
	// ルートシーン作成
	std::shared_ptr<Ant_PT_IdleState> pPTIdle = std::make_shared<Ant_PT_IdleState>();
	std::shared_ptr<Ant_PT_MoveState> pPTMove = std::make_shared<Ant_PT_MoveState>();
	std::shared_ptr<Ant_AT_TrackingState> pATTracking = std::make_shared<Ant_AT_TrackingState>();
	std::shared_ptr<Ant_AT_MoveState> pATMove = std::make_shared<Ant_AT_MoveState>();
	std::shared_ptr<Ant_AT_AttackAcidState> pATAttackAcid = std::make_shared<Ant_AT_AttackAcidState>();

	//描画インターフェイス設定
	// add_childの中で子シーンにもセットしてるのでaddする前に呼んで！
	pPTIdle->set_Renderer(&_renderer);
	pPTMove->set_Renderer(&_renderer);
	pATTracking->set_Renderer(&_renderer);
	pATMove->set_Renderer(&_renderer);
	pATAttackAcid->set_Renderer(&_renderer);

	// ********************************************************************************
	// 子ステート作成
	// ********************************************************************************
	//std::shared_ptr<c_Title_LoadProcess> c_LoadProcess = std::make_shared<c_Title_LoadProcess>();		// ロード処理

	// 親を設定
	//c_LoadProcess->set_Parent(pTitleScene);

	// 子を登録
	//pTitleScene->add_Child(c_TITLE::c_TITLE_LOAD_PROCESS, (c_LoadProcess));

	//ステートマシンに登録
	_out.RegisterState(ANT_STATE::ANT_STATE_PATROL_IDLE, std::move(pPTIdle));
	_out.RegisterState(ANT_STATE::ANT_STATE_PATROL_MOVE, std::move(pPTMove));
	_out.RegisterState(ANT_STATE::ANT_STATE_ACTIVE_TRACKING, std::move(pATTracking));
	_out.RegisterState(ANT_STATE::ANT_STATE_ACTIVE_MOVE, std::move(pATMove));
	_out.RegisterState(ANT_STATE::ANT_STATE_ACTIVE_ATTACK_ACID, std::move(pATAttackAcid));
}
