#pragma once

namespace EnemyData
{
	/// <summary>
	/// 敵の種類
	/// </summary>
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_ANT_Normal,

		ENEMY_TYPE_NUM,
	};

	/// <summary>
	/// アリのステート
	/// </summary>
	enum ANT_STATE
	{
		ANT_STATE_IDLE,			// 待機
		ANT_STATE_TRACKING,		// 追従
		ANT_STATE_ATTACK_BITE,	// 噛みつき攻撃
		ANT_STATE_ATTACK_ACID,	// 酸攻撃
		ANT_STATE_ESCAPE,		// 逃げる

		ANT_STATE_NUM,
	};


}