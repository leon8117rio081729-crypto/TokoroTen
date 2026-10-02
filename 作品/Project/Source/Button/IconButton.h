#pragma once
#include "DxLib.h"

class IconButton
{
public:
	enum  State
	{
		Normal,
		Hover,
		Press,
	};

	struct  ButtonImg
	{
		int sNormal = -1;
		int sHover = -1;
		int sPush = -1;

		int sNormal2 = -1;
		int sHover2 = -1;
		int sPush2 = -1;
	};

	IconButton(int x, int y, int width, int height, int imgNormal, int imgHover, int imgPress);

	void Update();
	void Draw();

	bool IsClicked() const { return clicked; }
	// コントローラ／キーボードで選択されたことを通知する
	void SetSelected(bool sel) { selected = sel; }
	bool IsSelected() const { return selected; }

	static ButtonImg LoadImgButton();

private:
	int x, y, w, h;
	int imgNormal;
	int imgHover;
	int imgPress;

	float m_Scale;

	State state;

	bool clicked;
	int prevMouse;

	// コントローラやキーボードによる選択状態を保持
	bool selected;
};