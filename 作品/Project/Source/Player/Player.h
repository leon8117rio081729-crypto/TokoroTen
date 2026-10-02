#pragma once
#include "DxLib.h"
#include "../Inventory/Inventory.h"

class CollisionAABB;
class CollisionSphere;
class StageObject;
class GimmickBase; 
class DeliveryBox;

enum class PlayerState
{
	START,  //スタート
	NORMAL, //通常
	DAMAGE, //ダメージ
};

// プレイヤークラス
class Player 
{
public:

	// 定数
	static constexpr float ROTATION_SPEED = 0.1f;     // 回転速度
	static constexpr float MOVE_SPEED = 0.25f;        // 移動速度
	static constexpr float ANIMATION_SPEED = 1.0f;    // アニメーション速度
	static constexpr float ANIM_TIME = 10;            // アニメーションの時間
	static constexpr int DAMAGE_TIME = 25;            // ダメージを受ける間隔
	static constexpr int SHOT_DAMAGE_TIME = 1;        // 弾からダメージを受ける間隔
	static constexpr float PLAYER_POS_Y_MAX = 18.0f;  // Y軸の移動制限
	static constexpr float PLAYER_POS_Y_MIN = -10.0f; // Y軸の移動制限
	static constexpr float PLAYER_POS_Z_MAX = 450.0f; // Z軸の移動制限
	static constexpr float PLAYER_POS_Z_MIN = -100.0f;// Z軸の移動制限
	static constexpr float GROUND_Y = 0.5f;           // 地面のY座標
	static constexpr float GRAVITY = 0.04f;           // 重力加速度
	static constexpr int HP = 30;                     // プレイヤーのHP初期値
	static constexpr int HP_MAX = 50;                // プレイヤーのHP最大値
	static constexpr int STAMINA_MAX = 100;           // スタミナ初期値
	static constexpr float JUMP_POWER = 1.25f;        // ジャンプの初速
	static constexpr int QUOTA = 150;                 // ノルマ

	Player();	// コンストラクタ
	~Player();	// デストラクタ

public:
	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Step();	// ステップ
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

public:
	// シングルトンインスタンス取得用メソッドを追加
	static Player* GetInstance() { return m_Instance; }
	static void CreateInstance() { if (!m_Instance) m_Instance = new Player; }
	static void DeleteInstance() { if (m_Instance) { delete m_Instance; m_Instance = nullptr; } }

	VECTOR SetPos(const VECTOR& pos) { m_Pos = pos; return m_Pos; }
	VECTOR SetVelocity(const VECTOR& vel) { m_Move = vel; return m_Move; }
	VECTOR GetPos() { return m_Pos; }
	VECTOR GetRot() const { return m_Rot; }
	VECTOR GetMove() const { return m_Move; }
	CollisionAABB* GetAABB() { return m_AABB; }
	CollisionAABB* GetAttackAABB() { return m_AttackAABB; }
	CollisionSphere* GetSphereCollision() { return m_SphereCollision; }
	Inventory* GetInventory() { return Inventory::GetInstance(); }
public:
	void HitEnemy(int value);
	void HitEnemyShot();
	void HitGoal();
	void OnStomp(); // 敵を踏んだ時の反動処理
	bool IsInteractPressed() const;
	void PlayerDead();
	void KilledCount() { m_Enemy++; }
	int GetEnemyCount() { return m_Enemy; }

	void SetRotationY(float rotY) { m_Rot.y = rotY; }
	void SetDashFlag(bool flag) { m_DashFlag = flag; }

	int GetHp() const { return m_Hp; }
	int GetHpMax() const { return m_Hp_MAX; }
	void SetHp(int hp) { m_Hp = hp; }
	void SetHpMax(int value) { m_Hp_MAX = value; }
	
	int GetStamina() const { return m_Stamina; }
	int GetStaminaMax() const { return m_Stamina_MAX; }
	void SetStaminaMax(int value) { m_Stamina_MAX = value; }

	int GetScore() const { return m_Score; }
	void SetScore(int score) { m_Score = score; }

	void CheckHitStageObjects(const std::vector<StageObject*>& objects);
	void CheckHitGimmickObjects(const std::vector<GimmickBase*>& gimmicks);
	void CheckHitDeliveryBox(const std::vector<DeliveryBox*>& boxes);
	void StopMove();

	void SetOnLadder(bool onLadder) { m_OnLadder = onLadder; }
	bool IsOnLadder() const { return m_OnLadder; }
	void LadderMove();
	bool GetGoalActive() { return m_GoalActive; }
	void SetGoalLevel(int level) { m_StageLevel = level; }
	int  GetStageLevel() const { return m_StageLevel; }
	bool IsJump() { return m_JumpingFlag; }

	void DrawUI(); // UI描画

private:
	int m_StageLevel; // ステージレベル
	int m_Handle;	    // 画像ハンドル
	int m_LifeHandle;	// プレイヤーのライフ画像ハンドル
	int m_FlashingCount;// 点滅カウント
	bool m_WasVisible;// 前フレームの表示状態
	int m_StatsFont; // ステータス用フォントハンドル
	
	int m_Enemy;// 倒した敵の数
	int m_Hp;                   //プレイヤーのHP
	int m_Hp_MAX; // プレイヤーのHP最大値
	int m_Stamina;              // スタミナ
	int m_Stamina_MAX; // スタミナ最大値
	int m_Speed; // プレイヤーの移動速度
	int m_Speed_MAX; // プレイヤーの移動速度最大値
	int m_Score; // スコア
	int m_Quota; // ノルマ
	int m_DamageInterval;// ダメージを受ける間隔
	int m_ShotDamageInterval;// ショットダメージを受ける間隔
	float m_AnimTime;// アニメーションの時間
	int m_StaminaTimer;// スタミナのタイマー
	bool m_MoveFlag; // 移動フラグ
	bool m_JumpingFlag;// ジャンプフラグ
	bool m_DashFlag; // ダッシュフラグ
	bool m_AttackFlag; // 攻撃フラグ
	bool m_AttackEfectFlag; // 攻撃エフェクトフラグ
	bool m_ShotFlag; // ショットフラグ
	bool m_ShotEfectFlag; // ショットエフェクトフラグ
	bool m_HitThisAnim; // 今回の攻撃アニメでヒット済みか

	bool m_OnLadder; // はしごに触れているかどうか
	bool m_GoalActive;// ゴール条件を満たしているかどうか
	bool m_CanGoalSEPlayed; // ゴール可能SEが再生されたかどうか

	VECTOR m_Pos;	// 座標
	VECTOR m_Rot;	// 回転
	VECTOR m_Scale;	// スケール
	VECTOR m_Move;	// 移動量
	VECTOR m_PrevPos; // 前回の座標
	VECTOR m_InputMove; // 入力による移動量
	CollisionAABB* m_AABB;	// AABBの当たり判定
	CollisionAABB* m_AttackAABB; // 攻撃判定用のAABB
	CollisionSphere* m_SphereCollision;
	Inventory* m_Inventory;
	PlayerState m_State;
	static Player* m_Instance;
};

