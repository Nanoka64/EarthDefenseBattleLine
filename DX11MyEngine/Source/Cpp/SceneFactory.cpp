#include "pch.h"
#include "SceneFactory.h"
#include "SceneStateEnums.h"
#include "RendererEngine.h"

/* タイトル */
#include "Root_TitleSceneState.h"
#include "c_Title_LoadProcess.h"
#include "c_Title_MainMenu.h"
#include "c_Title_MissionSelect.h"
#include "c_MissionSelect_DifficultSelect.h"
#include "c_Title_SoldierSelect.h"
#include "c_Title_Config.h"

/* ゲーム */
#include "Root_GameSceneState.h"
#include "c_Game_LoadProcess.h"
#include "c_Game_Config.h"
#include "c_Game_Play.h"


/* リザルト */
#include "Root_ResultSceneState.h"
#include "c_Result_Cleared.h"
#include "c_Result_Failed.h"
#include "c_Result_GetItemCheck.h"



using namespace SceneStateEnums;



//*---------------------------------------------------------------------------------------
//* @:SceneFactory Class 
//*【?】シーンステートの作成
//* 引数：1.StateMachine
//* 返値：void
//*----------------------------------------------------------------------------------------
void SceneFactory::Create(StateMachine<SceneManager> &_out, int _createScene, RendererEngine &_renderer)
{
	switch (_createScene)
	{
	case SCENE_STATE_TITLE:
		CreateTitleScene(_out, _renderer);
		break;
	case SCENE_STATE_GAME:
		CreateGameScene(_out, _renderer);
		break;
	case SCENE_STATE_RESULT:
		CreateResultScene(_out, _renderer);
		break;
	default:
		break;
	};

}


//*---------------------------------------------------------------------------------------
//*【?】タイトルシーンの作成
//*
//* [引数]
//* &_out : 出力先
//* &renderer : 描画エンジンの参照
//*
//* [返値]
//* true : 成功
//* false : 失敗
//*----------------------------------------------------------------------------------------
void SceneFactory::CreateTitleScene(StateMachine<SceneManager> &_out, RendererEngine &_renderer)
{
	// ルートシーン作成
	std::shared_ptr<Root_TitleSceneState> pTitleScene = std::make_shared<Root_TitleSceneState>();

	//描画インターフェイス設定
	// add_childの中で子シーンにもセットしてるのでaddする前に呼んで！
	pTitleScene->set_Renderer(&_renderer);

	// ********************************************************************************
	// シーンの子ステート作成
	// ********************************************************************************
	std::shared_ptr<c_Title_LoadProcess> c_LoadProcess		= std::make_shared<c_Title_LoadProcess>();		// ロード処理
	std::shared_ptr<c_Title_MainMenu> c_MainMenu			= std::make_shared<c_Title_MainMenu>();			// メインメニュー
	std::shared_ptr<c_Title_MissionSelect> c_MissionSelect	= std::make_shared<c_Title_MissionSelect>();	// ミッション選択
	std::shared_ptr<c_Title_SoldierSelect> c_SoldierSelect	= std::make_shared<c_Title_SoldierSelect>();	// 兵科選択
	std::shared_ptr<c_Title_Config> c_Config				= std::make_shared<c_Title_Config>();			// 設定
	std::shared_ptr<c_MissionSelect_DifficultSelect> c_DifficultSelect	= std::make_shared<c_MissionSelect_DifficultSelect>();	// ミッション選択 - 難易度選択

	// 親を設定
	c_LoadProcess->set_Parent(pTitleScene);
	c_MainMenu->set_Parent(pTitleScene);
	c_MissionSelect->set_Parent(pTitleScene);
	c_SoldierSelect->set_Parent(pTitleScene);
	c_Config->set_Parent(pTitleScene);
	
	c_DifficultSelect->set_Parent(c_MissionSelect);

	// 子を登録
	pTitleScene->add_Child(c_TITLE::c_TITLE_LOAD_PROCESS, (c_LoadProcess));
	pTitleScene->add_Child(c_TITLE::c_TITLE_MAIN_MENU, (c_MainMenu));
	pTitleScene->add_Child(c_TITLE::c_TITLE_MISSION_SELECT, (c_MissionSelect));
	pTitleScene->add_Child(c_TITLE::c_TITLE_SOLDIER_SELECT, (c_SoldierSelect));
	pTitleScene->add_Child(c_TITLE::c_TITLE_CONFIG, (c_Config));

	c_MissionSelect->add_Child(c_TITLE::c_TITLE_DIFFICULT_SELECT, (c_DifficultSelect));	// ミッションの子に難易度選択

	// タイトルをステートマシンに登録
	_out.RegisterState(SCENE_STATE::SCENE_STATE_TITLE, std::move(pTitleScene));
}


//*---------------------------------------------------------------------------------------
//*【?】ゲームシーンの作成
//*
//* [引数]
//* &_out : 出力先
//* &renderer : 描画エンジンの参照
//*
//* [返値]
//* true : 成功
//* false : 失敗
//*----------------------------------------------------------------------------------------
void SceneFactory::CreateGameScene(StateMachine<SceneManager> &_out, RendererEngine &_renderer)
{
	// ルートシーン作成
	std::shared_ptr<Root_GameSceneState> pGameScene = std::make_shared<Root_GameSceneState>();

	//描画インターフェイス設定
	// add_childの中で子シーンにもセットしてるのでaddする前に呼んで！
	pGameScene->set_Renderer(&_renderer);

	// ********************************************************************************
	// シーンの子ステート作成
	// ********************************************************************************
	std::shared_ptr<c_Game_LoadProcess> c_LoadProcess = std::make_shared<c_Game_LoadProcess>();	// ゲームロード
	std::shared_ptr<c_Game_Play> c_GamePlay = std::make_shared<c_Game_Play>();					// ゲームプレイ
	std::shared_ptr<c_Game_Config> c_GameConfig = std::make_shared<c_Game_Config>();			// 設定画面

	// 親を設定
	c_LoadProcess->set_Parent(pGameScene);
	c_GamePlay->set_Parent(pGameScene);
	c_GameConfig->set_Parent(pGameScene);

	// 子を登録
	pGameScene->add_Child(c_GAME::c_GAME_LOAD, (c_LoadProcess));
	pGameScene->add_Child(c_GAME::c_GAME_PLAY, (c_GamePlay));
	pGameScene->add_Child(c_GAME::c_GAME_CONFIG, (c_GameConfig));

	// タイトルをステートマシンに登録
	_out.RegisterState(SCENE_STATE::SCENE_STATE_GAME, std::move(pGameScene));
}


//*---------------------------------------------------------------------------------------
//*【?】リザルトシーンの作成
//*
//* [引数]
//* &_out : 出力先
//* &renderer : 描画エンジンの参照
//*
//* [返値]
//* true : 成功
//* false : 失敗
//*----------------------------------------------------------------------------------------
void SceneFactory::CreateResultScene(StateMachine<SceneManager> &_out, RendererEngine &_renderer)
{
	// ルートシーン作成
	std::shared_ptr<Root_ResultSceneState> pRsultScene = std::make_shared<Root_ResultSceneState>();

	//描画インターフェイス設定
	// add_childの中で子シーンにもセットしてるのでaddする前に呼んで！
	pRsultScene->set_Renderer(&_renderer);

	// ********************************************************************************
	// シーンの子ステート作成
	// ********************************************************************************
	std::shared_ptr<c_Result_GetItemCheck> c_GetItemCheck = std::make_shared<c_Result_GetItemCheck>();	// 入手したアイテムの確認
	std::shared_ptr<c_Result_Cleared> c_Cleared = std::make_shared<c_Result_Cleared>();	// 入手したアイテムの確認
	std::shared_ptr<c_Result_Failed> c_Failed = std::make_shared<c_Result_Failed>();	// 入手したアイテムの確認

	// 親を設定
	c_GetItemCheck->set_Parent(pRsultScene);
	c_Cleared->set_Parent(pRsultScene);
	c_Failed->set_Parent(pRsultScene);

	// 子を登録
	pRsultScene->add_Child(c_RESULT::c_GETITEM_CHECK, (c_GetItemCheck));
	pRsultScene->add_Child(c_RESULT::c_CLEARED, (c_Cleared));
	pRsultScene->add_Child(c_RESULT::c_FAILED, (c_Failed));

	// タイトルをステートマシンに登録
	_out.RegisterState(SCENE_STATE::SCENE_STATE_RESULT, std::move(pRsultScene));
}
