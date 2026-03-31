#pragma once
//--------------------------------------------------------------------------------------
//      * Includes *
//--------------------------------------------------------------------------------------
#include "InputConfig.h"

enum class INPUT_FLAG_TYPE
{
    DOWN,       // 押された瞬間
    UP,         // 離された瞬間
    HOLD,       // 長押し
    HOLD_REPEAT // 長押し（入力判定になるまでのフレーム指定用）
};

// =======================================================================================
//
// *---        InputManager Class         ---*
//         * シングルトンパターン *
// 
//			     入力情報の管理
//          入力処理を取る際は基本的に 
//      InputFactory.h using namespace Input
//                                      から取る
//           このクラスからは呼ばない
// 参考サイト：https://yttm-work.jp/directx/directx_0028.html
//           ：https://cammy.co.jp/technical/%e3%83%9e%e3%82%a6%e3%82%b9%e3%81%ae%e7%8a%b6%e6%85%8b%e5%8f%96%e5%be%97directinput/
// =======================================================================================
class InputManager
{
private:
    // DirectInput用
    LPDIRECTINPUT8   m_pDInput = nullptr;

    // 入力デバイスへのアクセスに使う用らしい
    LPDIRECTINPUTDEVICE8 m_pKeyDevice = nullptr;    // キーボード用
    LPDIRECTINPUTDEVICE8 m_pMouseDevice = nullptr;  // マウス用

    /// <summary>
    /// コンフィグ情報
    /// </summary>
    struct ConfigInfo
    {
        MOUSE_BUTTON_STATE _mouse;
        //KEY_STATE _key;
        int _key;
        GAMEPAD_STATE _pad;

        //int _count = 0;

        ConfigInfo() :
            _key(-1),
            _mouse(MOUSE_BUTTON_STATE::NONE),
            _pad(GAMEPAD_STATE::NONE)
        {
        }
    };

    // 必要なキーと入力フレームを連想配列で管理
    //std::unordered_map<enum class CONFIG_INPUT,  int> m_PrevKeyState;       // ひとつ前に押されていたキーの状態
    //std::unordered_map<enum class CONFIG_INPUT,  int> m_CrntKeyState;       // 現在押されているキーの状態
    //std::unordered_map<enum class CONFIG_INPUT,  int> m_ConfigKeyMap;       // コンフィグ管理用

    std::unordered_map<enum class SYSTEM_CONFIG, ConfigInfo> m_SystemConfigKeyMap;  // システム用コンフィグ管理

    std::unordered_map<enum class GAME_CONFIG, ConfigInfo> m_GameConfigMap;         // ゲームシーン用コンフィグ管理
    std::unordered_map<enum class GAME_CONFIG, int>m_CrntGameConfigCountersMap;
    std::unordered_map<enum class GAME_CONFIG, int>m_PrevGameConfigCountersMap;

    INPUT_TYPE m_CrntInputType;     // 現在の入力方式
    int m_InputStopTime = 0;        // 入力を受け付けない時間
    bool m_InputStopFlag = false;   // 入力を受け付けないフラグ
    POINT m_MousePos;               // マウス座標


    /// <summary>
    /// マウス情報
    /// </summary>
    struct MouseState{
        // rgbButtons[4]
        // [0] = 左クリック
        // [1] = 右クリック
        // [2] = ホイールクリック
        // [3] = その他
        DIMOUSESTATE _state; 
        int _count[static_cast<int>(MOUSE_BUTTON_STATE::NUM)];  // 押されている時間のカウント(ボタンの個数分)
    };

    MouseState m_CrntMouseState;  // 現在のマウス状態
    MouseState m_PrevMouseState;  // ひとつ前のマウス状態

public:
    InputManager() = default;
    ~InputManager();

    bool Init(HWND hWnd);
    void Update();
    void Term();

    /* 入力情報の取得メソッド */
    bool GetInput(GAME_CONFIG key);        // キーが押されているか
    bool GetInputDown(GAME_CONFIG key);    // キーが押された瞬間
    bool GetInputUp(GAME_CONFIG key);      // キーが離された瞬間
    bool GetInputHold(GAME_CONFIG key, int repeatFrame);      // 入力判定になるまでのフレーム指定用
    bool GetInputHoldRepeat(GAME_CONFIG key, int waitFrame, int repeatFrame);   // 長押しの場合、連続入力を阻止する用
    
    bool GetMouseClick(MOUSE_BUTTON_STATE _button)const;       // マウスボタンが押されているか
    bool GetMouseClickDown(MOUSE_BUTTON_STATE _button)const;   // 押された瞬間
    bool GetMouseClickUp(MOUSE_BUTTON_STATE _button)const;     // 離された瞬間
    bool GetMouseClickHoldRepeat(MOUSE_BUTTON_STATE _button, int _waitFrame, int _repeatFrame, bool _isStartPushJudge = false)const;     // 入力判定になるまでのフレーム指定用
    
    inline POINT GetMousePos()const { return m_MousePos; };   // マウス座標の取得
    inline LONG GetMousePosSlopeX()const{return m_CrntMouseState._state.lX;};   // Xの移動量の差の取得
    inline LONG GetMousePosSlopeY()const{return m_CrntMouseState._state.lY;};   // Yの移動量の差の取得
    inline LONG GetMousePosSlopeZ()const{return m_CrntMouseState._state.lZ;};   // Zの移動量の差の取得
    inline int GetCrntMouseClickCount(MOUSE_BUTTON_STATE _button)const { return m_CrntMouseState._count[static_cast<int>(_button)]; }   // 現在のマウスのクリックカウントを取得
    inline int GetPrevMouseClickCount(MOUSE_BUTTON_STATE _button)const { return m_PrevMouseState._count[static_cast<int>(_button)]; }   // ひとつ前のマウスのクリックカウントを取得
    
    /* ---------------------- */



    void InitDefaultKeyConfig();            // コンフィグデフォルト設定

	void StopInput(int stopTime);    // 入力を一定時間受け付けないようにする

private:
    // コピー禁止
    InputManager(const InputManager &) = delete;
    InputManager &operator=(const InputManager &) = delete;
    // ------------------------------------------------------

    bool SetupKeyDevice(HWND hWnd);
    bool SetupMouseDevice(HWND hWnd);
    bool SetupPadDevice(HWND hWnd);  // 追加予定


    bool KeyDeviceProcess();
    bool MouseDeviceProcess();

};

