#pragma once
#include "EnemyParams.h"
#include "EnemyType.h"
#include <vector> 
#include "DxLib.h"
class CollisionAABB;
class CollsionOBB;
class CollisionSphere;
class Player;
class StageObject;
class GimmickBase;
class EnemyManager;

enum class EnemyState 
{
	Wander,//プレイヤーを探す徘徊状態
	Chase, //プレイヤーを追いかける状態
};

// ★ 攻撃用スフィアデータ構造体追加
struct AttackSphereData
{
	CollisionSphere* sphere;
	VECTOR localOffset; //根元→先端方向のローカル位置
};

class EnemyBase
{
public:
	// 定数
	static constexpr float GRAVITY = 0.01f; // 重力

	// 壁や範囲にぶつかったら方向反転
	static constexpr float AREA_MIN_X = -300.0f;
	static constexpr float AREA_MAX_X = 300.0f;
	static constexpr float AREA_MIN_Z = -300.0f;
	static constexpr float AREA_MAX_Z = 300.0f;

	EnemyBase();
	virtual ~EnemyBase();

public:
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Start() = 0;

	// 各エネミー専用で処理を作る必要がない場合は基底クラスで共通処理にする
	virtual void Step();
	virtual void Update();
	virtual void Draw();	
	virtual void Fin();

	// 複製、量産するためのクローン関数
	virtual EnemyBase* Clone() = 0;

	virtual void HitShot();
	virtual void Damage();      // 踏まれたときの処理
	virtual void HitAttack() {} // プレイヤーの攻撃に当たったときの処理
	virtual void DebugDraw();   // デバッグ描画

public:
	void SetPos(const VECTOR& pos) { m_Pos = pos; }
	VECTOR GetPos() { return m_Pos; }
	void SetActive(bool active) { m_Active = active; }
	bool GetActive() { return m_Active; }
	bool GetIsDead() { return m_IsDead; }
	void SetRespawnFlag(bool flag) { m_RespawnRegistered = flag; }
	void SetKilledFlag(bool flag) { m_FirstKilled = flag; }
	CollisionSphere* GetSphereCollision() { return m_SphereCollision; }
	void SetType(EnemyType type) { m_EnemyType = type; }
	EnemyType GetType() const { return m_EnemyType; }
	void SetParams(const EnemyParams& params) { m_Params = params; }
	const EnemyParams& GetParams() const { return m_Params; }

	void ResetStatus();

	void CheckHitStageObjects(const std::vector<StageObject*>& objects);
	void CheckHitGimmickObjects(const std::vector<GimmickBase*>& gimmicks);

protected:

	void Chase(Player* player);// プレイヤーを追いかける
	void Wander();// ランダムにうろうろする
	void Dead(); // 死亡時の処理
    void Attack(Player* player); // 攻撃処理
	void SetupAttackSpheres();
	bool IntersectSegmentAABB(VECTOR start, VECTOR end, CollisionAABB* box);
	bool CanSeePlayer(Player* player);
	int m_Handle;
	int m_Hp;
	int m_AttackInterval;
	bool m_AttackFlag;
	float m_Speed;
	float m_DetectionRange;
	float m_AttackRotY; // 攻撃開始時の向き
	int m_EffectTimer;
	bool m_Active;
	bool m_Chase;       // Chase状態かどうか
	bool m_HitThisAnim; // 現在の攻撃アニメーションでヒット済みか
	bool m_AttackSEPlayed; // 攻撃SEを再生済みか
	bool m_IsAttacking; // 攻撃モーション中
	int m_WanderTimer;   // 徘徊用タイマー
	bool m_IsPausing;    // 停止中フラグ
	int m_PauseTimer;    // 停止時間
	bool m_IsDead;      // 死亡フラグ
	bool m_FirstKilled;  // 初回撃破フラグ
	bool m_RespawnRegistered;// 再出現登録済みフラグ
	VECTOR m_Pos;
	VECTOR m_Rot;
	VECTOR m_Move;
	CollisionSphere* m_SphereCollision;	// 球の当たり判定
	EnemyParams m_Params; // JSONから読み込んだパラメータを保持
	EnemyState m_State;
	EnemyType m_EnemyType = ENEMY_TYPE_NONE;
	std::vector<AttackSphereData> m_AttackSpheres; // 攻撃スフィア群
};
