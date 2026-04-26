#pragma once
#include "IComponent.h"
#include "ConstantUtilityData.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Faction Class --- */
//
//  šŒp³FIComponent š
//
// y?z”h”´‚ğŠÇ—‚·‚é
//		
// ***************************************************************************************
class Faction : public IComponent
{
private:
	UtilityData::FactionInfo m_FactionInfo;	// ”h”´î•ñ


public:
	Faction(std::weak_ptr<GameObject> pOwner, int updateRank);
	~Faction();

	const UtilityData::FactionInfo& get_FactionInfo()const { return m_FactionInfo; }	// ”h”´î•ñ‚Ìæ“¾
	void set_Faction(const UtilityData::FACTION& f) { m_FactionInfo._faction = f; }		// ”h”´‚Ìİ’è
	UtilityData::FACTION get_Faction()const { return m_FactionInfo._faction; }			// ”h”´‚Ìæ“¾
};

