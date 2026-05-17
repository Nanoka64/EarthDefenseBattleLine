#pragma once
#include "IComponent.h"
#include "IMoveBehaviour.h"
#include "ConstantUtilityData.h"


// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:MoveLogic Class --- */
//
//  ★継承：Component ★
//
// 【?】移動ロジック
//      ビヘイビアを切り替えることで、移動の挙動を変えることができる
//
// ***************************************************************************************
class MoveLogic : public IComponent
{
private:
    std::array<std::unique_ptr<IMoveBehaviour>, static_cast<size_t>(UtilityData::MOVE_BEHAVIOUR_TYPE::NUM)> m_pMoveBehaviourMap;    // 移動挙動のマップ
    IMoveBehaviour *m_pMoveBehaviour = nullptr;	// 現在の移動挙動
    VECTOR3::VEC3 m_CrntMoveVelocity;           // 現在の移動ベクトル
    float m_GravityVelocity;
	MoveParam m_MoveParam;                      // 移動パラメータ

public:
    MoveLogic(std::weak_ptr<GameObject> pOwner, int updateRank = 100);
    ~MoveLogic();

    void Start(RendererEngine &renderer) override;		// 初期化
    void Update(RendererEngine &renderer) override;		// 更新
	void set_MoveParam(const MoveParam& _param) { m_MoveParam = _param; }	// 移動パラメータの設定

    /// <summary>
    /// パラメータのリセット
    /// </summary>
    void ParamReset();

    /// <summary>
    /// 移動挙動の登録
    /// </summary>
    /// <param name="_type">移動挙動の種類</param>
    void Register(UtilityData::MOVE_BEHAVIOUR_TYPE _type);

    /// <summary>
    /// 移動挙動の変更
    /// </summary>
    /// <param name="_type">変更する移動挙動の種類</param>
    void ChangeBehaviour(UtilityData::MOVE_BEHAVIOUR_TYPE _type);

private:
    void Calculate(const struct MoveParam& _param);		    // 移動処理
};

