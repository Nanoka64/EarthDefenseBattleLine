#pragma once



// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:ObjectFactory  --- */
//
// 【?】オブジェクトの種類を選択して、ある程度簡潔に外から生成できるようにする
//
// ***************************************************************************************
namespace ObjectFactory {

    /// <summary>
    /// 生成するオブジェクトの情報
    /// </summary>
    struct BaseCreateObjectInfo
    {
        std::string _tag = "GameObject";
    };





    /// <summary>
    /// 空のオブジェクトの生成
    /// </summary>
    /// <returns>空オブジェクト</returns>
    static std::shared_ptr<GameObject> CreateEmpty();

    /// <summary>
    /// 3Dオブジェクト生成関数群
    /// </summary>
    namespace Primitive3D
    {
        /// <summary>
        /// キューブの生成
        /// </summary>
        /// <returns>キューブ</returns>
        static std::shared_ptr<GameObject> CreateCube();

        /// <summary>
        /// 平面の生成
        /// </summary>
        /// <returns>平面</returns>
        static std::shared_ptr<GameObject> CreatePlane();

        /// <summary>
        /// 平面の生成
        /// </summary>
        /// <returns>平面</returns>
        static std::shared_ptr<GameObject> CreateQuad();

        /// <summary>
        /// 3Dモデルの生成
        /// </summary>
        /// <returns>3Dモデル</returns>
        static std::shared_ptr<GameObject> CreateModel();
    };

    /// <summary>
    /// ライト生成関数群
    /// </summary>
    namespace Light
    {
        /// <summary>
        /// ポイントライトの生成
        /// </summary>
        /// <returns>ポイントライト</returns>
        static std::shared_ptr<GameObject> CreatePointLight();

        /// <summary>
        /// ディレクションライトの生成
        /// </summary>
        /// <returns>ディレクションライト</returns>
        static std::shared_ptr<GameObject> CreateDirectionalLight();
    };

    /// <summary>
    /// UI生成関数群
    /// </summary>
    namespace UI
    {
        /// <summary>
        /// スプライトの生成
        /// </summary>
        /// <returns>スプライト</returns>
        static std::shared_ptr<GameObject> CreateSprite();

        /// <summary>
        /// ボタンの生成
        /// </summary>
        /// <returns>ボタン</returns>
        static std::shared_ptr<GameObject> CreateButton();
    };
}
