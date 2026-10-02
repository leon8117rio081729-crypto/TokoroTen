#include "Door.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Collision/CollisionAABB.h"

#define DEFAULT_POS VGet(0.0f, 0.0f, 0.0f)

void Door::Start()
{
	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	m_AABB->SetTargetPos(&m_Pos);
	m_AABB->SetSize(VGet(1.0f, 8.0f, 5.0f));
	m_AABB->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
}

StageObject* Door::Clone()
{
	Door* clone = new Door;

	*clone = *this;
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	return clone;
}

