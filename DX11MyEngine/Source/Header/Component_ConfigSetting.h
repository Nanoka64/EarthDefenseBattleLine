#pragma once
#include "IComponent.h"

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:ConfigSetting Class --- */
//
//  ★継承：IComponent ★
//
// 【?】設定画面の処理
//		
// ***************************************************************************************
class ConfigSetting :  public IComponent
{
private:
		/* 定数 */
	const VECTOR2::VEC2 CONFIG_ITEM_SIZE		 = VECTOR2::VEC2(500.0f, 35.0f);			// 項目のサイズ
	const VECTOR2::VEC2 CONFIG_ITEM_BUTTON_SIZE	 = VECTOR2::VEC2(50.0f, 35.0f);				// ボタンのサイズ
	const VECTOR4::VEC4 CONFIG_ITEM_COLOR		 = VECTOR4::VEC4(0.3f, 0.3f, 0.7f, 1.0f);	// 
	const VECTOR2::VEC2 BACK_SPRITE_SIZE		 = VECTOR2::VEC2(700.0f, 500.0f);			// 最奥背景スプライトのサイズ
	const VECTOR2::VEC2 BACK_SPRITE_POS			 = VECTOR2::VEC2(550.0f, 500.0f);			// 最奥背景スプライトの位置
	const VECTOR2::VEC2 CONFIG_VALUE_OFFSET_SIZE = VECTOR2::VEC2(250.0f, 70.0f);			// 項目の値を表示するテキストの位置は、項目の位置にこのオフセットを足した位置になるようにする
	const VECTOR2::VEC2 CONFIG_ITEM_START_POS	 = VECTOR2::VEC2(650.0f, 550.0f);			// 項目の開始位置
	const float ITEM_POS_Y_BETWEEN_DIST	 = 70.0f;											// 項目同士のY距離
	const float BUTTON_OFFSET_POS_X				 = 350.0f;									// ボタンの位置（項目名の右側に配置）
	const float BUTTON_POS_X_BETWEEM_DIST		 = 100.0f;									// ボタン同士の間の距離

	VECTOR2::VEC2 m_ItemStartPos;															// 項目の開始位置

	class GameObject* m_pConfigBackSpriteObj;																		// 設定画面の背景スプライトオブジェクト
	class GameObject* m_pConfigSelectItemSpriteObj;																	// 選択した項目につけるスプライト
	class GameObject* m_pConfigAdjustmentButtonObjArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)][2];				// 設定項目の調整カーソルのオブジェクト[0]が左、[1]が右
	std::weak_ptr<class ButtonUI> m_pButtonArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];							// ButtonUI配列  
	std::weak_ptr<class RectTransform> m_pMenuItemRectTransformArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];		// RectTransform配列
	UtilityData::ConfigMenuItemInfo m_ItemInfoArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];						// 項目の情報配列
	UtilityData::ConfigMenuItemInfo m_DefaultConfigItemInfoArray[UINT_CAST(UtilityData::CONFIG_ITEM::NUM)];			// 設定情報を入れる受け皿
	float m_Timer;

public:
	ConfigSetting(std::weak_ptr<GameObject> pOwner, int updateRank);
	~ConfigSetting();

	void Start(RendererEngine& renderer) override;	// 初期化
	void Update(RendererEngine& renderer) override;	// 更新
	void Draw(RendererEngine& renderer) override;	// 更新

	void set_ItemStartPos(const VECTOR2::VEC2& _pos) { m_ItemStartPos = _pos; }	// 項目の開始位置設定

	void ChangeConfigValue(UtilityData::CONFIG_ITEM _item, bool _isLeftIndex);		// 設定値を変更する
	void BorrowFromUIPoolAndSetup(RendererEngine& renderer);						// プールからスプライトを取り出し、初期化
	void ReleaseToUIPool();															// プールへ返す

};

