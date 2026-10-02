#include "EnemyShot.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "ShotManager.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionSphere.h"

#define ROTATION_SPEED 0.1f
#define MOVE_SPEED 0.1f

EnemyShot::EnemyShot()
{
	m_ShotLife = 0;
}

EnemyShot::~EnemyShot()
{
}

void EnemyShot::Init()
{
}

void EnemyShot::Load()
{
	m_Handle = MV1LoadModel("Data/Shot/Enemy/敵弾.x");
}

void EnemyShot::Start()
{
	ShotBase::Start();
	m_ShotLife = 60;

}

void EnemyShot::Step()
{
	ShotBase::Step();
	if (m_IsShotActive)
	{
		// 弾は回転前進
		m_Rot.y += ROTATION_SPEED;
		m_Pos.z -= MOVE_SPEED;
	}
	
}

// 呼ばれたオブジェクトの複製を作る関数
ShotBase* EnemyShot::Clone()
{
	// クローン用のオブジェクトを生成
	EnemyShot* clone = new EnemyShot;

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 球の当たり判定を設定
	clone->m_SphereCollision = CollisionManager::GetInstance()->CreateSphere();
	clone->m_SphereCollision->SetTargetPos(&clone->m_Pos);
	clone->m_SphereCollision->SetLocalPos(VGet(0.0f, 1.1f, 0.0f));
	clone->m_SphereCollision->SetRadius(0.1f);

	// 出来上がったクローンを返却
	return clone;
}
