#include "pch.h"
#include "Component_WeaponBase.h"

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//* [引数]
//* pOwner : オーナーオブジェクト
//* updateRank : 更新レイヤー
//*----------------------------------------------------------------------------------------
WeaponBase::WeaponBase(std::weak_ptr<GameObject> pOwner, int updateRank )
	: IComponent(pOwner, updateRank),
	m_WeaponFlags(),
	m_pWeaponData(nullptr)
{

}