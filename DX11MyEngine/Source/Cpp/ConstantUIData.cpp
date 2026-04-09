#include "pch.h"
#include "ConstantUIData.h"
#include "Component_RectTransform.h"
#include "Component_SpriteRenderer.h"
#include "Component_ButtonUI.h"

/// <summary>
/// UIトランスフォームのパラメータを一括設定
/// </summary>
/// <param name="_out">パラメータを設定するUIトランスフォーム</param>
/// <param name="_param">パラメータ</param>
void UIData::RectTransformData::SetRectTransformData(RectTransform &_out, const RectTransformData &_param)
{
    _out.set_RectPosition(_param._pos);
    _out.set_Size(_param._size.x, _param._size.y);
    _out.set_RotateToDeg(0.0f, 0.0f, _param._rotZ);
    _out.set_AnchorMin(_param._anchorMin);
    _out.set_AnchorMax(_param._anchorMax);
    _out.set_Pivot(_param._pivot);
};


/// <summary>
/// スプライト情報の設定
/// </summary>
/// <param name="_out">パラメータを設定するスプライト</param>
/// <param name="_param">パラメータ</param>
void UIData::SpriteUIData::SetSpriteData(SpriteRenderer &_out, const UIData::SpriteUIData &_param)
{
	// パスが空でない場合はテクスチャの読み込みをする 
    // 空なら、テクスチャは設定せずにカラーのみ設定する
    if (_param._imagePath.empty() == false && _param._shaderType != SHADER_TYPE::FORWARD_UNLIT_UI_NOTEXTURE_SPRITE)
    {
        // テクスチャの読み込み
        auto texture = Master::m_pResourceManager->LoadWIC_Texture(Tool::StringToWstring(_param._imagePath));
        _out.set_UVOffset(_param._UVOffset);
        if (texture != nullptr) {
            _out.set_Texture(texture, 0);
        }
    }
    _out.set_ShaderType(_param._shaderType);
    _out.set_Color(_param._color);
}

/// <summary>
/// ボタン情報の設定
/// </summary>
/// <param name="_out">パラメータを設定するボタン</param>
/// <param name="_param">パラメータ</param>
void UIData::ButtonUIData::SetButtonData(ButtonUI &_out, const UIData::ButtonUIData &_param)
{
    _out.OnClickFunc(_param._onClicFunction, _param._inputValidationState);
    _out.set_Sprite(_param._pSprite);
    _out.set_Text(_param._text);
    _out.set_TextOffsetPos(_param._textOffsetPos);
    _out.set_InputSoundID(_param._inputSoundID);
}
