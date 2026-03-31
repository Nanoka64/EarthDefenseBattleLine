#include "pch.h"
#include "c_Title_SoldierSelect.h"
#include "Root_TitleSceneState.h"
#include "GameObjectManager.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "Component_ButtonUI.h"
#include "InputFactory.h"
#include "GameObject.h"

using namespace Tool;
using namespace VECTOR2;
using namespace VECTOR3;

// 項目の衝突判定用サイズ
static const VECTOR2::VEC2 g_ItemSize = VECTOR2::VEC2(500.0f, 140.0f);

// 項目の位置
static const VECTOR2::VEC2 g_ItemPosArray[UINT_CAST(SOLDIER_TYPE::NUM)] =
{
	//VEC2( 400.0f,600.0f),
	//VEC2(1000.0f,600.0f),
	//VEC2( 400.0f,800.0f),
	//VEC2(1000.0f,800.0f),
	VEC2(400.0f,500.0f),
	VEC2(400.0f,640.0f),
	VEC2(400.0f,780.0f),
	VEC2(400.0f,920.0f),
};

/// <summary>
/// 兵科名
/// </summary>
static constexpr const char* g_SoldierNames[UINT_CAST(SOLDIER_TYPE::NUM)] =
{
	//"陸戦歩兵",
	//"...",
	//"...",
	//"...",

	"スタンダード",
	"ラピッド",
	"スカウト",
	"ヘビー",
};

/// <summary>
/// 兵装説明
/// </summary>
static const char* g_WeaponDescriptions[UINT_CAST(SOLDIER_TYPE::NUM)] =
{
	"・バランスの取れた兵装\n特にクセもなく、初心者におすすめだ。",
	"・連射性能に優れた兵装\n攻撃力は低めだが、とにかく撃ちまくって数で圧倒しろ！",
	"・中/遠距離に特化した兵装\n連射性能は抑え目だが、中距離以上からの射撃に長けているぞ。",
	"・高火力兵装\n扱いは難しいが、火力は群を抜いて高い。まさにロマン砲！",
};

/// <summary>
/// マウスカーソルが項目の上に乗った際に、項目をどれくらいずらすか
/// </summary>
static const float g_MouseHoveredItemSlideOffset = 60.0f;


using namespace SceneStateEnums;

//*---------------------------------------------------------------------------------------
//* @:c_Title_SoldierSelect Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_SoldierSelect::OnEnter(SceneManager *pOwner)
{
	m_NextState = c_TITLE_SOLDIER_SELECT;

	// メニュー項目のオブジェクトとコンポーネントの取得と設定
	for (int i = 0; i < INT_CAST(SOLDIER_TYPE::NUM); i++)
	{
		UIData::RectTransformData rectTrans;
		UIData::ButtonUIData buttonData;
		buttonData._imagePath = "Resource/Texture/Title/25260264.png";
		buttonData._text = g_SoldierNames[i];
		buttonData._layerRank = 105;
		buttonData._inputValidationState = UIData::STATE::SELECTED;
		buttonData._textOffsetPos = VEC2(80.0f, 50.0f);
		buttonData._onClicFunction = // 武器の選択
			[this, i]() {
				Master::m_pDataManager->set_SelectWeaponID(m_CrntSelectItem * 2, 0);
				Master::m_pDataManager->set_SelectWeaponID(m_CrntSelectItem * 2 + 1, 1);
				m_DecisionTextDrawCounter = 120;	// 装備決定のテキストを表示するためのカウンターをセット
				m_DecisionSoldierTypeIndex = m_CrntSelectItem;	// 決定した兵科のインデックスをセット
			};
		rectTrans._size = g_ItemSize;
		rectTrans._pos = g_ItemPosArray[i];
		m_pButtonsObjArray[i] = Master::m_pUIManager->GetButton(*m_pRenderer, rectTrans, buttonData);
		m_pButtonArray[i] = m_pButtonsObjArray[i]->get_Component<ButtonUI>();
		m_pMenuItemRectTransformArray[i] = m_pButtonsObjArray[i]->get_RectTransform();

		m_ItemInfoArray[i]._pos = g_ItemPosArray[i];
		m_ItemInfoArray[i]._name = g_SoldierNames[i];
	}

	UIData::RectTransformData rectData;
	rectData._size = VEC2(1000.0f, 600.0f);
	rectData._pos = VEC2(850.0f, 500.0f);
	UIData::SpriteUIData spriteData;
	spriteData._tag = "WeaponDescriptionbackSprite";
	spriteData._imagePath = "Resource/Texture/Title/24675532.png";
	spriteData._layerRank = 110;
	m_pWeaponDescriptionbackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);

	m_CrntSelectItem = 0;

	m_IsInit = true;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_SoldierSelect Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_SoldierSelect::OnExit(SceneManager *pOwner)
{
	// オブジェクトを非アクティブに
	for (int i = 0; i < UINT_CAST(SOLDIER_TYPE::NUM); i++)
	{
		m_pButtonsObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
	}

	m_pWeaponDescriptionbackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_SoldierSelect Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Title_SoldierSelect::Update(SceneManager *pOwner)
{
	// 右クリックでメインメニューへ戻る
	if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT))
	{
		return c_TITLE::c_TITLE_MAIN_MENU;
	}



	POINT mousePos = Master::m_pInputManager->GetMousePos();	// マウス座標

	//m_NextState = c_TITLE_SOLDIER_SELECT;

	int i = 0;
	for (auto &button : m_pButtonArray)
	{
		UIData::STATE state = button.lock()->get_State();;
			

		m_ItemInfoArray[i]._isHovered = false;

		switch (state)
		{
		case UIData::STATE::NORMAL:
			break;
		case UIData::STATE::HIGH_LIGHTED:
		{
			if (m_CrntSelectItem != i) {
				// ****************************************************
				//				カーソルが載った時のSE再生
				// ****************************************************
				Master::m_pSoundManager->Play(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::SYSTEM_MOVING_CURSOR01));
			}
			m_ItemInfoArray[i]._isHovered = true;	// マウスが乗ってる
			m_CrntSelectItem = i;					// なんの項目に乗ったか保持
		}
			break;
		case UIData::STATE::PRESSED:
			m_ItemInfoArray[i]._isHovered = true;	
			m_CrntSelectItem = i;

			break;
		case UIData::STATE::SELECTED:
			break;
		case UIData::STATE::DISABLED:
			break;
		default:
			break;
		}

		i++;
	}

	return m_NextState;
	return c_TITLE::c_TITLE_SOLDIER_SELECT;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_SoldierSelect Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_SoldierSelect::Draw(SceneManager *pOwner)
{
	int i = 0;

	for (auto& item : m_ItemInfoArray)
	{
		// 項目の位置
		VEC2 menuItemPos = item._pos;

		// スプライトの位置（文字の少し左側になるよう補正している）
		VEC2 spritePos;
		spritePos.x = menuItemPos.x - 100.0f;
		spritePos.y = menuItemPos.y;


		// マウスが乗っている項目は少しずらす
		if (m_CrntSelectItem == i)
		{
			// ずらす
			menuItemPos.x += g_MouseHoveredItemSlideOffset;
			spritePos.x += g_MouseHoveredItemSlideOffset;
		}

		m_pMenuItemRectTransformArray[i].lock()->set_RectPosition(VEC2(spritePos.x, spritePos.y));
											  
		// 文字表示
		//Master::m_pDirectWriteManager->DrawString(item._name, menuItemPos, "White_40_STD");

		i++;
	}

	auto DrawWeaponInfo = [](const WeaponData::GunWeaponData* weaponData, int weaponIndex, float startY) {
		if (!weaponData) return;

		// 基底データ（全弾種共通のパラメータ）の取得
		const auto& baseBulletData = std::visit([](const auto& arg) -> const BulletData::NormalBulletData& {
			return arg;
			}, weaponData->_bulletParam);

		// 弾種ごとの固有パラメータを取得して文字列化
		std::wstring extraInfoStr = L"";
		std::visit([&extraInfoStr](const auto& arg) {
			using T = std::decay_t<decltype(arg)>; // 型を推論

			// 爆発弾の場合のみ、爆発半径の文字列を追加する
			if constexpr (std::is_same_v<T, BulletData::ExplosionBulletData>) {
				extraInfoStr = L"　・爆発半径:" + FormatFloat(arg._explosionRadius, 1) + L"m";
			}
			// 将来的にショットガン（散弾）などの型が増えたら、ここに else if constexpr を足すだけで対応可能
			}, weaponData->_bulletParam);

		// 武器名の描画
		std::wstring weaponInfoStr = L"武器" + std::to_wstring(weaponIndex) + L":" + weaponData->_name;
		Master::m_pDirectWriteManager->DrawString(weaponInfoStr, VEC2(950.0f, startY), "White_30_STD");

		// 各種計算・変換
		std::wstring laserSightStr = weaponData->_isLaserSight ? L"装備" : L"---";
		float fireRatePerSec = (weaponData->_fireRate > 0.0f) ? (60.0f / weaponData->_fireRate) : 0.0f;
		std::wstring damageStr = FormatFloat((baseBulletData._damage));
		damageStr = (weaponData->_bulletSimultaneousNum > 1) ? damageStr + L" x " + std::to_wstring(weaponData->_bulletSimultaneousNum) : damageStr; // 同時発射数が1より多い場合はダメージに「x N」を追加


		// 情報テキストの構築（追加パラメータ extraInfoStr を結合）
		std::wstring detailStr =
			L"　・弾数:" + std::to_wstring(weaponData->_bulletMaxNum) +
			L"　・連射速度（発/sec）:" + FormatFloat(fireRatePerSec) +
			L"　・ダメージ:" + damageStr +
			L"\n" +
			L"　・リロード:" + FormatFloat(weaponData->_reloadTime) + L"秒" +
			L"　・射程:" + FormatFloat(baseBulletData._range / 10.0f) + L"m" +
			L"　・弾速:" + FormatFloat(baseBulletData._speed) +
			L"\n" +
			L"　・精度:" + FormatFloat(weaponData->_accuracy,2) + // ※ここを「S/A/B...」等に変換する関数を噛ませると綺麗です
			L"　・ズーム（倍）:" + FormatFloat(weaponData->_zoomLength) +
			L"　・レーザーサイト:" + laserSightStr +
			L"\n" + 
			L"　・敵貫通可能数:" + std::to_wstring(baseBulletData._penetrationsCount) +
			extraInfoStr; // ★ 爆発弾の時だけここに「爆発半径」が足される

		// 6. パラメータの描画
		Master::m_pDirectWriteManager->DrawString(detailStr, VEC2(950.0f, startY + 50.0f), "White_20_STD", D2D1_DRAW_TEXT_OPTIONS_NONE, false);
		};

	// 描画の実行（現在選択中の武器）
	auto weaponData_1 = static_cast<const WeaponData::GunWeaponData*>(Master::m_pWeaponDataManager->FindWeaponData(m_CrntSelectItem * 2));
	auto weaponData_2 = static_cast<const WeaponData::GunWeaponData*>(Master::m_pWeaponDataManager->FindWeaponData(m_CrntSelectItem * 2 + 1));
	Master::m_pDirectWriteManager->DrawString(g_WeaponDescriptions[m_CrntSelectItem], VEC2(950.0f, 550.0f), "White_20_STD");

	DrawWeaponInfo(weaponData_1, 1, 620.0f);
	DrawWeaponInfo(weaponData_2, 2, 800.0f);


	// 装備を決定したときのテキスト表示 ************************************************************************
	if (m_DecisionTextDrawCounter > 0) {
		std::string decisionText = g_SoldierNames[m_DecisionSoldierTypeIndex];
		float textY = m_ItemInfoArray[m_DecisionSoldierTypeIndex]._pos.y;
		float textX = m_ItemInfoArray[m_DecisionSoldierTypeIndex]._pos.x - 150.0f;
		Master::m_pDirectWriteManager->DrawString("[" + decisionText + "]" + "を装備しました", VEC2(textX, textY), "White_30_STD");

		m_DecisionTextDrawCounter--;
	}

	Master::m_pDirectWriteManager->DrawString("☆兵装選択", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");

}
