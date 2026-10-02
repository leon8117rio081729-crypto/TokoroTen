#include "PlayerShotManager.h"
#include "PlayerNormalShot.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyManager.h"

PlayerShotManager* PlayerShotManager::m_Instance = nullptr;

PlayerShotManager::PlayerShotManager()
	:m_OriginalShot{}
{
}

PlayerShotManager::~PlayerShotManager()
{
	Fin();
}

void PlayerShotManager::Init()
{
	// クローン元のプレイヤーショットを生成する
	m_OriginalShot[PLAYER_NORMAL] = new PlayerNormalShot;
}

void PlayerShotManager::Load()
{
	// クローン元のプレイヤーショットをロードする
	for (int i = 0; i < PLAYER_SHOT_TYPE_MAX; i++)
	{
		m_OriginalShot[i]->Load();
	}
}

void PlayerShotManager::Start()
{
	for (int i = 0; i < PLAYER_SHOT_TYPE_MAX; i++)
	{
		m_OriginalShot[i]->Start();
	}
}

void PlayerShotManager::Step()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Step();
	}
}

void PlayerShotManager::Update()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Update();
	}
}

void PlayerShotManager::Draw()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Draw();
	}
}

void PlayerShotManager::Fin()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		delete Shot;
	}

	// リストをクリア
	m_ShotList.clear();

	// クローン元も削除する
	for (auto Shot : m_OriginalShot)
	{
		delete Shot;
	}
}

void PlayerShotManager::FireShot(VECTOR pos, VECTOR dir, PlayerShotType type, float speed, int shotlife)
{
	EnemyBase* target = nullptr;
	float minDistSq = FLT_MAX;

	// 一番近い敵を探す
	for (auto enemy : EnemyManager::GetInstance()->GetEnemyList())
	{
		if (!enemy->GetActive()) continue;

		VECTOR diff = VSub(enemy->GetPos(), pos);

		float distSq =
			diff.x * diff.x +
			diff.y * diff.y +
			diff.z * diff.z;

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			target = enemy;
		}
	}

	// ターゲットがいたら方向を変更
	if (target)
	{
		dir = VSub(target->GetPos(), pos);

		// 真上・真下を狙わないなら
		dir.y = 0.0f;

		dir = VNorm(dir);
	}
	for (auto shot : m_ShotList)
	{
		if (!shot->IsActive())
		{
			shot->Respawn(shotlife);
			shot->SetPos(pos);
			shot->SetMove(VScale(dir, speed)); // 方向 × 速度
			shot->SetActive(true);
			return;
		}
	}

	PlayerShotBase* shot = CreateShot(type);
	shot->SetPos(pos);
	shot->SetMove(VScale(dir, speed));
	shot->SetActive(true);
}

PlayerShotBase* PlayerShotManager::CreateShot(PlayerShotType type)
{
	// タイプに合わせたショットをクローンで生成
	PlayerShotBase* Shot = m_OriginalShot[type]->Clone();

	// 生成したショットを管理用リストに追加
	m_ShotList.push_back(Shot);

	// 返却すれば生成した後にいろいろいじれる
	return Shot;
}
