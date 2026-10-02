#include "EnemyNormalShot.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionSphere.h"

constexpr float ROTATION_SPEED = 0.1f;
constexpr float MOVE_SPEED = 0.35f;

EnemyNormalShot::EnemyNormalShot()
	: m_ShotLife(0)
{	
}

EnemyNormalShot::~EnemyNormalShot()
{
}

void EnemyNormalShot::Init()
{
}

void EnemyNormalShot::Load()
{
	m_Handle = MV1LoadModel("Data/Shot/Enemy/EnemyBullet.x");
}

void EnemyNormalShot::Start()
{
	EnemyShotBase::Start();
	m_ShotLife = 60;
}

void EnemyNormalShot::Step()
{
	EnemyShotBase::Step();
	if (m_IsShotActive)
	{
		// 通常弾は前進
		m_Pos.z -= MOVE_SPEED;
	}
}

// 呼ばれたオブジェクトの複製を作る関数
EnemyShotBase* EnemyNormalShot::Clone()
{
	// クローン用のオブジェクトを生成
	EnemyNormalShot* clone = new EnemyNormalShot;

	// 自身の中身をクローンにコピー
	*clone = *this;

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 球の当たり判定を設定
	clone->m_SphereCollision = CollisionManager::GetInstance()->CreateSphere();
	clone->m_SphereCollision->SetTargetPos(&clone->m_Pos);
	clone->m_SphereCollision->SetLocalPos(VGet(0.1f, 0.03f, 0.0f));
	clone->m_SphereCollision->SetRadius(0.3f);

	// 出来上がったクローンを返却
	return clone;
}
