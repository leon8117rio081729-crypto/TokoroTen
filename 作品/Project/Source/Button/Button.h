#pragma once
#include "DxLib.h"

class Button 
{
public:
    Button(int x, int y, int w, int h, const char* label);

    void Update();           // 状態更新（毎フレーム呼ぶ）
    void Draw(bool isSelected);             // 描画
    bool IsClicked() { return clicked; }        // 今回クリックされた瞬間か？
	
private:
    int x, y, w, h;
    const char* label;

    bool clicked;
    int prevMouseInput;
    bool isHovered;  // マウスが上に乗っているかどうか
	bool isSelected; // 選択中かどうか（外部から設定される）
};
