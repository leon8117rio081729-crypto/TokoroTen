#pragma once
#include <unordered_map>
#include <string>
#include <DxLib.h>
//アニメーションタイプ
enum AnimationType
{
	PLAYER_ANIMATION_ATTACK,
	PLAYER_ANIMATION_IDLE, 
	PLAYER_ANIMATION_JUMP,
	PLAYER_ANIMATION_WALK,
	PLAYER_ANIMATION_NONE,
};

// アニメーション管理クラス
class AnimationManager
{
public:
	AnimationManager();	// コンストラクタ
	~AnimationManager();	// デストラクタ

public:
	// アニメーションマネージャーを生成する
	static void CreateInstance() { if (!m_Instance) m_Instance = new AnimationManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static AnimationManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Load();	// ロード
	void Fin();		// 終了

public:
	void PlayAnimation(int handle, const std::string& animName, bool isLoop);
	void UpdateAnimation(int handle);
	float GetCurrentFrame(int handle);// ★ 現在のフレーム番号を取得する
	bool CheckAnimationFinish(int handle);
	void RegisterAnimations(int handle, const std::unordered_map<std::string, int>& animMap);
	bool IsPlaying(int handle, const std::string& animName);
	bool IsFinished(int handle, const std::string& animName);

	bool m_IsAnimationFinished = false;
private:
	// 生成されたAnimationManager自身を格納する変数
	// AnimationManagerはゲーム上に１つのみなのでstaticにしている
	static AnimationManager* m_Instance;
	
	struct AnimState
	{
		int attachIndex = -1;
		float totalTime = 0.0f;
		float nowTime = 0.0f;
		bool isLoop = false;
		std::string nowAnim;
	};
	std::unordered_map<int, AnimState> m_AnimStates; // モデルハンドルごとのアニメーション状態を管理
	// ★モデルハンドルとアニメーション名の対応表を管理
	std::unordered_map<int, std::unordered_map<std::string, int>> m_AnimNameToIndex;
};
