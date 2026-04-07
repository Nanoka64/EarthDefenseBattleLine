#pragma once
#include "IState.h"


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_IdleState Class --- */
//
//  Åöåpè≥ÅFIComponent Åö
//
// Åy?Åzë“ã@éûÉXÉeÅ[Ég
//		
// ***************************************************************************************
class Ant_IdleState : public IState<class EnemyController>
{
private:



public:
	void OnEnter(class EnemyController* pOwner) override;
	void OnExit(class EnemyController* pOwner)override;
	int Update(class EnemyController* pOwner)override;

};



// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Ant_TrackingState Class --- */
//
//  Åöåpè≥ÅFIComponent Åö
//
// Åy?Åzí«è]ÉXÉeÅ[Ég
//		
// ***************************************************************************************
class Ant_TrackingState : public IState<class EnemyController>
{
private:

public:
	void OnEnter(class EnemyController *pOwner) override;
	void OnExit(class EnemyController *pOwner)override;
	int Update(class EnemyController *pOwner)override;
};

