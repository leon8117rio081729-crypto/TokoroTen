#pragma once

#include "DxLib.h"

class CollisionSphere;

class EnemyShotBase
{
public:
	EnemyShotBase();
	virtual ~EnemyShotBase();

public:
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Start();
	virtual void Step();

	// 各エネミー専用で処理を作る必要がない場合は基底クラスで共通処理にする
	virtual void Update();
	virtual void Draw();
	virtual void Fin();

	// 複製、量産するためのクローン関数
	virtual EnemyShotBase* Clone() = 0;

public:
	void SetPos(const VECTOR& pos) { m_Pos = pos; }
	void SetMove(const VECTOR& move) { m_Move = move; }
	void SetActive(bool active) { m_IsShotActive = active; }
	bool IsActive() { return m_IsShotActive; }

	void Respawn(int shotlife);
	void SetType(int type) { m_Type = type; }
	int GetType() { return m_Type; }
	CollisionSphere* GetSphereCollision() { return m_SphereCollision; }

protected:
	int m_Type;
	int m_Handle;
	bool m_IsShotActive;
	int m_ShotLife;
	VECTOR m_Pos;
	VECTOR m_Rot;
	VECTOR m_Move;
	CollisionSphere* m_SphereCollision;	// 球の当たり判定

};


