#pragma once
#include "SceneBase.h"
#include "../Button/IconButton.h"

class IconButton;

class ClearScene : public SceneBase
{
public:
	ClearScene();
	virtual ~ClearScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;
private:
	int m_ClearHandle;
	IconButton* titleBtm;
	IconButton* exitBtm;
	IconButton::ButtonImg img;// ボタン画像構造体
};
