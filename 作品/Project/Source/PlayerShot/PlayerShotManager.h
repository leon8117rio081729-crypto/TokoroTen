#pragma once
#include "PlayerShotBase.h"
#include <list>

enum PlayerShotType
{
	PLAYER_NORMAL,
	PLAYER_SHOT_TYPE_MAX,
	PLAYER_SHOT_TYPE_NONE = -1
};

class PlayerShotManager
{
public:
	PlayerShotManager();
	~PlayerShotManager();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new PlayerShotManager; }
	static PlayerShotManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance)delete m_Instance; m_Instance = nullptr; }

public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();
	void FireShot(VECTOR pos, VECTOR dir, PlayerShotType type, float speed, int shotlife);

public:
	PlayerShotBase* CreateShot(PlayerShotType type);

	std::list<PlayerShotBase*> GetShotList() { return m_ShotList; }

private:
	static PlayerShotManager* m_Instance;

	// クローン元のプレイヤーショットを管理する配列
	PlayerShotBase* m_OriginalShot[PLAYER_SHOT_TYPE_MAX];

	// C++標準ライブラリのリストクラス
	// リストによる管理が簡単にできる
	std::list<PlayerShotBase*> m_ShotList;
};
