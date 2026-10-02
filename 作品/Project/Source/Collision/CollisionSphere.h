#pragma once
#include "../MyMath/MyMath.h" 
#include "DxLib.h"

class MyMath;
class CollisionAABB;

class CollisionSphere
{
public:
	CollisionSphere();
	~CollisionSphere();

public:
	void Draw();			// 描画

public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetWorldPos(const VECTOR& pos) { m_WorldPos = VAdd(pos, m_LocalPos); }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetRadius(float radius) { m_Radius = radius; }

	VECTOR GetTargetPos() const { return (m_TargetPos) ? *m_TargetPos : m_WorldPos; }
	VECTOR GetLocalPos() const { return m_LocalPos; }
	VECTOR GetWorldPos() const;
	float GetRadius() const { return m_Radius; }
	//VECTOR GetWorldPos() const { return MyMath::VecAdd(*m_TargetPos, m_LocalPos); }

public:
	bool CheckSphere(CollisionSphere* other);
	bool CheckAABB(class CollisionAABB* other)const;
	// 当たり判定の有効/無効を切り替える
	void SetEnable(bool enable) { m_IsCollisionActive = enable; }
	bool IsEnable() const { return m_IsCollisionActive; }
private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// ゲーム空間での実際の位置
	VECTOR m_WorldPos;  
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 球の半径
	float m_Radius;
	bool m_IsCollisionActive = true; // 当たり判定の有効無効
};
