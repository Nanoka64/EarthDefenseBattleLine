#pragma once
#include "IComponent.h"

class WeaponBase;

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:WeaponController Class --- */
//
//  ★継承：IComponent ★
//
// 【?】武器の管理
//      このクラスではプレイヤーがどの武器を持つか、などの切り替え処理などを担う
//      実際の発射処理は武器クラスに記述
//
// ***************************************************************************************
class WeaponController : public IComponent
{
private:
    std::vector<std::shared_ptr<WeaponBase>> m_WeaponArray;    // 武器配列
    int m_CrntWeaponSlotIndex;  // 現在の武器スロットのインデックス
    int m_MaxSlot;              // 武器の最大スロット
    int m_WeaponChangeIntervalCounter; // 武器の切り替え間隔用のカウンタ


    std::vector<class GameObject*> m_pBulletGageSpriteObjArray; // 弾ゲージスプライト
    std::vector<class GameObject*> m_pWeaponSpriteObjArray; // 武器

    class GameObject* m_pReloadBarSpriteObj;		// リロードバー
    class GameObject* m_pReloadBarBackSpriteObj;	// リロードバー背景
    const VECTOR2::VEC2 RELOAD_BAR_POS = VECTOR2::VEC2(-100.0f, -10.0f);	 // リロードバー位置（真ん中が中心にしてるので、サイズの半分引く）
    const VECTOR2::VEC2 RELOAD_BAR_SIZE = VECTOR2::VEC2(200.0f, 20.0f);		 // リロードバーサイズ
    const VECTOR2::VEC2 RELOAD_BAR_BACK_POS = VECTOR2::VEC2(0.0f, 0.0f);	 // リロードバー背景位置
    const VECTOR2::VEC2 RELOAD_BAR_BACK_SIZE = VECTOR2::VEC2(210.0f, 30.0f); // リロードバー背景サイズ
    const VECTOR2::VEC2 RELOAD_TEXT_POS = VECTOR2::VEC2(0.0f, 0.0f);	 // リロードテキスト位置


public:
    WeaponController(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
    ~WeaponController();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    /// <returns>成功/失敗</returns>
    bool Setup(RendererEngine& renderer, int _maxSlot);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    void Update(RendererEngine& renderer)override;

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="renderer">描画エンジンの参照</param>
    void Draw(RendererEngine& renderer)override;

    /// <summary>
    /// 武器の登録
    /// </summary>
    void RegisterWeapon(std::shared_ptr<class GunWeapon> _pWeapon, int _slot);


    /// <summary>
    /// 開始時に持っている武器
    /// </summary>
    void StartingWeapon(int _slot);

    /// <summary>
    /// 現在、装備している武器のスロットインデックス取得
    /// </summary>
    /// <returns></returns>
    int get_CrntWeaponSlotIndex()const { return m_CrntWeaponSlotIndex; }

    /// <summary>
    /// 現在装備している武器がリロード中か
    /// </summary>
    /// <returns></returns>
    bool get_IsCrntWeaponReloading();

    /// <summary>
    /// 武器をクリア
    /// </summary>
    void ClearWeapon() ;

private:
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

    void SetupReloadSprite(RendererEngine& renderer);
    void SetupWeaponSprite(RendererEngine& renderer);
};

