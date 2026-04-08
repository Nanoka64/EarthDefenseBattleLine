#pragma once

/// <summary>
/// 汎用データ
/// </summary>
namespace UtilityData
{
	/// <summary>
	/// 派閥
	/// </summary>
	enum class FACTION
	{
		PLAYER,		// プレイヤー
		ALLY,		// 味方
		ENEMY,		// 敵
		NEUTRAL,	// 中立
		VEHICLE,	// 乗り物
	};

	struct FactionInfo
	{
		FACTION _faction;
	};

	/// <summary>
	/// 兵科
	/// </summary>
	enum class SOLDIER_TYPE
	{
		RANGER,	// レンジャー

		// WING,
		// AIR,
		// F,
	};




};