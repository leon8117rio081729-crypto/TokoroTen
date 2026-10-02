#pragma once
#include <DxLib.h>
#include "../../Scene/ScreenSize.h"

class ScrollManager
{
public:
	// シングルトン操作
	static void CreateInstance();
	static ScrollManager* GetInstance();
	static void DeleteInstance();

	ScrollManager();
	~ScrollManager();

	// ライフサイクル
	void Init();
	void Fin();

	// 背景設定
	bool LoadBackground(const char* path); // LoadGraph して設定
	void SetBackgroundHandle(int handle);  // 既に読み込み済みハンドルを設定
	int  GetBackgroundHandle() const { return m_BackgroundHandle; }

	// スクロール制御
	void SetSpeed(float vx, float vy); // px/frame
	void SetActive(bool active);
	bool IsActive() const { return m_Active; }

	// 毎フレーム呼ぶ
	void Update();
	void Draw() const;

	// オプション
	void SetTile(bool tileX, bool tileY); // タイル描画する軸

private:
	static ScrollManager* m_Instance;

	int  m_BackgroundHandle;
	int  m_ImgW;
	int  m_ImgH;
	float m_OffsetX;
	float m_OffsetY;
	float m_SpeedX;
	float m_SpeedY;
	bool m_Active;
	bool m_TileX;
	bool m_TileY;
};
