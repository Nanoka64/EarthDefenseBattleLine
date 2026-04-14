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


	/*
	* 
	* PT : 巡回中
	* AT : 戦闘態勢
	* 
	*/

	/// <summary>
	/// アリのステート
	/// </summary>
	enum ANT_STATE
	{
		// 非アクティブ
		ANT_STATE_PATROL_IDLE,	// 待機
		ANT_STATE_PATROL_MOVE,	// 移動
		

		// アクティブ
		ANT_STATE_ACTIVE_MOVE,			// 移動
		ANT_STATE_ACTIVE_TRACKING,		// 追従
		ANT_STATE_ACTIVE_ATTACK_BITE,	// 噛みつき攻撃
		ANT_STATE_ACTIVE_ATTACK_ACID,	// 酸攻撃
		ANT_STATE_ACTIVE_DAMAGED,		// ダメージ
		ANT_STATE_ACTIVE_BLOWN_AWAY,	// 爆発による吹き飛び
		ANT_STATE_ACTIVE_FALLING,		// 落下中
		ANT_STATE_ACTIVE_DEAD,			// 死亡
	};
}