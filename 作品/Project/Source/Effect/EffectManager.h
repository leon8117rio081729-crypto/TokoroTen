#pragma once
#include "DxLib.h"
#include <vector>

constexpr int EFFEKSEER_MAX_PARTICLE = 10000;
constexpr int EFFEKSEER_EFFECT_MAX = 32;

enum EffectType
{
	EFFECT_KILL,
	EFFECT_PLAYER_MOVE,
	EFFECT_PLAYER_UP_MOVE,
	EFFECT_PLAYER_DOWN_MOVE,
	EFFECT_PLAYER_FORWARD_MOVE,
	EFFECT_PLAYER_ATTACK,
	EFFECT_PLAYER_SHOT,
	EFFECT_ENEMY_TOOL_IDLE,
	EFFECT_HAMMER_USED,
	EFFECT_LADDER_USED,
	EFFEKSEER_EFFECT_TYPE_MAX
};

class Effect;

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

public:
	static void CreateInstence() { if (!m_Instance) m_Instance = new EffectManager; }
	static EffectManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	// Effekseerを使うのに必要なセットアップ処理
	// DxLibInitよりも前にやる処理をまとめたもの
	void Setup();

	// Effekseerを使う前に必要な初期化処理
	// DxLibInitよりも後にやる処理をまとめたもの
	bool Init();

	// Effekseerのエフェクトデータをロードする
	void Load();

	// Effekseer使用開始処理
	// Effekseerのエフェクトを実際に使うシーンの初期化処理などで呼ぶ
	void Start();

	// エフェクト再生
	Effect* PlayEffect(int type, VECTOR pos);
	// エフェクト停止
	void StopAllEffects();

	// ステップ処理
	void Step();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// Effekseer終了処理
	// Effekseerのエフェクトが不要になったら呼ぶ
	void Fin();

private:
	static EffectManager* m_Instance;	// シングルトン用インスタンス

	std::vector<int> m_EffectHandles;	// リソースハンドル配列
	std::vector<Effect*> m_Effects;   	// エフェクト配列

	bool m_IsActive;
};

