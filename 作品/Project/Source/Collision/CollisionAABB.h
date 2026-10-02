#pragma once
#include "DxLib.h"

class CollisionAABB
{
public:
	CollisionAABB();
	~CollisionAABB();

public:
	void Draw();			// 描画

public:
	void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
	void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
	void SetWorldPos(VECTOR worldPos);
	void SetSize(VECTOR size) { m_Size = size; }

	VECTOR GetLocalPos() const { return m_LocalPos; }
	VECTOR GetSize() const { return m_Size; }
	VECTOR GetTargetPos() const { return m_UseWorldPos ? m_WorldPos : *m_TargetPos; }
	VECTOR GetMin();
	VECTOR GetMax();
	VECTOR GetCenter() const ;
	VECTOR GetHalfSize() const;

public:
	bool CheckAABB(const CollisionAABB* other) const;
	static bool RayIntersectsAABB(const VECTOR& ro, const VECTOR& rd, const VECTOR& aabbMin, const VECTOR& aabbMax, float* outT);
	bool IsActive() const { return m_IsCollisionActive; }
	void SetActive(bool active) { m_IsCollisionActive = active; }
	// 当たり判定の有効/無効を切り替える
	void SetEnable(bool enable) { m_IsCollisionActive = enable; }
	bool m_IsCollisionActive = true; // 当たり判定の有効無効
private:
	// 対象の座標
	VECTOR* m_TargetPos;
	// 対象の座標を原点としたローカル座標
	VECTOR m_LocalPos;
	// 縦横奥行き幅
	VECTOR m_Size;
	// ワールド座標を直接指定する場合の座標
	VECTOR m_WorldPos;
	// ワールド座標を直接指定するかどうか
	bool m_UseWorldPos;
	
};
