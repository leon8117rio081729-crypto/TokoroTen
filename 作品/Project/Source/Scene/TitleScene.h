#pragma once
#include "SceneBase.h"
#include "../Button/IconButton.h"

class IconButton;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

private:
	int m_TitleHandle;
	IconButton* startBtm;// ゲーム開始ボタン
	IconButton* exitBtm; // ゲーム終了ボタン
	IconButton::ButtonImg img;// ボタン画像構造体
};
