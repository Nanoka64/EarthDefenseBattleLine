#include "pch.h"
#include "c_Title_LoadProcess.h"
#include "RendererEngine.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "Component_SpriteRenderer.h"
#include "Component_ButtonUI.h"
#include "Component_SkinnedMeshAnimator.h"
#include "Component_PlayerController.h"
#include "Component_BoxCollider.h"
#include "Component_3DCamera.h"
#include "Component_Transform.h"
#include "Component_TrailRenderer.h"
#include "Component_LineRenderer.h"
#include "Component_DirectionalLight.h"
#include "Component_AssultRifle.h"
#include "Component_Health.h"
#include "Component_PointLight.h"
#include "GameObject.h"
#include "MeshFactory.h"
#include "InputFactory.h"

using namespace SceneStateEnums;
using namespace VECTOR2;
using namespace VECTOR3;
using namespace VECTOR4;
using namespace GIGA_Engine;;


//*---------------------------------------------------------------------------------------
//* @:c_Title_LoadProcess Class 
//*【?】開始
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_LoadProcess::OnEnter(SceneManager *pOwner)
{
	// すでにロードされているなら返す
	if (m_IsLoad) {
		return;
	}

    // *************************************************************************************************
    /**  UI管理の初期化 **/
    // *************************************************************************************************
    if (!Master::m_pUIManager->Init(*m_pRenderer))
    {
        MessageBox(NULL, "UI管理クラスの初期化に失敗しました", "GameLoad", MB_OK);
        assert(false);
    }

    /* カメラの作成 */
    {
        auto obj = Instantiate3D(std::move(std::make_shared<GameObject>()), false);
        if (obj == nullptr)
        {
            assert(false);
        }
        obj->Init(*m_pRenderer);
        obj->set_LayerRank(91);
        obj->set_Tag("Camera");
        obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
        obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_DONT_DESTROY);
        obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 1000.0f);
        m_pCameraComp = obj->add_Component<Camera3D>(); // カメラコンポーネントの追加
        m_pCameraComp->set_IsControl(false);    // 操作フラグをオフに

        // 描画エンジン側にカメラコンポーネントを渡す
        m_pRenderer->set_CameraComponent(m_pCameraComp);
    }

    // ライトにカメラのTransformを持たせる
    Master::m_pLightManager->set_CameraTransform(m_pCameraComp->get_OwnerObj().lock()->get_Transform());


 //   // ロード画面用用スプライト**********************************************
	//CreateSpriteInfo sprite;
	//sprite.pTextureMap[0] = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Texture/Title/Load.png");
	//sprite.IsActive = true;
	//sprite.ObjTag = "LoadScreen_Sp";
	//sprite.pRenderer = m_pRenderer;
	//sprite.ShaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
	//sprite.Type = SPRITE_USAGE_TYPE::NORMAL;
	//sprite.Width = m_pRenderer->get_ScreenWidth();
	//sprite.Height = m_pRenderer->get_ScreenHeight();
	//sprite.IsActive = true;
	//sprite.IsTransparent = true;
	//auto obj = MeshFactory::CreateSprite(sprite);
	//if (obj) {
	//	obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 0.0f);
 //       m_pLoadBackSprite = obj->get_Component<SpriteRenderer>();
 //   }

    // ロード画面用用スプライトを作る **********************************************
	float width = static_cast<float>(m_pRenderer->get_ScreenWidth());
	float height = static_cast<float>(m_pRenderer->get_ScreenHeight());
	UIData::RectTransformData rectData;
    rectData._size = VEC2(width, height);
	UIData::SpriteUIData spriteData;
    spriteData._tag = "LoadSprite";
    spriteData._imagePath = "Resource/Texture/Title/Load.png";
    spriteData._layerRank = 101;
    m_pLoadBackObj = Master::m_pUIManager->GetSprite(*m_pRenderer, rectData, spriteData);
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_LoadProcess Class 
//*【?】終了
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_LoadProcess::OnExit(SceneManager *pOwner)
{
    //// ロード画面用スプライトオフ
    //m_pLoadBackSprite->get_OwnerObj().lock()->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
    if (m_IsMatLoad)
    {
        return;
    }


    // CSVからマテリアルデータの読み込み
    if (!Master::m_pResourceManager->ImportCSV_AllMaterialData("Resource/Excel_Param/MaterialParam.csv"))
    {
        MessageBox(NULL, "CSVの読み込みに失敗", "GameLoad", MB_OK);
        assert(false);
    }

    // マテリアルの作成 (今後CSVで読み込むようにする)
    {
        /* クモ */
        {
            Material mat[4];
            mat[0].m_DiffuseMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/Spinnen_Bein_tex.jpg");
            mat[0].m_NormalMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/haar_detail_NRM.jpg");
            mat[0].m_DiffuseColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[0].m_SpecularColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[0].m_SpecularPower = 150.0f;

            // マテリアル登録
            Master::m_pResourceManager->RegisterMaterialData("Spider_1", mat[0]);

            mat[1].m_DiffuseMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/Spinnen_Bein_tex.jpg");
            mat[1].m_DiffuseColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[1].m_SpecularColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[1].m_SpecularPower = 150.0f;

            // マテリアル登録
            Master::m_pResourceManager->RegisterMaterialData("Spider_2", mat[1]);

            mat[2].m_DiffuseMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/Spinnen_Bein_tex.jpg");
            mat[2].m_DiffuseColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[2].m_SpecularColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[2].m_SpecularPower = 150.0f;

            // マテリアル登録
            Master::m_pResourceManager->RegisterMaterialData("Spider_3", mat[2]);

            mat[3].m_DiffuseMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/SH3.png");
            mat[3].m_NormalMap.Texture = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Model/fbx/textures/haar_detail_NRM.jpg");
            mat[3].m_DiffuseColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[3].m_SpecularColor = VEC4(1.0f, 1.0f, 1.0f, 1.0f);
            mat[3].m_SpecularPower = 100.0f;

            // マテリアル登録
            Master::m_pResourceManager->RegisterMaterialData("Spider_4", mat[3]);
        }

        /* スカイボックス */
        {
            Material mat;
            mat.m_DiffuseMap.Texture = Master::m_pResourceManager->LoadDDS_CubeMap_Texture(L"Resource/Texture/cloudy_skybox.dds");

            // マテリアル登録
            Master::m_pResourceManager->RegisterMaterialData("SkyBox", mat);
        }
    }

    std::shared_ptr<GameObject> pPlayerObj;
    /* プレイヤー モデルの生成 */
    {
        // マテリアル取得
        auto matPtr1 = Master::m_pResourceManager->FindMaterial("Soldier_body");
        auto matPtr2 = Master::m_pResourceManager->FindMaterial("Soldier_head");

        SetupMaterialInfo matInfo[3];
        matInfo[0].Index = 0;
        matInfo[0].pMaterialData = matPtr1; // 体

        matInfo[1].Index = 1;
        matInfo[1].pMaterialData = matPtr1; // ヘルメット（体）

        matInfo[2].Index = 2;
        matInfo[2].pMaterialData = matPtr2; // 頭

        CreateModelInfo model;
        model.pRenderer = m_pRenderer;
        model.Path = "Resource/Model/Ranger/Swat_01.fbx";
        model.ObjTag = "Player";
        model.IsAnim = true;
        model.MatNum = 3;
        model.SetupMaterial = matInfo;
        model.ObjLayer = 90;
        model.Shadow_ShaderType = SHADER_TYPE::POST_SHADOWMAP;
        model.ShaderType = SHADER_TYPE::DEFERRED_STD_SKINNED_N;
        model.IsActive = false;
        pPlayerObj = MeshFactory::CreateModel(model);
        pPlayerObj->get_Component<MyTransform>()->set_Scale(1.0f, 1.0f, 1.0f);
        pPlayerObj->get_Component<SkinnedMeshAnimator>()->set_IsAnim(true);
        pPlayerObj->get_Component<SkinnedMeshAnimator>()->set_AnimIndex(INT_CAST(PlayerData::PLAYER_RANGER_ANIM_ID::RIFLE_AMING_IDLE));

        //// マテリアル取得
        //auto matPtr = Master::m_pResourceManager->FindMaterial("PlayerModel");

        //SetupMaterialInfo matInfo[1];
        //matInfo[0].Index = 0;
        //matInfo[0].pMaterialData = matPtr;

        //CreateModelInfo model;
        //model.pRenderer = m_pRenderer;
        //model.Path = "Resource/Model/Player2/AL_Standard.fbx";
        //model.ObjTag = "Player";
        //model.IsAnim = true;
        //model.MatNum = 1;
        //model.SetupMaterial = matInfo;
        //model.ShaderType = SHADER_TYPE::DEFERRED_STD_SKINNED_N;
        //model.Shadow_ShaderType = SHADER_TYPE::POST_SHADOWMAP;
        //model.ObjLayer = 90;
        //pPlayerObj = MeshFactory::CreateModel(model);
        //pPlayerObj->get_Component<MyTransform>()->set_Scale(0.1f, 0.1f, 0.1f);
        //pPlayerObj->get_Component<SkinnedMeshAnimator>()->set_IsAnim(true);
        //pPlayerObj->get_Component<SkinnedMeshAnimator>()->set_AnimIndex(0);

        // 破棄しない
        pPlayerObj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_DONT_DESTROY);

        // 動的オブジェクト
        pPlayerObj->set_State(OBJECT_STATE::DYNAMIC);

        pPlayerObj->get_Transform().lock()->set_Pos(-900.0f, 0.0f, 900.0f);
        //pPlayerObj->add_Component<PlayerController>(1);
        //pPlayerObj->get_Transform().lock()->set_RotateToDeg(0.0f, 0.0f, 0.0f);

        // 軌跡
        //auto trail = pPlayerObj->add_Component<TrailRenderer>();
        //trail->set_MinVertexDistance(4.0f);
        //trail->set_Width(5.0f);
        //trail->set_EmissivePower(2.0f);
        //trail->set_Color(VEC4(1.0f, 1.0f, 1.0f, 1.0f));

        // コライダーの追加
        auto collider = pPlayerObj->add_Component<BoxCollider>();
        collider->set_Size(VEC3(1.0f, 1.0f, 1.0f));
        collider->set_Center(VEC3(0.0f, 1.0f, 0.0f));

        // カテゴリ
        collider->set_CollisionCategory(COLLISION_CATEGORY::PLAYER);

        // 衝突マスクの設定
        collider->set_CollisionBitMask(UINT_CAST(COLLISION_CATEGORY::ENEMY) | UINT_CAST(COLLISION_CATEGORY::BUILDING) | UINT_CAST(COLLISION_CATEGORY::DESTRUCTION_BUILDING));

        auto health = pPlayerObj->add_Component<Health>();

        // コライダーの登録
        Master::m_pCollisionManager->RegisterCollider(collider);

        // カメラのフォーカスオブジェクトに設定
        m_pCameraComp->set_FocusObject(pPlayerObj);
    }

    /* ディレクションライトの生成(Cubuで分かりやすく) */
    {
        // マテリアル取得
        auto matPtr = Master::m_pResourceManager->FindMaterial("DirLight");

        SetupMaterialInfo matInfo[1];
        matInfo[0].Index = 0;
        matInfo[0].pMaterialData = matPtr;

        CreateUtilityMeshInfo mesh;
        mesh.pRenderer = m_pRenderer;
        mesh.Type = UTILITY_MESH_TYPE::CUBE;
        mesh.ObjTag = "DirLight";
        mesh.MatNum = 1;
        mesh.MaterialData = matInfo;

        auto obj = MeshFactory::CreateUtilityMesh(mesh);
        //auto obj = Instantiate3D(std::move(std::make_shared<GameObject>()));
        obj->set_Tag("DirLight");
        obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);
        obj->set_StatusFlag(OBJECT_STATUS_BITFLAG::IS_DONT_DESTROY);// 破棄しない
        auto light = obj->add_Component<DirectionalLight>();
        light->set_LightColor(VEC3(1.0f, 1.0f, 1.0f));
        light->set_Intensity(3.2f);
        light->set_LightCameraTrackingObj(m_pCameraComp->get_OwnerObj().lock());
        light->Start(*m_pRenderer);

        obj->get_Transform().lock()->set_Pos(VEC3(0.0f, 1000.0f, -1000.0f));
        obj->get_Transform().lock()->set_Scale(VEC3(30.0, 30.0, 80.0));
        obj->get_Transform().lock()->set_RotateToDeg(VEC3(30.0f, 150.0f, 0.0f));
    }

    ///* タイトル画面背景用スプライト1 */
    //{
    //    CreateSpriteInfo sprite;
    //    sprite.pTextureMap[0] = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Texture/Title/TitleBack.png");
    //    sprite.ObjTag = "TitleBack_Sp";
    //    sprite.pRenderer = m_pRenderer;
    //    sprite.ShaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    //    sprite.Type = SPRITE_USAGE_TYPE::NORMAL;
    //    sprite.Width = m_pRenderer->get_ScreenWidth();
    //    sprite.Height = m_pRenderer->get_ScreenHeight();
    //    sprite.IsActive = true;
    //    sprite.IsTransparent = true;
    //    auto obj = MeshFactory::CreateSprite(sprite);
    //    if (obj) {
    //        obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 0.0f);
    //    }
    //}
    ///* タイトル画面背景用スプライト2 */
    //{
    //    CreateSpriteInfo sprite;
    //    sprite.pTextureMap[0] = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Texture/Title/2221833.png");
    //    sprite.ObjTag = "TitleBack_Sp2";
    //    sprite.pRenderer = m_pRenderer;
    //    sprite.ShaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    //    sprite.Type = SPRITE_USAGE_TYPE::NORMAL;
    //    sprite.Width = 2000.0f;
    //    sprite.Height = 2300.0f;
    //    sprite.IsActive = true;
    //    sprite.IsTransparent = true;
    //    auto obj = MeshFactory::CreateSprite(sprite);
    //    if (obj) {
    //        obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 0.0f);
    //    }

    //    // もう一つ作る
    //    sprite.ObjTag = "TitleBack_Sp3";
    //    obj = MeshFactory::CreateSprite(sprite);
    //    if (obj) {
    //        obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 0.0f);
    //    }
    //}
    ///* タイトルロゴ用スプライト */
    //{
    //    CreateSpriteInfo sprite;
    //    sprite.pTextureMap[0] = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Texture/Title/Title_logo.png");
    //    sprite.ObjTag = "TitleLogo_Sp";
    //    sprite.pRenderer = m_pRenderer;
    //    sprite.ShaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    //    sprite.Type = SPRITE_USAGE_TYPE::NORMAL;
    //    sprite.Width = m_pRenderer->get_ScreenWidth();
    //    sprite.Height = m_pRenderer->get_ScreenHeight();
    //    sprite.IsActive = true;
    //    sprite.IsTransparent = true;
    //    auto obj = MeshFactory::CreateSprite(sprite);
    //    if (obj) {
    //        obj->get_Transform().lock()->set_Pos(0.0f, 0.0f, 0.0f);
    //    }
    //}

    /* ボタンテスト用 */
    //{
    //    CreateSpriteInfo sprite;
    //    sprite.pTextureMap[0] = Master::m_pResourceManager->LoadWIC_Texture(L"Resource/Texture/Title/Line.png");
    //    sprite.pRenderer = m_pRenderer;
    //    sprite.ShaderType = SHADER_TYPE::FORWARD_UNLIT_UI_SPRITE;
    //    sprite.Type = SPRITE_USAGE_TYPE::NORMAL;
    //    sprite.Width = 500.0f;
    //    sprite.Height = 200.0f;
    //    sprite.IsActive = true;
    //    sprite.IsTransparent = true;
    //    // 項目分生成
    //    for (int i = 0; i < 4; i++)
    //    {
    //        sprite.ObjTag = "MenuItem_Button_" + std::to_string(i + 1);
    //        auto obj = MeshFactory::CreateSprite(sprite);
    //        obj->get_RectTransform().lock()->set_RectPosition(VEC2(960.0f, 500.0f));

    //        // ボタンコンポーネントの追加
    //        auto button = obj->add_Component<ButtonUI>();
    //        button->set_Sprite(obj->get_Component<SpriteRenderer>());
    //        button->set_Text("ボタン");
    //        button->set_TextOffsetPos(VEC2(100.0f, 0.0f));
    //    }
    //}

	// ロード画面用スプライトオフ
    m_pLoadBackObj->clear_StatusFlag(OBJECT_STATUS_BITFLAG::IS_ACTIVE);

    m_IsMatLoad = true;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_LoadProcess Class 
//*【?】更新
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
int c_Title_LoadProcess::Update(SceneManager *pOwner)
{
    // ロード済みならメインメニューへ
    if (m_IsLoad)
    {
        return c_TITLE::c_TITLE_MAIN_MENU;
    }


	return c_TITLE::c_TITLE_LOAD_PROCESS;
}


//*---------------------------------------------------------------------------------------
//* @:c_Title_LoadProcess Class 
//*【?】描画
//* 引数：1.SceneManager
//* 返値：void
//*----------------------------------------------------------------------------------------
void c_Title_LoadProcess::Draw(SceneManager *pOwner)
{
    // すでにロードされているなら返す
    if (m_IsLoad)
    {
        return;
    }

	//Master::m_pDirectWriteManager->DrawString("☆ロード中", VECTOR2::VEC2(40.0f, 500.0f), "White_40_STD");

    // ロード完了
    m_IsLoad = true;
}
