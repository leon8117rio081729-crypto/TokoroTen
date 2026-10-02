#include "DxLib.h"
#include "Input.h"
#include <math.h>

int Input::m_InputState = 0;
int Input::m_PrevInputState = 0;
int Input::m_PrevPadState = 0;
int Input::m_NowPadState = 0;
float Input::m_LeftStickX = 0.0f;
float Input::m_LeftStickY = 0.0f;
float Input::m_RightStickX = 0.0f;
float Input::m_RightStickY = 0.0f;
unsigned char Input::m_RightTrigger = 0;
unsigned char Input::m_PrevRightTrigger = 0;

void Input::Init()
{
	m_InputState = 0;
	m_PrevInputState = 0;
}

void Input::Update()
{
    // 前回の入力を保存
	m_PrevPadState = m_NowPadState;
	m_NowPadState = GetJoypadInputState(DX_INPUT_PAD1);
	// コントローラー入力
	XINPUT_STATE state;
	GetJoypadXInputState(DX_INPUT_PAD1, &state);

	// スティック値を -1.0f ～ 1.0f に正規化（int → float）
	m_LeftStickX = state.ThumbLX / STICK_NORMALIZATION;
	m_LeftStickY = state.ThumbLY / STICK_NORMALIZATION;
	m_RightStickX = state.ThumbRX / STICK_NORMALIZATION;
	m_RightStickY = state.ThumbRY / STICK_NORMALIZATION;

	// スティックのデッドゾーン処理（誤動作防止）
	if (fabs(m_LeftStickX) < SRTICK_DEADZONE) m_LeftStickX = 0.0f;
	if (fabs(m_LeftStickY) < SRTICK_DEADZONE) m_LeftStickY = 0.0f;
	if (fabs(m_RightStickX) < SRTICK_DEADZONE) m_RightStickX = 0.0f;
	if (fabs(m_RightStickY) < SRTICK_DEADZONE) m_RightStickY = 0.0f;

	// 前回のRT
	m_PrevRightTrigger = m_RightTrigger;
	// 現在のRT
	m_RightTrigger = state.RightTrigger;
}

void Input::Draw()
{
}

void Input::Fin()
{
}

bool Input::IsTriggerPadButton(int button)
{
	return (m_NowPadState & button) && !(m_PrevPadState & button);
}

bool Input::IsPressPadButton(int button)
{
	return (m_NowPadState & button);
}

bool Input::IsPressRT()
{
	return m_RightTrigger > 30;
}

bool Input::IsTriggerRT()
{
	return (m_RightTrigger > 30) &&
		(m_PrevRightTrigger <= 30);
}

VECTOR Input::GetLeftStick()
{
	return VGet(m_LeftStickX, m_LeftStickY, 0);
}

VECTOR Input::GetRightStick()
{
	return VGet(m_RightStickX, m_RightStickY, 0);
}
