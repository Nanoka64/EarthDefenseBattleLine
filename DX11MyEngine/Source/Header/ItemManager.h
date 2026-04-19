#pragma once

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:ItemManager Class --- */
//
//  ★★★シングルトン★★★
//
// 【?】プールで持つアイテムの管理をする
//      
//
// ***************************************************************************************
class ItemManager
{
private:
	std::unique_ptr<ObjectPool<GameObject>> m_pItemObjectPool;			// アイテムプール
	std::vector<GameObject* > m_ExtractedItemObject;	// 取り出したアイテムオブジェクトのvector

public:
	ItemManager();
	~ItemManager();

    /// <summary>
	/// 初期化
	/// </summary>
	/// <param name="renderer">描画エンジンの参照</param>
	/// <returns>成功/失敗</returns>
    bool Init(RendererEngine& renderer);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    void Update(RendererEngine& renderer);

	/// <summary>
	/// アイテムをスポーンさせる
	/// </summary>
	/// <param name="_type">種類</param>
	/// <param name="_pos">位置</param>
	void SpawnItem(UtilityData::ITEM_TYPE _type, const VECTOR3::VEC3& _pos);

	/// <summary>
	/// アイテムをランダムにスポーンさせる
	/// </summary>
	/// <param name="_type">種類</param>
	/// <param name="_pos">位置</param>
	void SpawnItemRand(int _minNum, int _maxNum, const VECTOR3::VEC3& _pos, float _radiuse);

private:
	// コピー禁止
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;
	// ------------------------------------------------------

};

