#include "DxLib.h"
#include "ShadowMap.h"

// シャドウマップ解像度
constexpr int SHADOW_MAP_SIZE_X = 4096;
constexpr int SHADOW_MAP_SIZE_Y = 4096;
// シャドウマップ描画範囲
#define SHADOW_MAP_AREA_MIN VGet(-100.0f, 0.0f, -100.0f)
#define SHADOW_MAP_AREA_MAX VGet(300.0f, 1.0f, 300.0f)

ShadowMap* ShadowMap::m_Instance = nullptr;

ShadowMap::ShadowMap()
	:m_Handle(0)
{
}

ShadowMap::~ShadowMap()
{
	Fin();
}

void ShadowMap::Init()
{
	// シャドウマップを作成
	m_Handle = MakeShadowMap(SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y);

	// ライトの向きを取得
	VECTOR lightDirection = GetLightDirection();

	// 影が出るライトの向きを設定
	SetShadowMapLightDirection(m_Handle, lightDirection);

	// 影を表示させる範囲を設定
	SetShadowMapDrawArea(m_Handle, SHADOW_MAP_AREA_MIN, SHADOW_MAP_AREA_MAX);
}

void ShadowMap::Fin()
{
	DeleteShadowMap(m_Handle);
}

void ShadowMap::StartDrawShadowMap()
{
	// シャドウマップへの描画を開始する
	// 終了までに描画されたモデルの影がシャドウマップに描画される
	ShadowMap_DrawSetup(m_Handle);
}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::StartAppearsShadowMap()
{
	// シャドウマップの映しこみを開始する
	// 終了までに描画されるモデルには
	// シャドウマップに映っている影が映る
	SetUseShadowMap(0, m_Handle);
}

void ShadowMap::EndAppearsShadowMap()
{
	// これ以上映すモデルがない場合は-1を渡して終了する
	SetUseShadowMap(0, -1);
}
