#pragma once
#include "EnemyShotBase.h"
#include <list>

enum EnemyShotType
{
	ENEMY_NORMAL,
	ENEMY_SHOT_TYPE_MAX,
	ENEMY_SHOT_TYPE_NONE = -1
};

class EnemyShotManager
{
public:
	EnemyShotManager();
	~EnemyShotManager();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new EnemyShotManager; }
	static EnemyShotManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance)delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();
	void FireShot(VECTOR pos, VECTOR dir, EnemyShotType type, float speed ,int shotlife);

public:
	EnemyShotBase* CreateShot(EnemyShotType type);

	std::list<EnemyShotBase*> GetShotList() { return m_ShotList; }

private:
	static EnemyShotManager* m_Instance;

	// クローン元のエネミーを管理する配列
	EnemyShotBase* m_OriginalShot[ENEMY_SHOT_TYPE_MAX];

	// C++標準ライブラリのリストクラス
	// リストによる管理が簡単にできる
	std::list<EnemyShotBase*> m_ShotList;
};
