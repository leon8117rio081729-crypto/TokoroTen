#pragma once
#include "DxLib.h"

static constexpr float STICK_NORMALIZATION = 32768.0f; // スティックの値を正規化するための定数
static constexpr float SRTICK_DEADZONE = 0.2f; // スティックのデッドゾーン（0.0f ～ 1.0f）

class Input
{
public:
	// 関数のプロトタイプ宣言 
	static void Init();
	static void Update();
	static void Draw();
	static void Fin();

	static bool IsTriggerPadButton(int button);    // 一度押したときだけtrue
	static bool IsPressPadButton(int button);      // 押し続けているときtrue
	static bool IsPressRT();      // RTを押している
	static bool IsTriggerRT();    // RTを押した瞬間
	static VECTOR GetLeftStick();                  // 左スティックのXY方向
	static VECTOR GetRightStick();                 // 右スティックのXY方向（視点操作用）
private:
	// 入力ビットフラグ
	static int m_InputState;
	// 前回の入力ビット
	static int m_PrevInputState;

	static int m_PrevPadState;
	static int m_NowPadState;

	static float m_LeftStickX;
	static float m_LeftStickY;

	static float m_RightStickX;
	static float m_RightStickY;

	static unsigned char m_RightTrigger;
	static unsigned char m_PrevRightTrigger;

};
