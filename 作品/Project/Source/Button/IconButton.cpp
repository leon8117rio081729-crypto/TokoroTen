#include "../Button/IconButton.h"
#include <corecrt_math.h>

IconButton::IconButton(int x, int y, int width, int height,
	int imgNormal,
	int imgHover,
	int imgPress)
	: x(x), y(y), w(width), h(height),
	imgNormal(imgNormal),
	imgHover(imgHover),
	imgPress(imgPress),
	state(Normal),
	clicked(false),
	prevMouse(0),
	selected(false),
	m_Scale(1.0f)
{
}

void IconButton::Update()
{
	int mx, my;

	GetMousePoint(&mx, &my);

	int mouse = GetMouseInput();

	bool hover =
		mx >= x && mx <= x + w &&
		my >= y && my <= y + h;

	// マウスホバー または コントローラによる選択 のどちらかで Hover 表示にする
	bool effectiveHover = hover || selected;

	clicked = false;

	if (effectiveHover)
	{
		// マウスで押された場合のみ Press にする（コントローラの押下は外部で処理）
		if (mouse & MOUSE_INPUT_LEFT)
		{
			state = Press;
		}
		else
		{
			state = Hover;
		}
		// マウスクリックの判定
		if (!(mouse & MOUSE_INPUT_LEFT) &&
			(prevMouse & MOUSE_INPUT_LEFT))
		{
			// マウスでのクリックを検出
			if (hover) clicked = true; // マウスクリックは実際にボタン上で離したときのみ有効
		}
	}
	else
	{
		state = Normal;
	}

	prevMouse = mouse;
}

void IconButton::Draw()
{
	int handle = imgNormal;

	// 状態に応じた画像を選択
	if (state == Press)
	{
		handle = imgPress;
	}
	else if (state == Hover)
	{
		int time = GetNowCount();
		// 0 or 1 が交互に切り替わる
		if ((time / 300) % 2 == 0)
		{
			handle = imgHover;
		}
		else
		{
			handle = imgNormal;
		}
	}

	// 拡大アニメーション
	float targetScale = (state == Hover) ? 1.1f : 1.0f;
	m_Scale += (targetScale - m_Scale) * 0.2f;
	float scale = m_Scale;
	// 画像サイズを取得
	int w, h;
	GetGraphSize(handle, &w, &h);
	// 中心基準で拡大
	int drawX = x - (int)((w * scale - w) * 0.5f);
	int drawY = y - (int)((h * scale - h) * 0.5f);

	// 画像を描画
	DrawExtendGraph(drawX, drawY, drawX + (int)(w * scale), drawY + (int)(h * scale), handle, TRUE);
}

IconButton::ButtonImg IconButton::LoadImgButton()
{
	ButtonImg img;

	img.sNormal = LoadGraph("Data/Button/SButtonN1.png");
	img.sHover = LoadGraph("Data/Button/SButtonH1.png");
	img.sPush = LoadGraph("Data/Button/SButtonP1.png");

	img.sNormal2 = LoadGraph("Data/Button/SButtonN2.png");
	img.sHover2 = LoadGraph("Data/Button/SButtonH2.png");
	img.sPush2 = LoadGraph("Data/Button/SButtonP2.png");


	return img;
}
