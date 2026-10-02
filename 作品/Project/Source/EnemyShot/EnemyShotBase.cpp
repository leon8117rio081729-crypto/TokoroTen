#include "EnemyShotBase.h"
#include "../MyMath/MyMath.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"

constexpr int SHOT_LIFE = 40;

EnemyShotBase::EnemyShotBase()
	: m_Type (0) 
	, m_Handle (0) 
	, m_Pos (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Rot (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Move (VGet(0.0f, 0.0f, 0.0f)) 
	, m_SphereCollision (nullptr) 
	, m_IsShotActive (false) 
	, m_ShotLife (0) 
{
}

EnemyShotBase::~EnemyShotBase()
{
	Fin();
}

void EnemyShotBase::Start()
{
	m_ShotLife = SHOT_LIFE;
}

void EnemyShotBase::Step()
{
	m_ShotLife--;

	if (m_ShotLife <= 0)
	{
		m_IsShotActive = false;
	}
}

void EnemyShotBase::Update()
{
	if (m_IsShotActive)
	{

		Player* player = PlayerManager::GetInstance()->GetPlayer();
		if (player)
		{
			VECTOR dir = VSub(player->GetPos(), m_Pos);
			dir = VNorm(dir);
			m_Move = VAdd(VScale(m_Move, 0.9f), VScale(dir, 0.1f)); // 徐々に追尾
		}
		m_Pos = MyMath::VecAdd(m_Pos, m_Move);

		MV1SetPosition(m_Handle, m_Pos);
		MV1SetRotationXYZ(m_Handle, m_Rot);
	}
}

void EnemyShotBase::Draw()
{
	if (m_IsShotActive)
	{
		MV1DrawModel(m_Handle);
	}
}

void EnemyShotBase::Fin()
{
	MV1DeleteModel(m_Handle);
}

void EnemyShotBase::Respawn(int shotlife)
{
	m_ShotLife = shotlife;
	m_IsShotActive = true;
}

