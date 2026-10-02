#pragma once
#include "DxLib.h"

class CollisionAABB;
class CollisionOBB;

class StageObject
{
public:
	StageObject();
	virtual ~StageObject();

	virtual void Start();
	void Load(const char* fileName);
	void Update();
	void Draw();
	void Fin();
	virtual StageObject* Clone() = 0;

	VECTOR GetPos() const { return m_Pos; }
	CollisionAABB* GetAABB() const { return m_AABB; }
	CollisionOBB* GetOBB() const { return m_OBB; }

	void SetTransform(VECTOR pos, VECTOR rot, VECTOR scale) { m_Pos = pos; m_Rot = rot; m_Scale = scale; }

protected:
	int m_Handle;
	VECTOR m_Pos;
	VECTOR m_Rot;
	VECTOR m_Scale;
	CollisionAABB* m_AABB;	// 当たり判定
	CollisionOBB* m_OBB;	// 当たり判定(回転あり)

};
