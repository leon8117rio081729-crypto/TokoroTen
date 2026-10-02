#include "DxLib.h"
#include "CollisionSphere.h"
#include <algorithm>
#include "CollisionAABB.h"
#include "../MyMath/MyMath.h"
#include "../Color/Color.h"

#undef min
#undef max

// コンストラクタ
CollisionSphere::CollisionSphere()
    : m_TargetPos(nullptr)
    , m_WorldPos(VGet(0.0f, 0.0f, 0.0f))
    , m_LocalPos(VGet(0.0f, 0.0f, 0.0f))
    , m_Radius(0.0f)
{
}

// デストラクタ
CollisionSphere::~CollisionSphere()
{

}

void CollisionSphere::Draw()
{
	// デバッグ用の当たり判定の可視化
	VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);
	DrawSphere3D(centerPos, m_Radius, 16, WHITE, WHITE, false);
}

VECTOR CollisionSphere::GetWorldPos() const
{
    if (m_TargetPos)
    {
        return MyMath::VecAdd(*m_TargetPos, m_LocalPos);
    }  
    else
        return m_WorldPos;
}

bool CollisionSphere::CheckSphere(CollisionSphere* other)
{
    if (!other) return false;
    if (!m_IsCollisionActive || !other->IsEnable()) return false;

    // ★ nullptr安全版
    VECTOR centerPos = GetWorldPos();
    VECTOR otherCenterPos = other->GetWorldPos();

    VECTOR vec = MyMath::VecCreate(centerPos, otherCenterPos);
    float distance = MyMath::VecLong(vec);

    return distance <= (m_Radius + other->GetRadius());
}

bool CollisionSphere::CheckAABB(CollisionAABB* other)const
{
    VECTOR centerPos = MyMath::VecAdd(*m_TargetPos, m_LocalPos);

    // AABB の中心とサイズを取得
    VECTOR otherCenter = MyMath::VecAdd(other->GetTargetPos(), other->GetLocalPos());
    VECTOR otherSize = other->GetSize();

    // min/max を計算
    VECTOR min, max;
    min.x = otherCenter.x - otherSize.x * 0.5f;
    min.y = otherCenter.y - otherSize.y * 0.5f;
    min.z = otherCenter.z - otherSize.z * 0.5f;

    max.x = otherCenter.x + otherSize.x * 0.5f;
    max.y = otherCenter.y + otherSize.y * 0.5f;
    max.z = otherCenter.z + otherSize.z * 0.5f;

    // 最近接点を求める（クランプ）
    VECTOR closest;
    closest.x = std::max(min.x, std::min(centerPos.x, max.x));
    closest.y = std::max(min.y, std::min(centerPos.y, max.y));
    closest.z = std::max(min.z, std::min(centerPos.z, max.z));

    // 中心と最近接点の距離を測る
    VECTOR diff = VSub(centerPos, closest);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    return distSq <= (m_Radius * m_Radius);
}
