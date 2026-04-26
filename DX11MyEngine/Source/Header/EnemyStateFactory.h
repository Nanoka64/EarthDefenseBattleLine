#pragma once
#include "ConstantEnemyData.h"
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:EnemyStateFactory Class --- */
//
// 【?】敵のステートの作成を行う
//
// ***************************************************************************************
class EnemyStateFactory
{
private:

public:
	/// <summary>
	/// シーンステートを作成して挿入する
	/// </summary>
	/// <param name="_out">出力先</param>
	/// <param name="_createScene">作成するシーン</param>
	/// <param name="_renderer">描画エンジンの参照</param>
	static void Create(StateMachine<class EnemyController>& _out, int _createState, RendererEngine& _renderer);

private:
	/// <summary>
	/// 蟻の作成
	/// </summary>
	/// <param name="_out">出力先</param>
	/// <param name="_renderer">描画エンジンの参照</param>
	static void CreateAntState(StateMachine<class EnemyController>& _out, RendererEngine& _renderer);
};

