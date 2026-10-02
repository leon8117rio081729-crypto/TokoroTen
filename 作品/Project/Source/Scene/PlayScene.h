#pragma once
#include "SceneBase.h"

class Skybox;
class Player;
class PlayScene : public SceneBase
{
public:
	PlayScene();
	virtual ~PlayScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

private:	
	void EndPause(); // ポーズ終了処理
	void DebugDrawJoypadButtons();

private:
	int m_IgnoreInputTimer; // 入力無視タイマー（ポーズ中の入力を無視するため）
	int m_StatsFont;
};
