#include "pch.h"
#include "Component_ButtonUI.h"
#include "Component_SpriteRenderer.h"
#include "RendererEngine.h"


using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//* [引数]
//* pOwner : オーナーオブジェクト
//* updateRank : 更新レイヤー
//*----------------------------------------------------------------------------------------
ButtonUI::ButtonUI(std::weak_ptr<GameObject> pOwner, int updateRank)
	: IComponent(pOwner, updateRank),
	m_StateColor(VEC4()),
	m_CrntState(UIData::STATE::NORMAL),
	m_InputValidationState(UIData::STATE::PRESSED),
	m_FadeDuration(0.1f),
	m_IsInteractable(true),
	m_Text("Button"),
	m_TextOffsetPos(VEC2())
{
	this->set_Tag("Button");
}


//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
ButtonUI::~ButtonUI()
{

}


//*---------------------------------------------------------------------------------------
//*【?】初期化
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void ButtonUI::Start(RendererEngine &renderer)
{
	if (m_pOwner.expired())
	{
		assert(false);
		return;
	}
	m_pMyTransform = m_pOwner.lock()->get_RectTransform();


	m_StateColor[UINT_CAST(UIData::STATE::NORMAL)]		 = VEC4(1.0f, 1.0f, 1.0f, 1.0f);	// 通常
	m_StateColor[UINT_CAST(UIData::STATE::HIGH_LIGHTED)] = VEC4(0.6f, 0.6f, 0.6f, 1.0f);	// ハイライト
	m_StateColor[UINT_CAST(UIData::STATE::PRESSED)]		 = VEC4(0.4f, 0.4f, 0.4f, 1.0f);	// 押されている
	m_StateColor[UINT_CAST(UIData::STATE::SELECTED)]	 = VEC4(1.0f, 1.0f, 1.0f, 1.0f);	// 選択された
	m_StateColor[UINT_CAST(UIData::STATE::DISABLED)]	 = VEC4(0.1f, 0.1f, 0.1f, 1.0f);	// 無効
}


//*---------------------------------------------------------------------------------------
//*【?】更新
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void ButtonUI::Update(RendererEngine &renderer)
{
	if (m_IsInteractable == false) {
		m_CrntState = UIData::STATE::DISABLED;	// 無効状態
		return;
	}

	m_CrntState = UIData::STATE::NORMAL;	// 通常状態

	POINT mousePos = Master::m_pInputManager->GetMousePos();	// マウス座標
	auto transform = m_pMyTransform.lock();
	VEC2 pos = transform->get_RectPosition();
	VEC2 size = transform->get_SizeDelta();

	// ボタンの衝突判定情報
	CollInData2D_AABB colData;
	colData._min = pos;
	colData._max = pos + size;

	// マウスとボタンの判定
	if (Master::m_pCollisionManager->HitCheck2D_BoxVsPoint(colData, VEC2(mousePos.x, mousePos.y)))
	{
		m_CrntState = UIData::STATE::HIGH_LIGHTED;	// ハイライト状態

		// 左クリックまたは、決定キーで選択
		if (GetMouseClick(MOUSE_BUTTON_STATE::LEFT) || GetInput(GAME_CONFIG::DECITION))
		{
			m_CrntState = UIData::STATE::PRESSED;	// 押されている状態
			
			// キーが離されたら入力判定
			if (GetMouseClickUp(MOUSE_BUTTON_STATE::LEFT) || GetInputUp(GAME_CONFIG::DECITION))
			{
				m_CrntState = UIData::STATE::SELECTED;	// 選択された状態
			}
		}
	}


	// 入力処理を行うステートと一致すれば実行
	if (m_InputValidationState == m_CrntState)
	{
		// クリック処理実行
		if (m_OnClick)
		{
			m_OnClick();
		}
	}
}


//*---------------------------------------------------------------------------------------
//*【?】描画
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void ButtonUI::Draw(RendererEngine &renderer)
{
	auto transform = m_pMyTransform.lock();
	VEC2 pos = transform->get_RectPosition();
	Master::m_pDirectWriteManager->DrawString(m_Text, pos + m_TextOffsetPos, "White_40_STD");

	if (!m_pSprite.expired())
	{
		auto sprite = m_pSprite.lock();

		/* 状態によってスプライトの色を変える */
		sprite->set_Color(m_StateColor[UINT_CAST(m_CrntState)]);
	}
}
