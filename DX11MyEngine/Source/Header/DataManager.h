#pragma once


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:DataManager Class --- */
//
//  ★★★シングルトン★★★
//
// 【?】データ管理を行う
// 
//	※ なぜ作ったか
//		RendererEngineクラス がコンポーネントなどのエンジン、ゲーム問わず、様々な場所で参照をしデータの
//		取得などを行う中継器的な設計になってしまっている。
//		このクラスはRenderEngineの中継器的な干渉を切り離す為に作った。
//		例えば今は、RenderEngineからスクリーンの幅などを取得しているが、それをこのクラスを通して取得することで、
//		過多な干渉を減らす。
//
// ***************************************************************************************
class DataManager
{
private:
	float m_ScreenWidth;
	float m_ScreenHeight;
	float m_DefaultFov;	// デフォルトのFOV
	bool m_IsDebugMode;
	std::shared_ptr<class RendererEngine> m_pRenderer;// 描画エンジンのポインタ
	int m_SelectWeaponID[2];	// 武器選択で選択した武器のID 一時的にここに置く
	UtilityData::UserConfigData m_UserConfigData;	// ユーザ設定データ


public:
	DataManager();
	~DataManager();

	bool Init(std::shared_ptr<RendererEngine>pRenderer);

	bool SettingsData_TitleLoadOnEnter(RendererEngine& renderer);			// タイトルロード時
	bool SettingsData_TitleLoadOnExit(RendererEngine& renderer);			// タイトルロード時
	bool SettingsData_MissionLoad(RendererEngine& renderer, UINT _missionNumber);			// ミッション開始時
	bool SettingsData_MissionTermination(RendererEngine& renderer, UINT _missionNumber);	// ミッション終了時

	void set_ScreenWidth(float _w) { m_ScreenWidth = _w; };			// スクリーンの横幅を設定（基本的にRendererEngine以外からは設定しない）
	void set_ScreenHeight(float _h) { m_ScreenHeight = _h; };		// スクリーンの縦幅を設定（基本的にRendererEngine以外からは設定しない）
	void set_DefaultFov(float _f) { m_DefaultFov = _f; }			// デフォルトFOVの設定
	void set_IdDebugMode(bool flag) { m_IsDebugMode = flag; }		// デバッグ用エディタのフラグ設定
	void set_UserConfigData(const UtilityData::UserConfigData &_data) { m_UserConfigData = _data; }	// ユーザ設定データの設定

	float get_ScreenWidth() const { return m_ScreenWidth; };		// スクリーンの横幅を取得
	float get_ScreenHeight() const { return m_ScreenHeight; }		// スクリーンの縦幅を取得
	float get_DefaultFov()const { return m_DefaultFov; }			// デフォルトFOVを取得
	bool get_IsDebugMode()const { return m_IsDebugMode; }			// デバッグ用エディタのフラグ取得
	const UtilityData::UserConfigData& get_UserConfigData() const { return m_UserConfigData; }	// ユーザ設定データの取得

	void set_Fov(float _fov);	// FOVの設定
	float get_Fov();			// FOVの取得

	void set_SelectWeaponID(int _id, int _slot) { m_SelectWeaponID[_slot] = _id; }	// 武器選択で選択した武器のIDを設定
	int get_SelectWeaponID( int _slot) { return m_SelectWeaponID[_slot]; }	// 武器選択で選択した武器のIDを取得

private:
	// コピー禁止
	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;
	// ------------------------------------------------------
};

