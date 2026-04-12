#include "pch.h"
#include "Component_Player_HPBar.h"
#include "Component_Health.h"
#include "RendererEngine.h"
#include "GameObject.h"

using namespace VECTOR3;
using namespace VECTOR2;


//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//*----------------------------------------------------------------------------------------
Player_HPBar::Player_HPBar(std::weak_ptr<GameObject> pOwner, int updateRank)
    :IComponent(pOwner, updateRank),
    m_pPlayerHPBarBackSpriteObj(nullptr),
    m_pPlayerHPBarSpriteObj(nullptr)
{
    this->set_Tag("Player_HPBar");
}


//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
Player_HPBar::~Player_HPBar()
{
    // ---------------------------------------
    // プールへ返す
    // ---------------------------------------
    if (m_pPlayerHPBarBackSpriteObj != nullptr) {
        m_pPlayerHPBarBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
    }
    if (m_pPlayerHPBarSpriteObj != nullptr) {
        m_pPlayerHPBarSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
    }
}

//*---------------------------------------------------------------------------------------
//*【?】初期化
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void Player_HPBar::Start(RendererEngine& renderer)
{
    m_pPlayerObj = Master::m_pGameObjectManager->get_ObjectByTag("Player");

    UIData::RectTransformData rectData;
    UIData::SpriteUIData spriteData;


    // プレイヤーHPバー用スプライトを作る **********************************************
    rectData._size = VEC2(600.0f, 300.0f);
    rectData._pos = VEC2(50.0f, 0.0f);
    rectData._anchorMax = VEC2(0.0f, 0.0f);
    rectData._anchorMin = VEC2(0.0f, 0.0f);
    spriteData._tag = "HPBar_Back";
    spriteData._imagePath = "Resource/Texture/UI/HPBar_Ver2.png";
    spriteData._layerRank = 101;
    m_pPlayerHPBarBackSpriteObj = Master::m_pUIManager->GetSprite(renderer, rectData, spriteData);


    // プレイヤーHPバー本体 **********************************************
    // HPバー本体はテクスチャなしで、カラーのみで表示する。
    rectData._size = VEC2(520.0f, 30.0f);
    rectData._pos = VEC2(90.0f, 125.0f);
    spriteData._tag = "HPBar";
    spriteData._layerRank = 100;
    spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;    // テクスチャなし
    spriteData._color = VECTOR4::VEC4(0.0f, 1.0f, 1.0f, 1.0f);
    m_pPlayerHPBarSpriteObj = Master::m_pUIManager->GetSprite(renderer, rectData, spriteData);
}

//*---------------------------------------------------------------------------------------
//*【?】更新
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void Player_HPBar::Update(RendererEngine& renderer)
{
    // HPバーの更新
    if (auto player = m_pPlayerObj.lock())
    {
        float crntHP = player->get_Component<Health>()->get_CrntHP();
        float maxHP = player->get_Component<Health>()->get_MaxHP();

        float hpBarScale = (crntHP / maxHP) * 520.0f;    // HPバーの最大幅が520なので、5.2倍してスケールに変換
        m_pPlayerHPBarSpriteObj->get_RectTransform().lock()->set_Size(hpBarScale, 30.0f);

        Master::m_pDirectWriteManager->DrawFormatString("{:d}", VECTOR2::VEC2(160.0f, 157), "White_20_STD", INT_CAST(crntHP));
        Master::m_pDirectWriteManager->DrawFormatString("{:d}", VECTOR2::VEC2(160.0f, 185), "White_20_STD", INT_CAST(maxHP));
    }
}
