#include "pch.h"
#include "Component_WeaponController.h"
#include "Component_GunWeapon.h"
#include "Component_WeaponBase.h"

using namespace VECTOR3;
using namespace VECTOR2;

constexpr int WEAPON_CHANGE_INTERVAL = 10;	// 武器の切り替え間隔

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//*----------------------------------------------------------------------------------------
WeaponController::WeaponController(std::weak_ptr<GameObject> pOwner, int updateRank):
	IComponent(pOwner, updateRank),
	m_CrntWeaponSlotIndex(-1),
	m_MaxSlot(-1),
	m_WeaponChangeIntervalCounter(0)
{
	this->set_Tag("WeaponController");
}

//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
WeaponController::~WeaponController()
{
	m_WeaponArray.clear();
}


//*---------------------------------------------------------------------------------------
//*【?】初期化処理
//*
//* [引数]
//* renderer : 描画エンジンの参照
//*_maxSlot  : 最大スロット数
//* 
//* 
//* [返値] なし
//*----------------------------------------------------------------------------------------
bool WeaponController::Setup(RendererEngine& renderer, int _maxSlot)
{
	m_MaxSlot = _maxSlot;
	
	// 最大数分メモリ確保
	m_WeaponArray.reserve(m_MaxSlot);

	return true;
}


//*---------------------------------------------------------------------------------------
//*【?】更新処理
//*
//* [引数]
//* renderer : 描画エンジンの参照
//*
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::Update(RendererEngine& renderer)
{
	if (m_WeaponChangeIntervalCounter != 0)
	{
		m_WeaponChangeIntervalCounter--;
		return;
	}

	// 武器１
	if (GetInputDown(GAME_CONFIG::WEAPON_CHANGE1))
	{
		SwitchWeapon(0);
	}
	// 武器２
	if (GetInputDown(GAME_CONFIG::WEAPON_CHANGE2))
	{
		SwitchWeapon(1);
	}

	// マウスホイールで武器切り替え
	LONG scroll = Master::m_pInputManager->GetMousePosSlopeZ();
	if (scroll > 0)
	{
		NextWeapon();
	}
	else if (scroll < 0)
	{
		PrevWeapon();
	}
}

//*---------------------------------------------------------------------------------------
//*【?】描画処理
//*
//* [引数]
//* renderer : 描画エンジンの参照
//*
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::Draw(RendererEngine& renderer)
{
	int ammoMax = 0, ammoRemaining = 0;	
	m_WeaponArray[m_CrntWeaponSlotIndex];


	//Master::m_pDirectWriteManager->DrawFormatString("残りの敵数：{:d}", VECTOR2::VEC2(0, 540), "White_40_STD", m_EnemyNum);
	//Master::m_pDirectWriteManager->DrawFormatString("残りの敵数：{:d}", VECTOR2::VEC2(0, 540), "White_40_STD", m_EnemyNum);
	//Master::m_pDirectWriteManager->DrawFormatString("残りの敵数：{:d}", VECTOR2::VEC2(0, 540), "White_40_STD", m_EnemyNum);

}


//*---------------------------------------------------------------------------------------
//*【?】 現在装備している武器がリロード中か
//*
//* [引数] なし
//* [返値] なし
//*----------------------------------------------------------------------------------------
bool WeaponController::get_IsCrntWeaponReloading()
{
	return m_WeaponArray[m_CrntWeaponSlotIndex]->get_WeaponFlags().GetFlag(WEAPON_STATUS::RELODING);
}


//*---------------------------------------------------------------------------------------
//*【?】武器の切り替え
//*
//* [引数]
//* _index : 切り替え先の武器のインデクス
//*
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::SwitchWeapon(int _index)
{
	if (_index < 0 || _index >= (int)m_WeaponArray.size() || _index == m_CrntWeaponSlotIndex) return;

	// 現在の武器がセットされていない
	if (m_CrntWeaponSlotIndex < 0)
	{
		return;
	}

	//*****************************************************************************************
	//						武器の切り替え音再生
	//*****************************************************************************************
	Master::m_pSoundManager->Play_RandPitch(SOUND_TYPE::SE, INT_CAST(SOUND_ID::GUN_CHANGE01), 200);

	// 切り替え時のリセット
	m_WeaponArray[m_CrntWeaponSlotIndex]->SwicthReset(); 
	
	// 現在の武器を非表示に
	m_WeaponArray[m_CrntWeaponSlotIndex]->get_OwnerObj().lock()->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

	// 新しい武器を表示
	m_CrntWeaponSlotIndex = _index;
	m_WeaponArray[m_CrntWeaponSlotIndex]->get_OwnerObj().lock()->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

	m_WeaponChangeIntervalCounter = WEAPON_CHANGE_INTERVAL;
}

//*---------------------------------------------------------------------------------------
//*【?】次の武器へ切り替え
//*
//* [引数] なし
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::NextWeapon()
{
	int nextIdx = (m_CrntWeaponSlotIndex + 1) % (int)m_WeaponArray.size();
	SwitchWeapon(nextIdx);
}

//*---------------------------------------------------------------------------------------
//*【?】前の武器へ切り替え
//*
//* [引数] なし
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::PrevWeapon()
{
	int count = (int)m_WeaponArray.size();
	int prevIdx = (m_CrntWeaponSlotIndex - 1 + count) % count;
	SwitchWeapon(prevIdx);
}

//*---------------------------------------------------------------------------------------
//*【?】武器の登録
//*
//* [引数] 
//* _pWeapon : 武器コンポーネントの共有ポインタ
//* _slot : 武器をセットするスロット 
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::RegisterWeapon(std::shared_ptr<class GunWeapon> _pWeapon, int _slot)
{
	if (_slot >= m_MaxSlot)return;

	m_WeaponArray.emplace_back(_pWeapon);
}


//*----------------------------------------------------------------------------------------
//*【?】開始時に持っている武器の設定
//*
//* [引数] 
//* _slot : 武器スロット（ 0以上 m_MaxSlot以下） 
//* [返値] なし
//*----------------------------------------------------------------------------------------
void WeaponController::StartingWeapon(int _slot)
{
	m_CrntWeaponSlotIndex = _slot;

	// アクティブ状態に
	m_WeaponArray[m_CrntWeaponSlotIndex]->get_OwnerObj().lock()->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

	// 装備武器以外は非アクティブに
	for (int i = 0; i < m_MaxSlot; i++)
	{
		if (m_CrntWeaponSlotIndex != i) {
			m_WeaponArray[i]->get_OwnerObj().lock()->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
		}
	}
}
