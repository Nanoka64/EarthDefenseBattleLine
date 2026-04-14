#pragma once
#include "SceneStateEnums.h"

/// <summary>
/// 汎用データ
/// </summary>
namespace UtilityData
{
	/// <summary>
	/// 派閥
	/// </summary>
	enum class FACTION
	{
		PLAYER,		// プレイヤー
		ALLY,		// 味方
		ENEMY,		// 敵
		NEUTRAL,	// 中立
		VEHICLE,	// 乗り物
	};

	struct FactionInfo
	{
		FACTION _faction;
	};


	enum class COLLIDER_TYPE
	{
		NONE,
		BOX,
		SPHERE,
		RAY,
	};

	/// <summary>
	/// 衝突判定のビット分け
	/// </summary>
	enum class COLLISION_CATEGORY : unsigned
	{
		NONE					= 0,
		PLAYER					= 1 << 0,	// プレイヤー
		PLAYER_BULLET			= 1 << 1,	// プレイヤーの弾
		ENEMY					= 1 << 2,	// 敵
		ENEMY_BULLET			= 1 << 3,	// 敵の弾
		DESTRUCTION_BUILDING	= 1 << 4,	// 破壊可能な建物
		BUILDING				= 1 << 5,	// 破壊不可能な建物

		EVERY = 0xFFFFFFFF	// 全てに衝突
	};

	// 文字列をEnumに変換するマップ
	static std::map<std::string, COLLISION_CATEGORY> g_CollisionCategoryMap = {
		{"PLAYER",				 COLLISION_CATEGORY::PLAYER},
		{"PLAYER_BULLET",		 COLLISION_CATEGORY::PLAYER_BULLET},
		{"ENEMY",				 COLLISION_CATEGORY::ENEMY},
		{"ENEMY_BULLET",		 COLLISION_CATEGORY::ENEMY_BULLET},
		{"DESTRUCTION_BUILDING", COLLISION_CATEGORY::DESTRUCTION_BUILDING},
		{"BUILDING",			 COLLISION_CATEGORY::BUILDING},
		{"EVERY",				 COLLISION_CATEGORY::EVERY},
	};


	/// <summary>
	/// タイトル項目
	/// </summary>
	enum class TITLEMENU_ITEM
	{
		MISSION_SELECT,	// ミッション選択
		SOLDER_SELECT,	// 兵科選択
		CONFIG,			// 設定
		EXIT,			// 終了

		NUM,
	};



	/// <summary>
	/// 兵科
	/// </summary>
	enum class SOLDIER_TYPE
	{
		RANGER,	// レンジャー

		// WING,
		// AIR,
		// F,
	};

	/// <summary>
	/// 設定項目
	/// </summary>
	enum class CONFIG_ITEM
	{
		BGM_VOLUME,
		SE_VOLUME,
		MOUSE_SENSITIVITY,
		INVERT_Y,
		SHADOW_ENABLED,

		NUM
	};

	/// <summary>
	/// 共用体の値の種類
	/// </summary>
	enum class VALUE_TYPE
	{
		BOOL,
		INT,
		FLOAT,
	};

	/// <summary>
	/// メニューの項目
	/// </summary>
	struct MenuItemInfo
	{
		VECTOR2::VEC2 _pos;	// 項目の位置
		const char* _name;	// 項目名
		bool _isHovered;	// マウスが上に乗っているか

		MenuItemInfo() :
			_pos(VECTOR2::VEC2()),
			_name(),
			_isHovered(false)
		{
		};

		MenuItemInfo(VECTOR2::VEC2 pos, const char* name)
			: _pos(pos), _name(name), _isHovered(false) {
		}
	};


	/// <summary>
	/// メインメニュー項目
	/// </summary>
	struct MainMenuItemInfo : public MenuItemInfo
	{
		TITLEMENU_ITEM _type;
		SceneStateEnums::c_TITLE _nextState;// 選択した際の遷移先ステート
		
		MainMenuItemInfo() :
			_type(UtilityData::TITLEMENU_ITEM::NUM),
			_nextState(SceneStateEnums::c_TITLE::c_TITLE_MAIN_MENU)
		{
		};
	};

	/// <summary>
	/// 設定メニュー項目
	/// </summary>
	struct ConfigMenuItemInfo : public MenuItemInfo
	{
		CONFIG_ITEM _type;			// 設定項目の種類（BGM音量、SE音量、マウス感度、カメラのY反転の有無、シャドウの有無）
		VALUE_TYPE _valueType;		// 値の種類（整数、浮動小数点数、真偽値）
		std::variant<bool, int, float> _value;	// 設定の値（値の種類に応じてbool、int、floatのいずれかを使用）


		ConfigMenuItemInfo() :
			_type(UtilityData::CONFIG_ITEM::NUM),
			_valueType(VALUE_TYPE::INT)
		{
		};

		// 全ての値を設定するためのコンストラクタ（追加）
		ConfigMenuItemInfo(VECTOR2::VEC2 pos, const char* name, CONFIG_ITEM type, VALUE_TYPE vType, std::variant<bool, int, float> val)
			: MenuItemInfo(pos, name), // 親クラスを初期化
			_type(type),
			_valueType(vType),
			_value(val) {
		}

		/// <summary>
		/// 値を設定する
		/// </summary>
		/// <param name="val"></param>
		void SetValue(std::variant<bool, int, float> val)
		{
			_value = val;
		}

		/// <summary>
		/// 共用体の値を文字列として表示できるようにする
		/// </summary>
		/// <returns></returns>
		std::string ConvertValueAsString() const
		{
			switch (_valueType)
			{
			case VALUE_TYPE::BOOL:
				return std::get<bool>(_value) ? "ON" : "OFF";
			case VALUE_TYPE::INT:
				return std::to_string(std::get<int>(_value));
			case VALUE_TYPE::FLOAT:
				return Tool::WStringToString(Tool::FormatFloat(std::get<float>(_value)));
			default:
				return "";
			}
		}
	};


	/// <summary>
	/// ユーザーの設定データ
	/// </summary>
	struct UserConfigData
	{
		int _BGMVolume;				// BGMの音量 : 100.0が最大（実際はさらにスケールがかけられ、小さくなる）
		int _SEVolume;				// SEの音量 : 100.0が最大（実際はさらにスケールがかけられ、小さくなる）
		float _mouseSensitivity;	// マウス感度 : 100.0が最大（実際はさらにスケールがかけられ、小さくなる）
		bool _isInvertY;			// Y軸反転の有無
		bool _isShadowEnabled;		// シャドウの有効/無効

		UserConfigData() :
			_BGMVolume(50),
			_SEVolume(70),
			_mouseSensitivity(40.0f), 
			_isInvertY(false),
			_isShadowEnabled(true)
		{
		}
	};


	/// <summary>
	/// 移動挙動の種類
	/// </summary>
	enum class MOVE_BEHAVIOUR_TYPE : unsigned char
	{
		NONE,

		LINEAR,     // 直線移動
		HOMING,     // ホーミング移動
	};




};