#include "StageObject.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionAABB.h" 
#include "../../Collision/CollisionOBB.h"

StageObject::StageObject()
	: m_Handle(-1)
	, m_Pos{}
	, m_Rot{}
	, m_Scale {}
	, m_AABB(nullptr)
	, m_OBB(nullptr)
{
}

StageObject::~StageObject()
{
	Fin();
}

void StageObject::Start()
{
	if (!m_AABB)
	{
		// CollisionManager で管理される AABB を作成
		m_AABB = CollisionManager::GetInstance()->CreateAABB();
		if (m_AABB)
		{
			// m_Pos のアドレスを渡して参照が切れないようにする
			m_AABB->SetTargetPos(&m_Pos);
			m_AABB->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
			// 必要に応じてモデルサイズに合わせて SetSize を呼ぶ
			m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f)); // 仮のサイズ
			m_AABB->SetActive(true);
		}	
	}
}

void StageObject::Load(const char* fileName)
{
	m_Handle = MV1LoadModel(fileName);
	MV1SetUseZBuffer(m_Handle, TRUE);
}

void StageObject::Update()
{
	MV1SetPosition(m_Handle, m_Pos);
	MV1SetRotationXYZ(m_Handle, m_Rot);
	// OBB があるならモデルの回転に合わせて軸を更新する
	if (m_OBB)
	{
		m_OBB->SetRot(m_Rot);
		m_OBB->SetTargetPos(&m_Pos);
	}
	MV1SetScale(m_Handle, m_Scale);
}

void StageObject::Draw()
{
	MV1DrawModel(m_Handle);
}

void StageObject::Fin()
{
	if (m_AABB)
	{
		CollisionManager::GetInstance()->DeleteAABB(m_AABB);
		m_AABB = nullptr;
	}

	if (m_OBB)
	{
		CollisionManager::GetInstance()->DeleteOBB(m_OBB);
		m_OBB = nullptr;
	}

	// モデルハンドルの削除は元からの処理
	if (m_Handle != -1)
	{
		MV1DeleteModel(m_Handle);
		m_Handle = -1;
	}
}
