#include "pch.h"
#include "Component_MiniMapRader.h"
#include "RendererEngine.h"
#include "GameObject.h"
#include "Component_3DCamera.h"

using namespace VECTOR3;
using namespace VECTOR2;

constexpr float MINIMAP_X = 1750.0f;    // ミニマップの中心X座標
constexpr float MINIMAP_Y = 150.0f;     // ミニマップの中心X座標
constexpr float MINIMAP_RADIUS = 130.0f;// ミニマップの半径
constexpr float MINIMAP_SIZE = 300.0f;  // ミニマップのサイズ
constexpr float MINIMAP_ENEMY_ICON_SIZE = 10.0f; // ミニマップ上の敵アイコンのサイズ

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//*----------------------------------------------------------------------------------------
MiniMapRader::MiniMapRader(std::weak_ptr<GameObject> pOwner, int updateRank)
	:IComponent(pOwner, updateRank),
    m_pRaderBackSpriteObj(nullptr),
    m_MaxEnemyNum(0),
    m_CrntEnemyNum(0)

{
	this->set_Tag("MiniMapRader");
}


//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
MiniMapRader::~MiniMapRader()
{
	// ---------------------------------------
    // プールへ返す
    // ---------------------------------------
    if (m_pRaderBackSpriteObj != nullptr) {
        m_pRaderBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
    }
    for (int i = 0; i < m_pRaderEnemySpriteObjArray.size(); i++) {
        if (m_pRaderEnemySpriteObjArray[i] != nullptr) {
            m_pRaderEnemySpriteObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
        }
    }
}

//*---------------------------------------------------------------------------------------
//*【?】初期化
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void MiniMapRader::Start(RendererEngine& renderer)
{
    m_pPlayerObj = Master::m_pGameObjectManager->get_ObjectByTag("Player");

    UIData::RectTransformData rectData;
	UIData::SpriteUIData spriteData;

    // レーダー背景用スプライト **********************************************
    rectData._size = VEC2(MINIMAP_SIZE, MINIMAP_SIZE);
    rectData._pos = VEC2(MINIMAP_X, MINIMAP_Y);
    rectData._anchorMax = VEC2(0.0f, 0.0f);
    rectData._anchorMin = VEC2(0.0f, 0.0f);
    rectData._pivot = VEC2(0.5f, 0.5f);         // ピボットを中心に設定
    spriteData._tag = "Radar_Back";
    spriteData._imagePath = "Resource/Texture/UI/Radar.png";
    spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    spriteData._layerRank = 100;
    m_pRaderBackSpriteObj = Master::m_pUIManager->GetSprite(renderer, rectData, spriteData);

    // レーダーに映す点用スプライト **********************************************
    rectData._size = VEC2(MINIMAP_ENEMY_ICON_SIZE, MINIMAP_ENEMY_ICON_SIZE);
    rectData._pos = VEC2(0.0f, 0.0f);
    rectData._anchorMax = VEC2(0.0f, 0.0f);
    rectData._anchorMin = VEC2(0.0f, 0.0f);
    rectData._pivot = VEC2(0.5f, 0.5f);         // ピボットを中心に設定
    spriteData._tag = "Circle";
    spriteData._imagePath = "Resource/Texture/UI/Circle.png";
    spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    spriteData._layerRank = 100;
    spriteData._color = VECTOR4::VEC4(5.0f, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 50; i++) {
        m_pRaderEnemySpriteObjArray.push_back(Master::m_pUIManager->GetSprite(renderer, rectData, spriteData));
    }
}

//*---------------------------------------------------------------------------------------
//*【?】更新
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void MiniMapRader::Update(RendererEngine& renderer)
{
    auto antList = Master::m_pGameObjectManager->get_ObjectListByTag("Ant");

    VEC3 playerPos = m_pPlayerObj.lock()->get_Transform().lock()->get_VEC3ToPos();
    float cameraAngleH = renderer.get_CameraComponent()->get_Angle_H();

    // カメラの水平回転角度に基づいて、サインとコサインを計算（ずれてしまうので0.7で補正）
    float cameraH_s = sinf(-cameraAngleH - M_PI_2);
    float cameraH_c = cosf(-cameraAngleH - M_PI_2);

    // レーダーの敵の位置更新
    for (int i = 0; i < antList.size(); i++)
    {
        auto& enemy = antList[i];
        VEC3 enemyPos = enemy->get_Transform().lock()->get_VEC3ToPos();

        // 相対座標の計算
        float relativeX = enemyPos.x - playerPos.x;
        float relativeZ = enemyPos.z - playerPos.z;

        // 回転とスケールの適用を一度に行う
        float rotX = (relativeX * cameraH_c - relativeZ * cameraH_s);
        float rotY = (relativeX * cameraH_s + relativeZ * cameraH_c);

        int enemyMapX = static_cast<int>(MINIMAP_X + rotX);
        int enemyMapY = static_cast<int>(MINIMAP_Y - rotY);

        // 位置をミニマップの円内に収める
        ClampToMinimap(enemyMapX, enemyMapY, static_cast<int>(MINIMAP_X), static_cast<int>(MINIMAP_Y), MINIMAP_RADIUS);

        m_pRaderEnemySpriteObjArray[i]->get_RectTransform().lock()->set_RectPosition(VEC2(enemyMapX, enemyMapY));
    }
}


//*---------------------------------------------------------------------------------------
//*【?】位置をミニマップ内の端に制限する
//*
//* [引数]
//* &_x : 制限したいX座標の参照 
//* &_y : 制限したいY座標の参照
//* _centerX : ミニマップの中心X座標
//* _centerY : ミニマップの中心Y座標
//* _radius : ミニマップの半径
//* [返値]なし
//*----------------------------------------------------------------------------------------
void MiniMapRader::ClampToMinimap(int& _Out_X, int& _Out_Y, int _centerX, int _centerY, float _radius)
{
    float dx = static_cast<float>(_Out_X - _centerX);
    float dy = static_cast<float>(_Out_Y - _centerY);

    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > _radius)
    {
        float scale = _radius / dist;
        _Out_X = _centerX + static_cast<int>(dx * scale);
        _Out_Y = _centerY + static_cast<int>(dy * scale);
    }
}