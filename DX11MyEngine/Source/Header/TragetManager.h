#pragma once
// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:TragetManager Class --- */
//
//  ★★★シングルトン★★★
//
// 【?】GameObjectManagerからいちいち検索するのは効率が悪いので、ここからアクセスできるようにする
//      
//
// ***************************************************************************************
class TragetManager
{
public:
	enum class TARGET_TYPE
	{
		PLAYER,			// プレイヤー
		GIANT_ANT_01,	// アリ01
	};


private:
	std::unordered_map<TARGET_TYPE, std::vector<std::weak_ptr<GameObject>>> m_pTragetMap;


public:
	TragetManager();
	~TragetManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// ターゲットの取得
	/// </summary>
	/// <param name="_target">ターゲットの種類</param>
	/// <returns>ターゲットオブジェクト</returns>
	std::weak_ptr<GameObject> get_Target(TARGET_TYPE& _target);

	
	/// <summary>
	/// 複数のターゲットを一括で返す
	/// </summary>
	/// <param name="_target">ターゲットの種類</param>
	/// <returns>ターゲットオブジェクト</returns>
	std::vector<std::weak_ptr<GameObject>>& get_Targets(TARGET_TYPE& _target);

	/// <summary>
	/// 読み取り専用ターゲット
	/// </summary>
	/// <returns></returns>
	const GameObject* get_TargetConst();


private:
	// コピー禁止
	TragetManager(const TragetManager&) = delete;
	TragetManager& operator=(const TragetManager&) = delete;
	// ------------------------------------------------------


};

