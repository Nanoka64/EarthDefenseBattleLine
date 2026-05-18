// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
/* - @:ピクセルシェーダ -*/
//
//  【?】フォワードシェーディング
//       ディストーションの描画
//
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
#pragma once
#include "ConstantBuffers_H.hlsli"
SamplerState g_sSampler : register(s0);
Texture2D g_tSceneTexture : register(t0); // シーンテクスチャ
Texture2D g_tNoizeTexture : register(t1); // ノイズテクスチャ


/* =========================================================================
/* - @:入力構造体 - */
/* =========================================================================*/
struct PS_IN
{
    float4 Pos       : SV_Position;
    float4 PosInProj : TEXCOORD0; // 投影空間の頂点座標
    float2 UV        : TEXCOORD1;
};


// **************************************************************************
/* - @:エントリーポイント - */
// **************************************************************************
float4 PSMain(PS_IN input) : SV_TARGET
{
    float4 finalColor = float4(0.0, 0.0, 0.0, 1.0);
   
    // ノイズテクスチャを使ってUVを歪ませる
    float2 noiseUV = input.UV + float2(0, cb_DistortionTime * 0.5f);
    float3 noise = g_tNoizeTexture.Sample(g_sSampler, noiseUV).rgb;
    
    // 0～1のノイズを -1～1 に変換して強度を調整
    float2 offset = (noise.xy * 2.0f - 1.0f) * cb_DistortionPower;
    
    // シーンテクスチャを歪ませたUVでサンプリング
    finalColor = g_tSceneTexture.Sample(g_sSampler, input.PosInProj.xy + offset);
    
    return finalColor;
}