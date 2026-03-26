#include "pch.h"
#include "Component_GunWeapon.h"
#include "Component_Transform.h"
#include "Component_NormalBullet.h"
#include "RendererEngine.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "InputFactory.h"
#include "ResourceManager.h"
#include "Component_3DCamera.h"
#include "Component_LineRenderer.h"
#include "Component_PointLight.h"
#include "GameManager.h"
#include "Gun_StateHeader.h"
#include "WeaponStateFactory.h"

using namespace GIGA_Engine;
using namespace Input;
using namespace VECTOR3;
using namespace BulletData;

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//* [引数]
//* pOwner : オーナーオブジェクト
//* updateRank : 更新レイヤー
//*----------------------------------------------------------------------------------------
GunWeapon::GunWeapon(std::weak_ptr<GameObject> pOwner, int updateRank)
    : WeaponBase(pOwner, updateRank),
    m_StateMachine(this),
    m_IsNowZoom(false),
    m_IsStopFire(false),
    m_AmmoRemaining(0)
{
    this->set_Tag("GunWeapon");
}



//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
GunWeapon::~GunWeapon()
{

}


//*---------------------------------------------------------------------------------------
//*【?】初期化
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void GunWeapon::Start(RendererEngine& renderer)
{
    // 照準レーザー用
    m_pLineRendererComp = m_pOwner.lock()->get_Component<LineRenderer>();
    m_pFlashPointLight = m_pOwner.lock()->get_Component<PointLight>();


    // ステートの作成
    WeaponStateFactory::CreateGunWeapon(m_StateMachine, 0, renderer);
    m_StateMachine.SetCrntState(GUN_STATE::GUN_STATE_IDLE);
}


//*---------------------------------------------------------------------------------------
//*【?】更新
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void GunWeapon::Update(RendererEngine& renderer)
{
    float c_AngleH = renderer.get_CameraComponent()->get_Angle_H();
    float c_AngleV = renderer.get_CameraComponent()->get_Angle_V();

    auto transform = m_pOwner.lock()->get_Transform().lock();
    VEC3 pos = transform->get_WorldVEC3ToPos();

    // 武器を回転させる
    // 水平方向はプレイヤーに合わせているので垂直方向のみ、カメラの回転を使う。
    transform->set_RotateToRad(VEC3(c_AngleV * -1, 0.0f, 0.0f));

    // レーザーサイト
    if (m_WeaponParameter._isLaserSight)
    {
        // ワールド変換行列から方向をとる
        XMMATRIX worldMtx = transform->get_WorldMtx();
        XMVECTOR forward = worldMtx.r[2];  // Z
        forward *= -1;  // プレイヤーが-Z前になってしまっているので

        // レーザーサイトの始点と方向
        m_pLineRendererComp.lock()->set_Dir(VEC3::FromXMVECTOR(XMVector3Normalize(forward)));
        m_pLineRendererComp.lock()->set_StartPos(pos);
    }

    // 弾を発射してないときはフラッシュライトをオフ
    m_pFlashPointLight.lock()->set_Intensity(0.0f);

    // ズーム倍率があるなら
    if (m_WeaponParameter._zoomLength > 1.0f)
    {
        // FOVに倍率
        float defaultFov = Master::m_pDataManager->get_DefaultFov();
        float zoomFov = defaultFov;

        // 右クリック
        if (GetMouseClickDown(MOUSE_BUTTON_STATE::RIGHT)) {
            m_IsNowZoom = m_IsNowZoom ? false : true;   // ズーム切り替え
        }

        // ズーム
        if (m_IsNowZoom) {
            zoomFov = defaultFov / m_WeaponParameter._zoomLength; // 倍率が高いほどFovが小さくなる
        }

        // Fovの設定
        renderer.get_CameraComponent()->set_Fov(zoomFov);
    }

    Master::m_pDebugger->BeginDebugWindow(Tool::U8ToChar(u8"武器情報"));
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"弾数：%d / %d"), m_AmmoRemaining, m_WeaponParameter._bulletMaxNum);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"リロード時間：%.2f"), m_WeaponParameter._reloadTime);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"ズーム倍率：%.2f"), m_WeaponParameter._zoomLength);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"連射速度：%.2f"), m_WeaponParameter._fireRate);
    Master::m_pDebugger->EndDebugWindow();

    // ステート更新
    m_StateMachine.Update();

    //// 左クリックで発射
    //if (GetMouseClickHoldRepeat(MOUSE_BUTTON_STATE::LEFT, m_WeaponParameter._fireRate, m_WeaponParameter._fireRate))
    //{
    //    // ****************************************************
    //    //				 発射音再生
    //    // ****************************************************
    //    Master::m_pSoundManager->Play_RandPitch(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::GUN_FIRE02), 300);

    //        
    //    // 同時発射
    //    for (int i = 0; i < m_WeaponParameter._bulletSimultaneousNum; i++)
    //    {
    //        // 親の向き等を参照
    //        VEC3 rad;
    //        rad.x = (c_AngleV) * -1;
    //        rad.y = (c_AngleH - 1.57f) * -1;
    //        rad.z = 0.0f;
    //        float accuracy = m_WeaponParameter._accuracy;

    //        // 弾のバラつき
    //        rad.x += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
    //        rad.y += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
    //        rad.z += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);

    //        // トランスフォームパラメータ
    //        BulletTransformData bulletTransform;
    //        bulletTransform._pos = pos;
    //        bulletTransform._rotRad = rad;
    //        bulletTransform._scale = VEC3(0.01f, 0.01f, 0.01f);

    //        BULLET_TYPE type = m_WeaponParameter._bulletType;

    //        // 弾データを共用体で持っているので、弾タイプにあったパラメータを入れるようにする
    //        std::visit([&](auto& param) {
    //            Master::m_pBulletManager->Shot(renderer, bulletTransform, param);
    //            }, m_WeaponParameter._bulletParam);
    //    }

    //    // フラッシュ
    //    m_pFlashPointLight.lock()->set_Range(30.0f);
    //    m_pFlashPointLight.lock()->set_Intensity(5.5f);
    //    m_pFlashPointLight.lock()->set_LightColor(VEC3(1.0f, 1.0f, 1.0f));

    //    // 弾数減らす
    //    m_AmmoRemaining--;
    //}
}


//*---------------------------------------------------------------------------------------
//*【?】描画
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void GunWeapon::Draw(RendererEngine& renderer)
{

}

//*---------------------------------------------------------------------------------------
//*【?】セットアップ
//*
//* [引数]
//* & _weaponData : 武器データ
//* [返値]なし
//*----------------------------------------------------------------------------------------
bool GunWeapon::Setup(const WeaponData::GunWeaponData& _weaponData)
{
    m_WeaponParameter = _weaponData;

    m_AmmoRemaining = m_WeaponParameter._bulletMaxNum;

    return true;
}