#pragma once
#include "DxLib.h"

// 床クラス
class Floor
{
public:
	Floor();
	~Floor();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new Floor; }
	static Floor* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }
public:
	void Init();
	void Load();
	void Start();
	void Step();
	void Update();
	void Draw();
	void Fin();

private:
	int m_Handle;
	VECTOR m_Pos;
	static Floor* m_Instance;
};
