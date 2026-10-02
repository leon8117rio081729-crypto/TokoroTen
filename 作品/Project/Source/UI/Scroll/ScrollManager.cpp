#include <DxLib.h> 
#include "ScrollManager.h"
#include <cmath>
#include "../../Color/Color.h"


ScrollManager* ScrollManager::m_Instance = nullptr;

void ScrollManager::CreateInstance()
{
	if (!m_Instance) m_Instance = new ScrollManager();
}

ScrollManager* ScrollManager::GetInstance()
{
	return m_Instance;
}

void ScrollManager::DeleteInstance()
{
	if (m_Instance)
	{
		m_Instance->Fin();
		delete m_Instance;
		m_Instance = nullptr;
	}
}

ScrollManager::ScrollManager()
	: m_BackgroundHandle(-1)
	, m_ImgW(0)
	, m_ImgH(0)
	, m_OffsetX(0.0f)
	, m_OffsetY(0.0f)
	, m_SpeedX(0.0f)
	, m_SpeedY(0.0f)
	, m_Active(false)
	, m_TileX(true)
	, m_TileY(true)
{
}

ScrollManager::~ScrollManager()
{
	Fin();
}

void ScrollManager::Init()
{
}

void ScrollManager::Fin()
{
	if (m_BackgroundHandle >= 0)
	{
		DeleteGraph(m_BackgroundHandle);
		m_BackgroundHandle = -1;
	}
	m_ImgW = m_ImgH = 0;
	m_OffsetX = m_OffsetY = 0.0f;
	m_Active = false;
}

bool ScrollManager::LoadBackground(const char* path)
{
	if (!path) return false;
	int h = LoadGraph(path);
	if (h < 0) return false;
	// 既にハンドルがあるなら置き換え
	if (m_BackgroundHandle >= 0)
	{
		DeleteGraph(m_BackgroundHandle);
	}
	m_BackgroundHandle = h;
	// 画像サイズ取得
	GetGraphSize(m_BackgroundHandle, &m_ImgW, &m_ImgH);
	if (m_ImgW <= 0 || m_ImgH <= 0) return false;
	return true;
}

void ScrollManager::SetBackgroundHandle(int handle)
{
	if (m_BackgroundHandle >= 0)
	{
		DeleteGraph(m_BackgroundHandle);
	}
	m_BackgroundHandle = handle;
	if (m_BackgroundHandle >= 0)
	{
		GetGraphSize(m_BackgroundHandle, &m_ImgW, &m_ImgH);
	}
	else
	{
		m_ImgW = m_ImgH = 0;
	}
}

void ScrollManager::SetSpeed(float vx, float vy)
{
	m_SpeedX = vx;
	m_SpeedY = vy;
}

void ScrollManager::SetActive(bool active)
{
	m_Active = active;
}

void ScrollManager::SetTile(bool tileX, bool tileY)
{
	m_TileX = tileX;
	m_TileY = tileY;
}

void ScrollManager::Update()
{
	if (!m_Active) return;
	// 単純にフレームごとのピクセル移動量で更新
	m_OffsetX += m_SpeedX;
	m_OffsetY += m_SpeedY;

	// 適切に巻き戻す（画像幅/高さが 0 の場合は無効化）
	if (m_ImgW > 0)
	{
		// fmod を使い負の値も処理
		m_OffsetX = std::fmod(m_OffsetX, static_cast<float>(m_ImgW));
		if (m_OffsetX < 0.0f) m_OffsetX += m_ImgW;
	}
	if (m_ImgH > 0)
	{
		m_OffsetY = std::fmod(m_OffsetY, static_cast<float>(m_ImgH));
		if (m_OffsetY < 0.0f) m_OffsetY += m_ImgH;
	}
}

void ScrollManager::Draw() const
{
	if (m_BackgroundHandle < 0) return;
	if (m_ImgW <= 0 || m_ImgH <= 0)
	{
		// フォールバック: 背景を単色で塗る
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		return;
	}

	// オフセットを整数に変換
	int ox = static_cast<int>(m_OffsetX) % m_ImgW;
	int oy = static_cast<int>(m_OffsetY) % m_ImgH;
	if (ox < 0) ox += m_ImgW;
	if (oy < 0) oy += m_ImgH;

	// タイル描画：画面全体をカバーするために複数描画
	int startX = -ox;
	int startY = -oy;

	for (int y = startY; y < SCREEN_HEIGHT; y += m_ImgH)
	{
		for (int x = startX; x < SCREEN_WIDTH; x += m_ImgW)
		{
			// 必要に応じて軸ごとの非タイル動作に対応（ここでは単純に1枚描画で止める）
			if (!m_TileX && x != startX) continue;
			if (!m_TileY && y != startY) continue;
			DrawGraph(x, y, m_BackgroundHandle, TRUE);
		}
	}
}

