#pragma once
#include "EnemyBase.h"
#include "../Player/Player.h"
class CollisionSphere;

class NormalEnemy : public EnemyBase
{
public:
	NormalEnemy();
	~NormalEnemy();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() ; // プレイヤーとの距離を測り、状態を更新
	void Attack(Player* player);
	// Update, Draw, FinはEnemyBaseの処理で十分なので不要
	// 複製、量産するためのクローン関数
	EnemyBase* Clone() override;

private:
	int m_ShotInterval; 
};

