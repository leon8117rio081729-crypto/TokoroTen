#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "EffectManager.h"
#include "Effect.h"


EffectManager* EffectManager::m_Instance = nullptr;

EffectManager::EffectManager()
	: m_EffectHandles{}
	, m_Effects{}
	, m_IsActive(false)
{
}

EffectManager::~EffectManager()
{
	Fin();
}

void EffectManager::Setup()
{
	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);
}

bool EffectManager::Init()
{
	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(EFFEKSEER_MAX_PARTICLE) == -1)
	{
		m_IsActive = false;
		return false;
	}

	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	m_IsActive = true;
	return true;
}

void EffectManager::Load()
{
	if (!m_IsActive) return;

	// ファイルパス群
	const char* PATH[] =
	{
		// ※順番をEffectTypeに合わせること
		"Data/Effect/EnemyKilled.efkefc",
		"Data/Effect/PlayerMove.efkefc",
		"Data/Effect/PlayerUPMove.efkefc",
		"Data/Effect/PlayerDOWNMove.efkefc",
		"Data/Effect/PlayerForwardMove.efkefc",
		"Data/Effect/CrossSlash.efkefc",
		"Data/Effect/Book.efkefc",
		"Data/Effect/Ghost_Possessed.efkefc",
		"Data/Effect/HammerUsed.efkefc",
		"Data/Effect/LadderUsed.efkefc",
	};

	for (const char* path : PATH)
	{
		int handle = LoadEffekseerEffect(path);
		if (handle != -1)
		{
			m_EffectHandles.push_back(handle);
		}
	}
}

void EffectManager::Start()
{
}

Effect* EffectManager::PlayEffect(int type, VECTOR pos)
{
	if (!m_IsActive) return nullptr;
	if (type < 0 || type >= (int)m_EffectHandles.size()) return nullptr;

	// 未使用のものがあれば使いまわす
	for (Effect* effect : m_Effects)
	{
		if (!effect->IsActive())
		{
			// アクティブにする
			effect->SetActive(true);

			// エフェクト再生
			effect->Play(m_EffectHandles[type]);

			// 位置設定
			effect->SetPos(pos);

			// 再生するエフェクトを返却
			return effect;
		}
	}

	// 未使用のものがなければ新しく作る
	Effect* effect = new Effect;
	// アクティブにする
	effect->SetActive(true);
	// エフェクト再生
	effect->Play(m_EffectHandles[type]);
	// 位置設定
	effect->SetPos(pos);
	// 配列に追加
	m_Effects.push_back(effect);

	return effect;
}

void EffectManager::StopAllEffects()
{
	for (Effect* effect : m_Effects)
	{
		if (effect && effect->IsActive())
		{
			effect->Stop();
		}
	}
}

void EffectManager::Step()
{
	// マウス座標を3D座標にする
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);
	VECTOR mousePos2D = { (float)mouseX, (float)mouseY, 0.995f };
	VECTOR mousePos3D = ConvScreenPosToWorldPos(mousePos2D);


	// 各エフェクトを更新
	for (Effect* effect : m_Effects)
	{
		effect->Step();
	}
}

void EffectManager::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// 各エフェクトを更新
	for (Effect* effect : m_Effects)
	{
		effect->Update();
	}

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
}

void EffectManager::Draw()
{
	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
}

void EffectManager::Fin()
{
	// ロードしたものを削除
	for (int handle : m_EffectHandles)
	{
		DeleteEffekseerEffect(handle);
	}
	// 動的配列をクリア
	m_EffectHandles.clear();
	m_EffectHandles.shrink_to_fit();

	// 生成されたエフェクトを全て削除
	for (Effect* effect : m_Effects)
	{
		delete effect;
	}
	// 動的配列をクリア
	m_Effects.clear();
	m_Effects.shrink_to_fit();

	// Effekseerを終了する。
	Effkseer_End();
	m_IsActive = false;
}
