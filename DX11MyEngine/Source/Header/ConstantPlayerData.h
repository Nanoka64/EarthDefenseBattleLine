#pragma once

namespace PlayerData
{
	/// <summary>
	/// アニメーションID -レンジャー-
	/// </summary>
	enum class PLAYER_RANGER_ANIM_ID
	{
		NONE = -1,

		JUMP_DOWN,
		JUMP_LOOP,
		JUMP_UP,

		RIFLE_AMING_IDLE,	
		RELOADING_IDLE,
		RUNING,
		RUNING_DIVE_ROLL,
		RUNNNING_RELOAD,
		WALKING,
	};

	const std::string g_PlayerAnimationNames[] =
	{
		"JUMP_DOWN",
		"JUMP_LOOP",
		"JUMP_UP",
		"RIFLE_AMING_IDLE",
		"RELOADING_IDLE",
		"RUNING",
		"RUNING_DIVE_ROLL",
		"RUNNNING_RELOAD",
		"WALKING",
	};




};