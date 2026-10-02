#pragma once
#include "EnemyBase.h"
#include "EnemyType.h"
#include "../Player/Player.h"
#include <list>

constexpr float ENEMY_DISTANCE = 150.0f;

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new EnemyManager; }
	static EnemyManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance)delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();

public:
	EnemyBase* CreateEnemy(EnemyType type);// エネミー生成
	// 指定時間後にエネミーをリスポーンさせる
	void ScheduleRespawn(EnemyType type, const VECTOR& pos, unsigned int delayMs);
	// エネミーが死亡したときの処理
	void OnEnemyDead(EnemyBase* enemy, unsigned int delayMs = 5000);
	// リスポーン処理を行う
	void ProcessRespawns();
	// 削除予定のエネミーを処理する
	void ProcessPendingDeletes();

	const std::list<EnemyBase*>& GetEnemyList() const { return m_EnemyList; }

	EnemyBase* GetEnemies() {
		if (m_EnemyList.empty()) return nullptr;
		return m_EnemyList.front();
	}

private:
	static EnemyManager* m_Instance;

	// クローン元のエネミーを管理する配列
	EnemyBase* m_OriginalEnemy[ENEMY_TYPE_MAX];

	// C++標準ライブラリのリストクラス
	// リストによる管理が簡単にできる
	std::list<EnemyBase*> m_EnemyList;
};
