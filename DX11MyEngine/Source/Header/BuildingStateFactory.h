#pragma once

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponStateFactory Class --- */
//
// 【?】武器のステートの作成を行う
//
// ***************************************************************************************
class BuildingStateFactory
{
private:

public:
	/// <summary>
	/// ステートを作成して挿入する
	/// </summary>
	/// <param name="_out">出力先</param>
	/// <param name="_createScene">作成する建物ステート</param>
	/// <param name="_renderer">描画エンジンの参照</param>
	static void CreateBuilding(StateMachine<class BuildingController>& _out, int _createState, RendererEngine& _renderer);

private:
};

