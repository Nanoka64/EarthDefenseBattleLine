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
#include "Component_SkinnedMeshAnimator.h"

using namespace GIGA_Engine;
using namespace Input;
using namespace VECTOR3;
using namespace BulletData;
using namespace WeaponData;

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

    // 全ビットを0に
    m_WeaponFlags.Init();
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


    auto player = Master::m_pGameObjectManager->get_ObjectByTag("Player");
    auto skinnedMesh = player->get_Component<SkinnedMeshAnimator>();
    //auto& boneMtx = skinnedMesh->get_BoneLocalWorldMatrix("mixamorig:RightHand");
    auto& boneMtx = skinnedMesh->get_BoneLocalWorldMatrix("WeaponSocket_Right");
    transform->set_OffsetWorldTransfomationMatrix(boneMtx);

    VEC3 pos = transform->get_WorldVEC3ToPos();

    // 武器を回転させる
    // 水平方向はプレイヤーに合わせているので垂直方向のみ、カメラの回転を使う。
    //transform->set_RotateToRad(VEC3(c_AngleV * -1, 0.0f, 0.0f));

    auto gunParam = get_GunWeaponParameter();

    // レーザーサイト
    if (gunParam->_isLaserSight && !m_pLineRendererComp.expired())
    {
        // ワールド変換行列から方向をとる
        XMMATRIX worldMtx = transform->get_WorldMtx();
        XMVECTOR forward = worldMtx.r[2];  // Z
        //forward *= -1;  // プレイヤーが-Z前になってしまっているので

        // レーザーサイトの始点と方向
        m_pLineRendererComp.lock()->set_Dir(VEC3::FromXMVECTOR(XMVector3Normalize(forward)));
        m_pLineRendererComp.lock()->set_StartPos(pos);

        // レーザーポインタ
        auto laserPoint = Master::m_pGameObjectManager->get_ObjectByTag("LaserPointBillboard");

        VEC3 laserDir = m_pLineRendererComp.lock()->get_Dir();
        VEC3 laserPointPos = pos + laserDir * m_pLineRendererComp.lock()->get_Length();
        laserPoint->get_Transform().lock()->set_Pos(laserPointPos);
        laserPoint->get_Transform().lock()->set_Scale(VEC3(5.0f));

    }

    if (!m_pFlashPointLight.expired()) {
        // 弾を発射してないときはフラッシュライトをオフ
        m_pFlashPointLight.lock()->set_Intensity(0.0f);
    }

    // ズーム倍率があるなら
    if (gunParam->_zoomLength > 1.0f)
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
            zoomFov = defaultFov / gunParam->_zoomLength; // 倍率が高いほどFovが小さくなる
        }

        // Fovの設定
        renderer.get_CameraComponent()->set_Fov(zoomFov);
    }


    Master::m_pDebugger->BeginDebugWindow(Tool::U8ToChar(u8"武器情報"));
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"弾数：%d / %d"), m_AmmoRemaining, gunParam->_bulletMaxNum);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"リロード時間：%.2f"), gunParam->_reloadTime);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"ズーム倍率：%.2f"), gunParam->_zoomLength);
    Master::m_pDebugger->DG_BulletText(Tool::U8ToChar(u8"連射速度：%.2f"), gunParam->_fireRate);
    Master::m_pDebugger->EndDebugWindow();

    // ステート更新
    m_StateMachine.Update();
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
//*【?】武器のパラメータ取得
//*
//* [引数] なし
//* [返値] 書き換え不可の武器パラメータ
//*----------------------------------------------------------------------------------------
const WeaponData::GunWeaponData* GunWeapon::get_GunWeaponParameter()const
{
    return static_cast<const WeaponData::GunWeaponData*>(m_pParameter);
}

//*---------------------------------------------------------------------------------------
//*【?】セットアップ
//*
//* [引数]
//* & _weaponData : 武器データ
//* [返値]なし
//*----------------------------------------------------------------------------------------
bool GunWeapon::Setup(const WeaponData::BaseWeaponData* _pWeaponData)
{
    m_pParameter = static_cast<const GunWeaponData*>(_pWeaponData);
    if (!m_pParameter)
    {
        MessageBox(NULL, "武器のパラメータが一致しません", "GunWeapon", MB_OK);
        return false;
    }

    auto gunParam = get_GunWeaponParameter();

    m_AmmoRemaining = gunParam->_bulletMaxNum;

    return true;
}


//*---------------------------------------------------------------------------------------
//*【?】武器切り替え時のリセット
//*
//* [引数] 
//* &renderer : 描画エンジンの参照
//* [返値] なし
//*----------------------------------------------------------------------------------------
void GunWeapon::SwicthReset()
{
	// ズーム解除
    Master::m_pDataManager->set_Fov(Master::m_pDataManager->get_DefaultFov());
}


//*---------------------------------------------------------------------------------------
//*【?】弾の発射処理
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]なし
//*----------------------------------------------------------------------------------------
void GunWeapon::Shoot(RendererEngine& renderer)
{
    float c_AngleH = renderer.get_CameraComponent()->get_Angle_H();
    float c_AngleV = renderer.get_CameraComponent()->get_Angle_V();

    auto transform = m_pOwner.lock()->get_Transform().lock();
    VEC3 pos = transform->get_WorldVEC3ToPos();

    // ****************************************************
    //				 発射音再生
    // ****************************************************
    Master::m_pSoundManager->Play_RandPitch(SOUND_TYPE::SE, SOUND_ID_TO_INT(SOUND_ID::GUN_FIRE02), 300);
        
    auto gunParam = get_GunWeaponParameter();


    // 同時発射
    for (int i = 0; i < gunParam->_bulletSimultaneousNum; i++)
    {
        // 親の向き等を参照
        //rad.x = (c_AngleV);
        //rad.y = (c_AngleH - 1.57f);
        //rad.z = 0.0f;

        // ワールド変換行列から方向をとる
        XMMATRIX worldMtx = transform->get_WorldMtx();
        DirectX::XMVECTOR scale;
        DirectX::XMVECTOR rotQuat; 
        DirectX::XMVECTOR trans;
        // 変換行列の分解しクォータニオンを取得する
        DirectX::XMMatrixDecompose(&scale, &rotQuat, &trans, worldMtx);


        // 弾のバラつき
        float accuracy = gunParam->_accuracy;
        VEC3 accuracyRot;
        accuracyRot.x += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
        accuracyRot.y += Master::m_pRandomManager->GetFloatRandom(-accuracy, accuracy);
        accuracyRot.z = 0.0f;

        // バラつきクォータニオン
        XMVECTOR spreadQuat = XMQuaternionRotationRollPitchYaw(accuracyRot.x, accuracyRot.y, accuracyRot.z);

        // 最終的なクォータニオン作成
        XMVECTOR finalRotQuat = XMQuaternionMultiply(rotQuat, spreadQuat);
        finalRotQuat = XMQuaternionNormalize(finalRotQuat); // 念のため正規化


        // トランスフォームパラメータ
        BulletTransformData bulletTransform;
        bulletTransform._pos = pos;
        bulletTransform._rotQ = finalRotQuat;
        bulletTransform._scale = VEC3(0.01f, 0.01f, 0.01f);

        BULLET_TYPE type = gunParam->_bulletType;

        // 弾データを共用体で持っているので、弾タイプにあったパラメータを入れるようにする
        std::visit([&](auto& param) {
            Master::m_pBulletManager->Shot(renderer, bulletTransform, param);
            }, gunParam->_bulletParam);
    }

    if (!m_pFlashPointLight.expired()) {
        // フラッシュ
        m_pFlashPointLight.lock()->set_Range(30.0f);
        m_pFlashPointLight.lock()->set_Intensity(5.5f);
        m_pFlashPointLight.lock()->set_LightColor(VEC3(1.0f, 1.0f, 1.0f));
    }

    // 弾数減らす
    m_AmmoRemaining--;
}
