#include "pch.h"
#include "DataManager.h"
#include "RendererEngine.h"
#include "ResourceManager.h"
#include "SceneStateEnums.h"
#include "Component_SpriteRenderer.h"
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

constexpr float VOLUME_SCALE = 0.01f;  // 音量のスケール（0.0f～1.0f）

//*---------------------------------------------------------------------------------------
//*【?】コンストラクタ
//*----------------------------------------------------------------------------------------
DataManager::DataManager():
m_ScreenWidth(0.0f),
m_ScreenHeight(0.0f),
m_DefaultFov(0.0f),
m_IsDebugMode(true),
m_pRenderer(nullptr),
m_SelectWeaponID()
{
}

//*---------------------------------------------------------------------------------------
//*【?】デストラクタ
//*----------------------------------------------------------------------------------------
DataManager::~DataManager()
{
}

//*---------------------------------------------------------------------------------------
//*【?】初期化
//*
//* [引数]なし
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
bool DataManager::Init(std::shared_ptr<RendererEngine>pRenderer)
{
	m_pRenderer = pRenderer;

	// 初期武器
	m_SelectWeaponID[0] = 0;
	m_SelectWeaponID[1] = 1;

	// ユーザー設定データの初期化
	m_UserConfigData._BGMVolume = 30.0f;
	m_UserConfigData._SEVolume = 50.0f;
	m_UserConfigData._isInvertY = false;
	m_UserConfigData._mouseSensitivity = 40.0f;
	m_UserConfigData._isShadowEnabled = true;

	Master::m_pSoundManager->set_Volume(SOUND_TYPE::BGM, m_UserConfigData._BGMVolume * VOLUME_SCALE);
	Master::m_pSoundManager->set_Volume(SOUND_TYPE::SE, m_UserConfigData._SEVolume * VOLUME_SCALE);

	return true;
}

//*---------------------------------------------------------------------------------------
//*【?】タイトルロードの開始時にセットするデータ
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
bool DataManager::SettingsData_TitleLoadOnEnter(RendererEngine& renderer)
{
	return true;
}

//*---------------------------------------------------------------------------------------
//*【?】タイトルロードの終了時にセットするデータ
//*
//* [引数]
//* &renderer : 描画エンジンの参照
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
bool DataManager::SettingsData_TitleLoadOnExit(RendererEngine& renderer)
{
	return true;
}

//*---------------------------------------------------------------------------------------
//*【?】ミッション開始時のデータセット
//*
//* [引数]
//* &renderer : 描画エンジンの参照 
//* _missionNumber : ミッション番号（0から）
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
bool DataManager::SettingsData_MissionLoad(RendererEngine& renderer, UINT _missionNumber)
{
	return true;
}

//*---------------------------------------------------------------------------------------
//*【?】ミッション終了時のデータセット
//*
//* [引数]
//* &renderer : 描画エンジンの参照 
//* _missionNumber : ミッション番号（0から）
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
bool DataManager::SettingsData_MissionTermination(RendererEngine& renderer, UINT _missionNumber)
{
	return true;
}

//*---------------------------------------------------------------------------------------
//*【?】カメラにFovをセットする
//*
//* [引数]
//* _fov : fov
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
void DataManager::set_Fov(float _fov)
{
	// 描画エンジンにFOVをセット
	if (m_pRenderer) {
		m_pRenderer->get_CameraComponent()->set_Fov(_fov);
	}
}

//*---------------------------------------------------------------------------------------
//*【?】カメラにFovをセットする
//*
//* [引数]
//* _fov : fov
//* [返値]
//* true  : 成功 
//* false : 失敗 
//*----------------------------------------------------------------------------------------
float DataManager::get_Fov()
{
	// 描画エンジンにFOVをセット
	if (m_pRenderer) {
		return m_pRenderer->get_CameraComponent()->get_Fov();
	}

	return 0.0f;
}

//*----------------------------------------------------------------------------------------
//*【?】BGM音量の設定
//*----------------------------------------------------------------------------------------
void DataManager::set_BGMVolume(int _vol)
{
	m_UserConfigData._BGMVolume = _vol;
	Master::m_pSoundManager->set_Volume(SOUND_TYPE::BGM, m_UserConfigData._BGMVolume * VOLUME_SCALE);
}

//*----------------------------------------------------------------------------------------
//*【?】SE音量の設定
//*----------------------------------------------------------------------------------------
void DataManager::set_SEVolume(int _vol)
{
	m_UserConfigData._SEVolume = _vol;
	Master::m_pSoundManager->set_Volume(SOUND_TYPE::SE, m_UserConfigData._SEVolume * VOLUME_SCALE);
}


//*----------------------------------------------------------------------------------------
//*【?】マウス感度の設定
//*----------------------------------------------------------------------------------------
void DataManager::set_MouseSensitivity(float _sens)
{
	m_UserConfigData._mouseSensitivity = _sens;
}

//*----------------------------------------------------------------------------------------
//*【?】Y反転の設定
//*----------------------------------------------------------------------------------------
void DataManager::set_IsInvertY(bool _isInv)
{
	m_UserConfigData._isInvertY = _isInv;
}

//*----------------------------------------------------------------------------------------
//*【?】シャドウの有無の設定
//*----------------------------------------------------------------------------------------
void DataManager::set_IsShadowEnabled(bool _isShadow)
{
	m_UserConfigData._isShadowEnabled = _isShadow;
}