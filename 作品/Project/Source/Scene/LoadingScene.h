#pragma once
#include "SceneBase.h"

class LoadingScene : public SceneBase
{
public:
	LoadingScene();
	virtual ~LoadingScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

private:
	// ロードバーの位置とサイズ
	const int BAR_X = 290;
	const int BAR_Y = 340;
	const int BAR_W = 1000;
	const int BAR_H = 40;

	float m_BarAnim;		// ロードバーアニメーション用変数
	float m_PressAnim;		// ボタン押下アニメーション用変数
	int m_CurrentTip;		// 現在のチップ番号
	int m_LoadProgress;     // 読み込み進捗（0〜100）
	bool m_IsLoaded;		// 読み込み完了フラグ
	int m_FrameCounter;     // 疑似的な読み込み時間を作るためのカウンタ
	const char* m_CurrentLoadingText;// 現在の読み込みテキスト
};
