#pragma once
#include "IComponent.h"
#include "ConstantWeaponData.h"

enum class WEAPON_STATUS : unsigned
{
	IDLE = 1 << 0,
	FIRE = 1 << 1,
	RELODING = 1 << 2,

};

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
protected:
	GIGA_Engine::BitFlag::BIT_FLAG<unsigned int> m_WeaponFlags;
	const WeaponData::BaseWeaponData *m_pParameter;

public:
	WeaponBase(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	virtual ~WeaponBase() = default;

	GIGA_Engine::BitFlag::BIT_FLAG<unsigned int>& get_WeaponFlags() { return m_WeaponFlags; }
	const GIGA_Engine::BitFlag::BIT_FLAG<unsigned int>& get_WeaponFlags() const { return m_WeaponFlags; }

	virtual bool Setup(const WeaponData::BaseWeaponData* _pWeaponData) = 0;
	virtual void SwicthReset() = 0;	// 武器切り替え時のリセット
};