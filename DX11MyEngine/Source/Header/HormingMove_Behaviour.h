#pragma once
#include "IMoveBehaviour.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:HormingMove_Behaviour Class --- */
//
//  šŒp³FIMoveBehaviour š
//
// y?z’Ç]‹““®
//
// ***************************************************************************************
class HormingMove_Behaviour : public IMoveBehaviour
{
private:

public:
	HormingMove_Behaviour();
	~HormingMove_Behaviour();

	ResultMove MoveCalculate(float _deltaTime, const MoveParam& _param, const class MyTransform& _transform) override;	// ˆÚ“®ŒvZ

};

