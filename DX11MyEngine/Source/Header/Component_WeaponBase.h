#pragma once
#include "IComponent.h"
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponBase Class --- */
//
//  ★継承：IComponent ★
//
// 【?】武器の基底コンポーネント
//		
// ***************************************************************************************
class WeaponBase : public IComponent
{
private:

public:
	WeaponBase(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	virtual ~WeaponBase() = default;
};