#include "EffekseerForDXLib.h"
#include "Effect.h"

Effect::Effect()
	: m_Active (false) 
	, m_Handle (0) 
	, m_Pos (VGet(0.0f, 0.0f, 0.0f)) 
{
}

Effect::~Effect()
{
	Fin();
}

void Effect::Step()
{
	// 再生中かどうか
	if (IsEffekseer3DEffectPlaying(m_Handle) != 0)
	{
		m_Active = false;
	}
}

void Effect::Update()
{
	if (!m_Active) return;

	// 位置設定
	SetPosPlayingEffekseer3DEffect(m_Handle, m_Pos.x, m_Pos.y, m_Pos.z);
}

void Effect::Fin()
{
}

void Effect::Play(int handle)
{
	m_Handle = PlayEffekseer3DEffect(handle);
}

void Effect::Stop()
{
	StopEffekseer3DEffect(m_Handle);
	m_Active = false;
}
