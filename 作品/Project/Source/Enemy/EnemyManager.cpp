#include "EnemyManager.h"
#include "EnemyParamsLoader.h"
#include "NormalEnemy.h"
#include "../Enemy/ToolEnemy/Ladder/LadderEnemy.h"
#include "../Enemy/ToolEnemy/Hammer/HammerEnemy.h"
#include "../Enemy/ToolEnemy/Bar/BarEnemy.h"
#include "../Enemy/ToolEnemy/Saw/SawEnemy.h"
#include "../Enemy/ToolEnemy/Screwdriver/ScrewdriverEnemy.h"
#include "../Player/PlayerManager.h"
#include <vector>
#include <DxLib.h>

// リスポーン用
struct RespawnEntry
{
	EnemyType type;
	VECTOR pos;
	unsigned int spawnTimeMs; // 復活予定時刻（GetNowCount 単位：ms）
};
// リスポーンリストの定義
static std::vector<RespawnEntry> g_RespawnList;
// 削除予定エネミーリストの定義
static std::vector<EnemyBase*> g_PendingDelete;

EnemyManager* EnemyManager::m_Instance = nullptr;
std::list<EnemyBase*> m_EnemyList; // エネミーリストの初期化

EnemyManager::EnemyManager()
	: m_OriginalEnemy{}
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
	// クローン元のエネミーを生成する
	m_OriginalEnemy[NORMAL_ENEMY] = new NormalEnemy;
	m_OriginalEnemy[LADDER_ENEMY] = new LadderEnemy;
	m_OriginalEnemy[HAMMER_ENEMY] = new HammerEnemy;
	m_OriginalEnemy[BAR_ENEMY] = new BarEnemy;
	m_OriginalEnemy[SAW_ENEMY] = new SawEnemy;
	m_OriginalEnemy[DRIVER_ENEMY] = new ScrewdriverEnemy;

	// エネミータイプを設定する
	m_OriginalEnemy[NORMAL_ENEMY]->SetType(NORMAL_ENEMY);
	m_OriginalEnemy[LADDER_ENEMY]->SetType(LADDER_ENEMY);
	m_OriginalEnemy[HAMMER_ENEMY]->SetType(HAMMER_ENEMY);
	m_OriginalEnemy[BAR_ENEMY]->SetType(BAR_ENEMY);
	m_OriginalEnemy[SAW_ENEMY]->SetType(SAW_ENEMY);
	m_OriginalEnemy[DRIVER_ENEMY]->SetType(DRIVER_ENEMY);

	// JSONからパラメータを読み込んで設定する
	// ノーマルエネミー 
	EnemyParams NormalParams = EnemyParamsLoader::Get("NormalEnemy");
	m_OriginalEnemy[NORMAL_ENEMY]->SetParams(NormalParams);// パラメータを設定
	// はしごエネミー
	EnemyParams LadderParams = EnemyParamsLoader::Get("LadderEnemy");
	m_OriginalEnemy[LADDER_ENEMY]->SetParams(LadderParams);// パラメータを設定
	// ハンマーエネミー
	EnemyParams HammerParams = EnemyParamsLoader::Get("HammerEnemy");
	m_OriginalEnemy[HAMMER_ENEMY]->SetParams(HammerParams);// パラメータを設定
	// バールエネミー
	EnemyParams BarParams = EnemyParamsLoader::Get("BarEnemy");
	m_OriginalEnemy[BAR_ENEMY]->SetParams(BarParams);// パラメータを設定
	// ノコギリエネミー
	EnemyParams SawParams = EnemyParamsLoader::Get("SawEnemy");
	m_OriginalEnemy[SAW_ENEMY]->SetParams(SawParams);// パラメータを設定
	// ドライバーエネミー
	EnemyParams DriverParams = EnemyParamsLoader::Get("ScrewdriverEnemy");
	m_OriginalEnemy[DRIVER_ENEMY]->SetParams(DriverParams);// パラメータを設定
}

void EnemyManager::Load()
{
	// クローン元のエネミーをロードする
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		m_OriginalEnemy[i]->Load();
	}
}

void EnemyManager::Start()
{
	//エネミー生成
	Player* p = PlayerManager::GetInstance()->GetPlayer();
	int stageLevel = p->GetStageLevel();
	// ステージレベルに応じてエネミーの数を変える
	// ※今後の課題：jsonでエネミーの出現位置を設定できるようにする
	if (stageLevel == 1)
	{
		// ゴースト
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(30.0f, -0.5f, 53.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(280.0f, -0.5f, 85.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(279.0f, -0.5f, -81.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(162.0f, -0.5f, -51.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(-51.0f, -0.5f, 71.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(155.0f, -0.5f, 50.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(155.0f, -0.5f, 55.0f));
		// はしご
		CreateEnemy(LADDER_ENEMY)->SetPos(VGet(-50.0f, -0.5f, -53.0f));
		// ハンマー
		CreateEnemy(HAMMER_ENEMY)->SetPos(VGet(1.5f, -0.5f, 1.0f));
		CreateEnemy(HAMMER_ENEMY)->SetPos(VGet(-74.0f, -0.5f, -73.0f));
		CreateEnemy(HAMMER_ENEMY)->SetPos(VGet(191.0f, -0.5f, -51.0f));
		// バール
		CreateEnemy(BAR_ENEMY)->SetPos(VGet(-55.0f, -0.5f, 6.0f));
		CreateEnemy(BAR_ENEMY)->SetPos(VGet(-60.0f, -0.5f, 38.0f));
		CreateEnemy(SAW_ENEMY)->SetPos(VGet(75.5f, -0.5f, -80.0f));
		CreateEnemy(SAW_ENEMY)->SetPos(VGet(191.0f, -0.5f, 50.0f));
		// ノコギリ
		CreateEnemy(DRIVER_ENEMY)->SetPos(VGet(70.5f, -0.5f, -80.0f));
		CreateEnemy(DRIVER_ENEMY)->SetPos(VGet(0.5f, -0.5f, 1.0f));
		CreateEnemy(DRIVER_ENEMY)->SetPos(VGet(-60.0f, -0.5f, 38.0f));
	}
	else if (stageLevel == 2)// レベル2ゴースト生成
	{
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(30.0f, -0.5f, 53.0f));
	}
	else if (stageLevel == 3)// レベル3ゴースト生成
	{
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(280.0f, -0.5f, 85.0f));
	}
	else if (stageLevel == 4)// レベル4ゴースト生成
	{
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(162.0f, -0.5f, -51.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(-51.0f, -0.5f, 71.0f));
	}
	else if (stageLevel >= 5)// レベル5以上ゴースト生成
	{
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(30.0f, -0.5f, 53.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(280.0f, -0.5f, 85.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(279.0f, -0.5f, -81.0f));
		CreateEnemy(NORMAL_ENEMY)->SetPos(VGet(162.0f, -0.5f, -51.0f));
	}
	

	// 範囲for文で安全にリストを回せる
	for (auto& enemy : m_EnemyList)
	{
		enemy->Start();
	}

}

void EnemyManager::Step()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		enemy->Step();
	}
}

void EnemyManager::Update()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		if (!enemy->GetActive()) continue;
		enemy->Update();   
	}

	// リスポーン処理
	ProcessPendingDeletes();
	ProcessRespawns();
}

void EnemyManager::Draw()
{
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		if (enemy->GetType() == ENEMY_TYPE_NONE)
		{
			//何もしない
		}
		else
		{
			Player* player = PlayerManager::GetInstance()->GetPlayer();
			VECTOR playerPos = player->GetPos();
			if (!enemy->GetActive()) continue;

			VECTOR pos = enemy->GetPos();

			float dist = VSize(VSub(playerPos, pos));

			if (dist > ENEMY_DISTANCE) continue; // 遠いから描画しない
			enemy->Draw();
#ifdef _DEBUG
			enemy->DebugDraw();  // ★ デバッグ描画
#endif
		}		
	}

}

void EnemyManager::Fin()
{
	// 保留中削除リストとリスポーンリストをクリア
	g_PendingDelete.clear();
	// リスポーンリストをクリア
	g_RespawnList.clear();
	// 範囲for文で安全にリストを回せる
	for (auto enemy : m_EnemyList)
	{
		if (enemy)
		{
			delete enemy;
			enemy = nullptr;
		}
	}

	// リストをクリア
	m_EnemyList.clear();

	// クローン元も削除する
	for (auto enemy : m_OriginalEnemy)
	{
		delete enemy;
		enemy = nullptr;
	}
}

EnemyBase* EnemyManager::CreateEnemy(EnemyType type)
{
	// タイプに合わせたエネミーをクローンで生成
	EnemyBase* enemy = m_OriginalEnemy[type]->Clone();

	// 生成したエネミーを管理用リストに追加
	m_EnemyList.push_back(enemy);

	// 返却すれば生成した後にいろいろいじれる
	return enemy;
}

void EnemyManager::ScheduleRespawn(EnemyType type, const VECTOR& pos, unsigned int delayMs)
{
	for (auto& r : g_RespawnList)
	{
		if (r.type == type &&
			r.pos.x == pos.x &&
			r.pos.y == pos.y &&
			r.pos.z == pos.z)
		{
			return; // すでに登録済み
		}
	}

	RespawnEntry e;
	e.type = type;
	e.pos = pos;
	e.spawnTimeMs = GetNowCount() + delayMs;
	g_RespawnList.push_back(e);
}

void EnemyManager::OnEnemyDead(EnemyBase* enemy, unsigned int delayMs)
{
	if (!enemy) return;

	// 取得情報
	EnemyType t = static_cast<EnemyType>(enemy->GetType());
	VECTOR p = enemy->GetPos();

	// 削除は遅延させる：二重登録チェック
	if (std::find(g_PendingDelete.begin(), g_PendingDelete.end(), enemy) == g_PendingDelete.end())
	{
		enemy->SetActive(false); // 念のため
		g_PendingDelete.push_back(enemy);
	}

	// リスポーン登録（delayMs ミリ秒後に再生成）
	ScheduleRespawn(t, p, delayMs);
}

void EnemyManager::ProcessRespawns()
{
	unsigned int now = GetNowCount();
	// spawn 時刻を満たしたものを集める
	for (auto it = g_RespawnList.begin(); it != g_RespawnList.end(); )
	{
		if (now >= it->spawnTimeMs)
		{
			// 再生成
			EnemyBase* e = CreateEnemy(it->type);
			if (e)
			{
				e->SetPos(it->pos);
				e->ResetStatus();
				e->SetKilledFlag(true);
				e->Start();
				e->SetRespawnFlag(false);
			}
			// リストから削除
			it = g_RespawnList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EnemyManager::ProcessPendingDeletes()
{
	if (g_PendingDelete.empty()) return;

	for (auto enemy : g_PendingDelete)
	{
		if (!enemy) continue;
		// リストから削除（万が一重複して残っている場合に備える）
		auto it = std::find(m_EnemyList.begin(), m_EnemyList.end(), enemy);
		if (it != m_EnemyList.end())
		{
			m_EnemyList.erase(it);
		}
		// 実際のメモリ解放
		delete enemy;
	}
	g_PendingDelete.clear();
}
