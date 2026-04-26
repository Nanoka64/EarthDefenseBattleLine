#include "pch.h"
#include "Component_Faction.h"

Faction::Faction(std::weak_ptr<GameObject> pOwner, int updateRank) : IComponent(pOwner, updateRank)
{
	this->set_Tag("Faction");
}

Faction::~Faction()
{
}