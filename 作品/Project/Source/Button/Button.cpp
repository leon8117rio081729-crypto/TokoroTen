#include "Button.h"
#include "../Color/Color.h"

Button::Button(int x, int y, int w, int h, const char* label)
    : x(x), y(y), w(w), h(h), label(label), clicked(false), prevMouseInput(0),isHovered(false), isSelected(false)
{
}

void Button::Update()
{
    int nowMouseInput = GetMouseInput();

    // マウス位置
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    // マウスが範囲内か判定（ホバー判定）
    isHovered = (mouseX >= x && mouseX <= x + w &&
        mouseY >= y && mouseY <= y + h);

    // 左クリックの瞬間で、かつ範囲内
    clicked = ((nowMouseInput & MOUSE_INPUT_LEFT) &&
        !(prevMouseInput & MOUSE_INPUT_LEFT) &&
        isHovered);

    prevMouseInput = nowMouseInput;
}

void Button::Draw(bool isSelected)
{
    unsigned int fillColor;

    if (isSelected) {
        fillColor = SKYBLUE;  // 選択中なら空色
    }
    else if (isHovered) {
        fillColor = YELLOW;  // ホバー中（黄色）
    }
    else {
        fillColor = BLUE;  // 通常（青）
    }

    DrawBox(x, y, x + w, y + h, fillColor, TRUE);     // 背景
    DrawBox(x, y, x + w, y + h, WHITE, FALSE);        // 枠線
    DrawString(x + 10, y + (h / 2 - 8), label, WHITE);

}
