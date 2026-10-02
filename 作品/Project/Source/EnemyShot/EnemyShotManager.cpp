#include "EnemyShotManager.h"
#include "EnemyNormalShot.h"

EnemyShotManager* EnemyShotManager::m_Instance = nullptr;

EnemyShotManager::EnemyShotManager()
	:m_OriginalShot{}
{
}

EnemyShotManager::~EnemyShotManager()
{
	Fin();
}

void EnemyShotManager::Init()
{
	// クローン元のエネミーを生成する
	m_OriginalShot[ENEMY_NORMAL] = new EnemyNormalShot;
}

void EnemyShotManager::Load()
{
	// クローン元のエネミーをロードする
	for (int i = 0; i < ENEMY_SHOT_TYPE_MAX; i++)
	{
		m_OriginalShot[i]->Load();
	}
}

void EnemyShotManager::Start()
{
	for (int i = 0; i < ENEMY_SHOT_TYPE_MAX; i++)
	{
		m_OriginalShot[i]->Start();
	}
}

void EnemyShotManager::Step()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Step();
	}
}

void EnemyShotManager::Update()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Update();
	}
}

void EnemyShotManager::Draw()
{
	// 範囲for文で安全にリストを回せる
	for (auto Shot : m_ShotList)
	{
		Shot->Draw();
	}
}

void EnemyShotManager::Fin()
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

void EnemyShotManager::FireShot(VECTOR pos, VECTOR dir, EnemyShotType type, float speed ,int shotlife)
{
	for (auto shot : m_ShotList)
	{
		if (!shot->IsActive() && shot->GetType() == type)
		{
			shot->Respawn(shotlife);
			shot->SetPos(pos);
			shot->SetMove(VScale(dir, speed)); // 方向 × 速度
			shot->SetActive(true);
			return;
		}
	}

	EnemyShotBase* shot = CreateShot(type);
	shot->SetType(type);
	shot->SetPos(pos);
	shot->SetMove(VScale(dir, speed));
	shot->SetActive(true);
}

EnemyShotBase* EnemyShotManager::CreateShot(EnemyShotType type)
{
	// タイプに合わせたショットをクローンで生成
	EnemyShotBase* Shot = m_OriginalShot[type]->Clone();

	// 生成したショットを管理用リストに追加
	m_ShotList.push_back(Shot);

	// 返却すれば生成した後にいろいろいじれる
	return Shot;
}
