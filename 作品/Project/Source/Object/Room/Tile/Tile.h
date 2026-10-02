#pragma once
#include "DxLib.h"

class CollisionAABB;

// 床クラス
class Tile
{
public:
	Tile();
	~Tile();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new Tile; }
	static Tile* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }
public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();

	Tile* GetTile() { return m_Tile; }
	CollisionAABB* GetAABB() { return m_AABB; }

	void SetPos(const VECTOR& pos) { m_Pos = pos; }

private:
	int m_Handle;
	VECTOR m_Pos;
	static Tile* m_Instance;
	CollisionAABB* m_AABB;
	Tile* m_Tile;
};
