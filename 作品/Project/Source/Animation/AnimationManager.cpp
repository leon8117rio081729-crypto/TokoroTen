#include "AnimationManager.h"
#include "DxLib.h"
constexpr float ANIMATION_SPEED = 1.0f;

// 静的変数の初期化
AnimationManager* AnimationManager::m_Instance = nullptr;

AnimationManager::AnimationManager()
	: m_IsAnimationFinished(false)
{
}

AnimationManager::~AnimationManager()
{
	// 削除時の後始末忘れを防止する
	Fin();
}

void AnimationManager::Load()
{
}

void AnimationManager::Fin()
{
	m_AnimStates.clear();
	m_AnimNameToIndex.clear();
}

void AnimationManager::PlayAnimation(int handle , const std::string& animName, bool isLoop)
{
	// アニメーションの状態を取得
	auto animTableIt = m_AnimNameToIndex.find(handle);
	if (animTableIt == m_AnimNameToIndex.end()) return;

	// 以前のアニメーションをデタッチ
	auto& animTable = animTableIt->second;
	auto it = animTable.find(animName);
	if (it == animTable.end()) return;

	int animIndex = it->second;
	
	AnimState& state = m_AnimStates[handle];
	if (state.nowAnim == animName) return; // 同じアニメ再生中なら何もしない

	// 以前のアニメをデタッチ
	if (state.attachIndex != -1)
	{
		MV1DetachAnim(handle, state.attachIndex);
	}

	// 新しいアニメをアタッチ
	state.attachIndex = MV1AttachAnim(handle, animIndex, -1, FALSE);
	// アニメーションのトータル時間を取得
	state.totalTime = MV1GetAttachAnimTotalTime(handle, state.attachIndex);
	// 現在の再生時間を0にする
	state.nowTime = 0.0f;
	// ループ設定
	state.isLoop = isLoop;
	// 再生中のアニメーション設定
	state.nowAnim = animName;
}

void AnimationManager::UpdateAnimation(int handle)
{
	m_IsAnimationFinished = false;

	// アニメーションの状態を取得
	auto it = m_AnimStates.find(handle);
	// ハンドルが見つからない場合は何もしない
	if (it == m_AnimStates.end())
	{
		return;
	}
	AnimState& state = it->second;

	// 再生時間を進める
	state.nowTime += ANIMATION_SPEED;

	// 末尾まで再生したか
	if (state.nowTime > state.totalTime)
	{
		// ループ設定なら冒頭に戻す
		if (state.isLoop)
		{
			state.nowTime = 0.0f;
		}
		// ループでなければ末尾で止める
		else
		{
			state.nowTime = state.totalTime;
			m_IsAnimationFinished = true;
		}
	}
	// アニメーションの再生時間を設定
	MV1SetAttachAnimTime(handle, state.attachIndex, state.nowTime);
}

float AnimationManager::GetCurrentFrame(int handle)
{
	auto it = m_AnimStates.find(handle);
    if (it == m_AnimStates.end())
    {
        return 0.0f; // 見つからなければ0を返す
    }
    const AnimState& state = it->second;

    // 現在フレームを直接取得
    return MV1GetAttachAnimTime(handle, it->second.attachIndex);
}

bool AnimationManager::CheckAnimationFinish(int handle)
{
	auto it = m_AnimStates.find(handle);
	if (it == m_AnimStates.end()) return true; // 未登録なら終わった扱い

	const AnimState& state = it->second;
	// ループ再生の場合は「終わらない」ので常に false
	if (state.isLoop) return false;

	// 再生時間が総時間以上なら終了
	return state.nowTime >= state.totalTime;
}

void AnimationManager::RegisterAnimations(int handle, const std::unordered_map<std::string, int>& animMap)
{
	m_AnimNameToIndex[handle] = animMap;
}

bool AnimationManager::IsPlaying(int handle, const std::string& animName)
{
	auto it = m_AnimStates.find(handle);
	if (it == m_AnimStates.end()) return false;
	return it->second.nowAnim == animName && !CheckAnimationFinish(handle);
}

bool AnimationManager::IsFinished(int handle, const std::string& animName)
{
	auto it = m_AnimStates.find(handle);
	if (it == m_AnimStates.end()) return true;
	return it->second.nowAnim == animName && CheckAnimationFinish(handle);
}
