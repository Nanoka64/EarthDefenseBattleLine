#include "pch.h"
#include "WeaponStateFactory.h"
#include "ConstantWeaponData.h"
#include "Component_GunWeapon.h"
#include "Gun_StateHeader.h"

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
void WeaponStateFactory::CreateGunWeapon(StateMachine<GunWeapon>& _out, int _createState, RendererEngine& _renderer)
{
	// ルートシーン作成
	std::shared_ptr<Gun_IdleState> pIdle = std::make_shared<Gun_IdleState>();					// 待機
	std::shared_ptr<Gun_ReloadingState> pReloading = std::make_shared<Gun_ReloadingState>();	// リロード
	std::shared_ptr<Gun_FireState> pFire = std::make_shared<Gun_FireState>();					// 発射
	
	//描画インターフェイス設定
	// add_childの中で子シーンにもセットしてるのでaddする前に呼んで！
	pIdle->set_Renderer(&_renderer);
	pReloading->set_Renderer(&_renderer);
	pFire->set_Renderer(&_renderer);
	
	//ステートマシンに登録
	_out.RegisterState(GUN_STATE::GUN_STATE_IDLE, std::move(pIdle));
	_out.RegisterState(GUN_STATE::GUN_STATE_RELOADING, std::move(pReloading));
	_out.RegisterState(GUN_STATE::GUN_STATE_FIRE, std::move(pFire));
}
