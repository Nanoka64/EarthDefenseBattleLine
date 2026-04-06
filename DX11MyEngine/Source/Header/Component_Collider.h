#pragma once
#include "IComponent.h"

enum class COLLIDER_TYPE
{
	NONE,
	BOX,
	SPHERE,
	RAY,
};

/// <summary>
/// 衝突判定のビット分け
/// </summary>
enum class COLLISION_CATEGORY : unsigned
{
	NONE = 0,
	PLAYER				 = 1 << 0,	// プレイヤー
	PLAYER_BULLET		 = 1 << 1,	// プレイヤーの弾
	ENEMY				 = 1 << 2,	// 敵
	ENEMY_BULLET		 = 1 << 3,	// 敵の弾
	DESTRUCTION_BUILDING = 1 << 4,	// 破壊可能な建物
	BUILDING			 = 1 << 5,	// 破壊不可能な建物

	EVERY = 0xFFFFFFFF	// 全てに衝突
};

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:Collider Class --- */
//
//  ★ 継承 ★
//
// 【?】コライダーコンポーネントの基底クラス
//
// ***************************************************************************************
class Collider : public IComponent
{
protected:
	bool m_IsEnable;				// 使用するかどうか
	bool m_IsTrigger;				// 衝突判定のみ取るかどうか（falseなら物理も判定をする）
	bool m_IsHit;					// 現在衝突しているかどうか
	bool m_IsStatic;				// 静的かどうか（建物など動かないもの）
	VECTOR3::VEC3 m_Center;			// コライダーの中心位置
	COLLIDER_TYPE m_ColliderType;	// コライダーの種類
	std::weak_ptr<class MyTransform> m_pTransform;


	COLLISION_CATEGORY m_CategoryBits;	// 衝突判定を分けるためのカテゴリー
	unsigned m_CollisionBitMask;		// 衝突判定を分けるためのビットマスク

	bool m_IsDrawDebugMesh;				// デバッグ用メッシュを表示するか

public:
	Collider(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
	~Collider();

	// 使用フラグ
	void set_IsEnable(bool _flag) { m_IsEnable = _flag; }
	bool get_IsEnable()const { return m_IsEnable; }

	// 物理判定フラグ
	void set_IsTrigger(bool _flag) { m_IsTrigger = _flag; }
	bool get_IsTrigger()const { return m_IsTrigger; }

	// 中心位置
	void set_Center(const VECTOR3::VEC3 &_vIn) { m_Center = _vIn; }
	VECTOR3::VEC3 get_Center()const { return m_Center; }

	COLLIDER_TYPE get_ColliderType()const { return m_ColliderType; }

	std::weak_ptr<class MyTransform>get_Transform() { return m_pTransform; };

	/* 衝突したかどうか */
	void set_IsHit(bool _flag) { m_IsHit = _flag; }
	bool get_IsHit()const { return m_IsHit; }

	/* 静的かどうか */
	void set_IsStatic(bool _flag) { m_IsStatic = _flag; }
	bool get_IsStatic()const { return m_IsStatic; }

	/* デバッグメッシュ表示するか */
	void set_IsDrawDebugMesh(bool _flag) { m_IsDrawDebugMesh = _flag; }
	bool get_IsDrawDebugMesh()const { return m_IsDrawDebugMesh; }


	/* 衝突のカテゴリ 自身のタイプ */
	void set_CollisionCategory(COLLISION_CATEGORY _category) { m_CategoryBits = _category; }
	COLLISION_CATEGORY get_CollisionCategory()const { return m_CategoryBits; }

	/* 衝突判定のビットマスク */
	// 一括設定
	void set_CollisionBitMask(unsigned _mask) { m_CollisionBitMask = _mask; };
	// 特定のカテゴリを追加
	void add_CollisionBitMask(COLLISION_CATEGORY _category) {GIGA_Engine::BitFlag::SetFlag(static_cast<unsigned>(_category), m_CollisionBitMask);}
	// 特定のカテゴリを除外
	void remove_CollisionBitMask(COLLISION_CATEGORY _category) { GIGA_Engine::BitFlag::UnsetFlag(static_cast<unsigned>(_category), m_CollisionBitMask); }
	unsigned get_CollisionBitMask()const { return m_CollisionBitMask; }
};

