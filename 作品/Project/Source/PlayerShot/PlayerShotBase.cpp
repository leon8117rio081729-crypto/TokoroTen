#include "PlayerShotBase.h"
#include "../MyMath/MyMath.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyManager.h"
#include "../Effect/EffectManager.h"

constexpr int SHOT_LIFE = 40;

PlayerShotBase::PlayerShotBase()
	: m_Handle(0)
	, m_Effect(nullptr)
	, m_Pos(VGet(0.0f, 0.0f, 0.0f))
	, m_Rot(VGet(0.0f, 0.0f, 0.0f))
	, m_Move(VGet(0.0f, 0.0f, 0.0f))
	, m_SphereCollision(nullptr)
	, m_IsActive(false)
	, m_ShotLife(0)
{
}

PlayerShotBase::~PlayerShotBase()
{
	Fin();
}

void PlayerShotBase::Start()
{
	m_ShotLife = SHOT_LIFE;
}

void PlayerShotBase::Step()
{
	m_ShotLife--;

	if (m_ShotLife <= 0)
	{
		m_IsActive = false;

		if (m_Effect)
		{
			m_Effect->Stop();
			m_Effect = nullptr;
		}
	}
}

void PlayerShotBase::Update()
{
	if (!m_IsActive) return;

	EnemyBase* target = nullptr;
	float minDistSq = FLT_MAX;

	// 一番近い敵を探す
	for (auto enemy : EnemyManager::GetInstance()->GetEnemyList())
	{
		if (!enemy->GetActive()) continue;

		// プレイヤーから敵までの距離
		VECTOR toEnemy = VSub(enemy->GetPos(), m_Pos);
		float distSq = VDot(toEnemy, toEnemy);

		// 一定範囲外なら無視
		if (distSq > HOMING_RANGE * HOMING_RANGE)
			continue;

		// 範囲内で一番近い敵を選ぶ
		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			target = enemy;
		}
	}
	// ターゲットがいたら方向を変更
	if (target)
	{
		VECTOR dir = VNorm(VSub(target->GetPos(), m_Pos));

		// ホーミング性能
		const float HOMING_POWER = 0.2f;

		m_Move = VAdd(
			VScale(m_Move, 1.0f - HOMING_POWER),
			VScale(dir, HOMING_POWER));

		// 速度を一定に保つ
		float speed = VSize(m_Move);
		m_Move = VScale(VNorm(m_Move), speed);
	}
	// 移動量を加算
	m_Pos = VAdd(m_Pos, m_Move);
	if (m_Effect)
	{
		m_Effect->SetPos(m_Pos);// エフェクトの位置をショットの位置に追従させる
	}
	MV1SetPosition(m_Handle, m_Pos);
}

void PlayerShotBase::Draw()
{
	if (m_IsActive)
	{
		// 何もしない
	}
}

void PlayerShotBase::Fin()
{
	MV1DeleteModel(m_Handle);
}

void PlayerShotBase::Respawn(int shotlife)
{
	m_ShotLife = shotlife;
	m_IsActive = true;
	m_Effect = EffectManager::GetInstance()->PlayEffect(EFFECT_PLAYER_SHOT, m_Pos);
}