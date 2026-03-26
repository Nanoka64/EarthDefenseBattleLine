#pragma once
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponStateFactory Class --- */
//
// 【?】武器のステートの作成を行う
//
// ***************************************************************************************
class WeaponStateFactory
{
private:

public:
	/// <summary>
	/// ステートを作成して挿入する
	/// </summary>
	/// <param name="_out">出力先</param>
	/// <param name="_createScene">作成する武器ステート</param>
	/// <param name="_renderer">描画エンジンの参照</param>
	static void CreateGunWeapon(StateMachine<class GunWeapon>& _out, int _createWeapon, RendererEngine& _renderer);

private:
};

