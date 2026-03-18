#pragma once

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:UIManager Class --- */
//
//  ★★★シングルトン★★★
//
// 【?】UIの管理を行う
//
// ***************************************************************************************
class UIManager
{
private:
	std::unique_ptr<ObjectPool<GameObject>> m_pUIPool;

public:
	UIManager();
	~UIManager();

	bool Init();
	void Update();
private:
	// コピー禁止
	UIManager(const UIManager &) = delete;
	UIManager &operator=(const UIManager &) = delete;
	// ------------------------------------------------------
};

