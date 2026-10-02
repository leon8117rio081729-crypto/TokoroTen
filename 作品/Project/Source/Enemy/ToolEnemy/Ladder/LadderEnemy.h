#pragma once
#include "../../../Enemy/EnemyBase.h"
#include "../../../Player/Player.h"
class CollisionSphere;

class LadderEnemy : public EnemyBase
{
public:
	LadderEnemy();
	~LadderEnemy();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override; // プレイヤーとの距離を測り、状態を更新
	void Attack(Player* player); // 攻撃処理
	// Update, Draw, FinはEnemyBaseの処理で十分なので不要
	// 複製、量産するためのクローン関数
	EnemyBase* Clone() override;
};

