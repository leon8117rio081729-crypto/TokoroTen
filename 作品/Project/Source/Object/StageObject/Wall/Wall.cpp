#include "Wall.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Collision/CollisionAABB.h"
#include "../../../MyMath/MyMath.h"

VECTOR WALLAABB_SIZE[WALL_MAX] = 
{
    VGet(10.0f, 15.0f, 1.0f), // WALL_00 
    VGet(1.0f, 15.0f, 10.0f), // WALL_01
    VGet(10.0f, -15.0f, 1.0f),// WALL_02
    VGet(1.0f, -15.0f, 10.0f) // WALL_03
};

void Wall::Start()
{
    VECTOR size = WALLAABB_SIZE[m_id];
    m_AABB = CollisionManager::GetInstance()->CreateAABB();
    m_AABB->SetTargetPos(&m_Pos);
    m_AABB->SetSize(size);
    m_AABB->SetLocalPos(VGet(0.0f, size.y * 0.5f, 0.0f));
    // 当たり判定を有効化しておく
    m_AABB->SetEnable(true);
}

StageObject* Wall::Clone()
{
	Wall* clone = new Wall;

	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return clone;
}

