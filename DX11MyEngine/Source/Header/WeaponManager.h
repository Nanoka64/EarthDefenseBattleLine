#pragma once

class WeaponBase;

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponManager Class --- */
//
// 【?】武器の管理
//      このクラスではプレイヤーがどの武器を持つか、などの切り替え処理などを担う
//      実際の発射処理は武器クラスに記述
//
// ***************************************************************************************
class WeaponManager
{
private:
    std::vector<std::shared_ptr<WeaponBase>> m_WeaponArray;    // 武器配列
    int m_CrntWeaponSlotIndex;  // 現在の武器スロットのインデックス
    int m_MaxSlot;              // 武器の最大スロット
public:
    WeaponManager();
    ~WeaponManager();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    /// <returns>成功/失敗</returns>
    bool Init(RendererEngine& renderer, int _maxSlot);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    void Update(RendererEngine& renderer);

    /// <summary>
    /// 武器の登録
    /// </summary>
    void RegisterWeapon(std::shared_ptr<class GunWeapon> _pWeapon, int _slot);

    /// <summary>
    /// 武器の切り替え
    /// </summary>
    /// <param name="_index"></param>
    void SwitchWeapon(int _index);

    /// <summary>
    /// 次の武器へ切り替え
    /// </summary>
    void NextWeapon();

    /// <summary>
    /// 前の武器へ切り替え
    /// </summary>
    void PrevWeapon();

private:
    // コピー禁止
    WeaponManager(const WeaponManager&) = delete;
    WeaponManager& operator=(const WeaponManager&) = delete;
};

