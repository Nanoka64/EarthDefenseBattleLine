#include "pch.h"
#include "SceneManager.h"
#include "c_Game_Play.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "InputFactory.h"
#include "Component_Health.h"
#include "RendererEngine.h"
#include "GameObject.h"
#include "Component_3DCamera.h"

using namespace SceneStateEnums;
using namespace VECTOR3;
using namespace VECTOR2;

constexpr float MINIMAP_X = 1700.0f;    // ミニマップの中心X座標
constexpr float MINIMAP_Y = 200.0f;     // ミニマップの中心X座標
constexpr float MINIMAP_RADIUS = 150.0f;// ミニマップの半径

// 位置をミニマップ内の端に制限するユーティリティ
void ClampToMinimap(int& x, int& y, int centerX, int centerY, float radius)
{
    float dx = static_cast<float>(x - centerX);
    float dy = static_cast<float>(y - centerY);

    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > radius)
    {
        float scale = radius / dist;
        x = centerX + static_cast<int>(dx * scale);
        y = centerY + static_cast<int>(dy * scale);
    }
}

//*---------------------------------------------------------------------------------------
//* @:c_Game_Play Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Play::OnEnter(SceneManager* pOwner)
{
    m_pPlayerObj = Master::m_pGameObjectManager->get_ObjectByTag("Player");

    // プレイヤーHPバー用スプライトを作る **********************************************
    UIData::RectTransformData rectData;
    rectData._size = VEC2(600.0f, 300.0f);
    rectData._pos = VEC2(50.0f, 0.0f);
    rectData._anchorMax = VEC2(0.0f, 0.0f);
    rectData._anchorMin = VEC2(0.0f, 0.0f);
    UIData::SpriteUIData spriteData;
    spriteData._tag = "HPBar_Back";
    spriteData._imagePath = "Resource/Texture/UI/HPBar.png";
    spriteData._layerRank = 101;
    m_pPlayerHPBarBackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);


    // プレイヤーHPバー本体 **********************************************
    // HPバー本体はテクスチャなしで、カラーのみで表示する。
    rectData._size = VEC2(520.0f, 30.0f);
    rectData._pos = VEC2(90.0f, 125.0f);
    spriteData._tag = "HPBar";
    spriteData._layerRank = 100;
    spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE;    // テクスチャなし
    spriteData._color = VECTOR4::VEC4(0.0f, 0.8f, 0.0f, 1.0f);
    m_pPlayerHPBarSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);

    // レーダー背景用スプライト **********************************************
    rectData._size = VEC2(350.0f, 350.0f);
    rectData._pos = VEC2(1700.0f, 200.0f);
    rectData._anchorMax = VEC2(0.0f, 0.0f);
    rectData._anchorMin = VEC2(0.0f, 0.0f);
	rectData._pivot = VEC2(0.5f, 0.5f);         // ピボットを中心に設定
    spriteData._tag = "Radar_Back";
    spriteData._imagePath = "Resource/Texture/UI/Radar.png";
    spriteData._shaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    spriteData._layerRank = 100;
    m_pRaderBackSpriteObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);

    // レーダーに映す点用スプライト **********************************************
    rectData._size = VEC2(10.0f, 10.0f);
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
        m_pRaderEnemySpriteObjArray.push_back(Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData));
    }
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Play Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Play::OnExit(SceneManager* pOwner)
{
	m_pPlayerHPBarSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);    // プールへ返す
    m_pPlayerHPBarBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);    // プールへ返す
    m_pRaderBackSpriteObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);    // プールへ返す

    for (int i = 0; i < m_pRaderEnemySpriteObjArray.size(); i++) {
        m_pRaderEnemySpriteObjArray[i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
    }
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Play Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Game_Play::Update(SceneManager *pOwner)
{
    if (m_pPlayerObj.expired())
    {
		MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(NULL, "プレイヤーが存在しない", "c_Game_Play", MB_OK);
        pOwner->OnSceneClose();
    }  
    
    auto antList = Master::m_pGameObjectManager->get_ObjectListByTag("Ant");
    m_EnemyNum = antList.size();
    // 敵が居なくなったらリザルトへ
    if (m_EnemyNum == 0)
    {
        return c_GAME::c_GO_RESULT_SCENE;
    }

    BomberMove();

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


    if (m_EnemyNum < m_pRaderEnemySpriteObjArray.size())
    {
        int diff = m_pRaderEnemySpriteObjArray.size() - m_EnemyNum;

        for (int i = 0; i < diff; i++)
        {
            m_pRaderEnemySpriteObjArray[(m_pRaderEnemySpriteObjArray.size() - 1) - i]->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);    // プールへ返す
            m_pRaderEnemySpriteObjArray.pop_back();
        }
    }

	VEC3 playerPos = m_pPlayerObj.lock()->get_Transform().lock()->get_VEC3ToPos();
    float cameraAngleH = m_pRenderer->get_CameraComponent()->get_Angle_H();

    // カメラの水平回転角度に基づいて、サインとコサインを計算（ずれてしまうので0.7で補正）
	float cameraH_s = sinf(-cameraAngleH - 0.7854f);
    float cameraH_c = cosf(-cameraAngleH - 0.7854f);

	// レーダーの敵の位置更新
    for (int i = 0; i < m_pRaderEnemySpriteObjArray.size(); i++)
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


    return c_GAME::c_GAME_PLAY;
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Play Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Play::Draw(SceneManager* pOwner)
{
    Master::m_pDirectWriteManager->DrawFormatString("残りの敵数：{:d}", VECTOR2::VEC2(0, 540), "White_40_STD",m_EnemyNum);
}


//*---------------------------------------------------------------------------------------
//* @:c_Game_Play Class 
//*【?】爆撃機動かす
//* 引数：なし
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Game_Play::BomberMove()
{
    m_pBomber[0] = Master::m_pGameObjectManager->get_ObjectByTag("B-2_1");
    m_pBomber[1] = Master::m_pGameObjectManager->get_ObjectByTag("B-2_2");
    m_pBomber[2] = Master::m_pGameObjectManager->get_ObjectByTag("B-2_3");
    static float counter = 0.0f;
    counter += 0.01f;

    VEC3 playerPos = m_pPlayerObj.lock()->get_Transform().lock()->get_VEC3ToPos();
    //-----------------------------------------------------------------------------
    // ■ 爆撃機の移動処理
    //-----------------------------------------------------------------------------
    static VEC3 B2PrevPos[3]{};
    VEC3 center = VEC3(0.0f, 0.0f, 0.0f);
    float radius = 700.0f;
    for (int i = 0; i < 3; i++)
    {
        auto B2Transform = m_pBomber[i].lock()->get_Transform().lock();
        if (B2Transform)
        {
            float intervalDist = 200.0f * i;

            VEC3 pos = VEC3(0.0f, 0.0f, 0.0f);
            pos.x = center.x + (radius - intervalDist) * cos(counter);
            pos.y = 300.0f + i * 100.0f;
            pos.z = center.z + (radius - intervalDist) * sin(counter);

            // 速度 = 今の座標 - 前の座標
            VEC3 velocity = pos - B2PrevPos[i];

            // 移動ベクトルから、Y軸周りの回転角度を求める
            float targetAngle = atan2(velocity.x, velocity.z);

            // 前の座標として保持
            B2PrevPos[i] = pos;

            B2Transform->set_Pos(pos);
            B2Transform->SlerpLookAt(playerPos, 0.5f);
        }
    }
}
