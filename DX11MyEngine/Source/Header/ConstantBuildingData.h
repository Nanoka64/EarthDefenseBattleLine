#pragma once

namespace BuildingData
{
    /* 建物の種類 */
    enum class BUILDING_TYPE_ID
    {
        BUILDING_TYPE_ID_GROUND_ROAD_1,	// 道路	
        BUILDING_TYPE_ID_BUILDING_1,

        BUILDING_TYPE_ID_MAX,
    };

    /* マップの保存する情報*/
    struct BUILDING_SAVE_DATA
    {
        BUILDING_TYPE_ID _id;	// オブジェクトの種類
        VECTOR3::VEC3 _pos;		// 座標
        VECTOR3::VEC3 _rotate;	// 回転
        VECTOR3::VEC3 _scale;	// スケール
    };

    /* マップの種類 */
    enum class MAP_TYPE_ID
    {
        TOWN_1,	// 街 1

        NUM,
    };

    /* 建物の状態 */
    enum BUILDING_STATE
    {
        BUILDING_STATE_IDLE,			// 通常
        BUILDING_STATE_CLLAPSE_IN,		// 倒壊始め
        BUILDING_STATE_CLLAPSE_NOW,		// 倒壊中...
        BUILDING_STATE_CLLAPSE_END,		// 倒壊終了
        BUILDING_STATE_FALL,			// 落下
        BUILDING_STATE_END,				// 終了

        BUILDING_STATE_HIT,				// 被弾
    };
};
