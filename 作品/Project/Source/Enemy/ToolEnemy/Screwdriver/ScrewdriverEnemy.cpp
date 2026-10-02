#include "ScrewdriverEnemy.h"
#include "../../../MyMath/MyMath.h"
#include "../../EnemyParamsLoader.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Collision/CollisionSphere.h"
#include "../../../Collision/CollisionAABB.h"
#include "../../../EnemyShot/EnemyShotManager.h"
#include "../../../Player/PlayerManager.h"
#include "../../../Sound/SoundManager.h"
#include "../../../Effect/EffectManager.h"
#include "../../../Animation/AnimationManager.h"
#include "../../../Item/ItemManager.h"
#include "../../../Item/Medicine/Effect/Heal.h"
#include "../../../Item/Medicine/MedicineItem.h"
#include "../../../Item/Tool/ToolItem.h"
#include "../../../Item/Tool/Effect/Screwdriver/Screwdriver.h"

ScrewdriverEnemy::ScrewdriverEnemy()
{
	m_Hp = 0;
	m_Speed = 0.0f;
	m_AttackInterval = 0;
	m_DetectionRange = 0.0f;
	m_EffectTimer = 0;
	m_State = EnemyState::Wander;
	m_AttackFlag = false;
}

ScrewdriverEnemy::~ScrewdriverEnemy()
{
}

void ScrewdriverEnemy::Init()
{
}

void ScrewdriverEnemy::Load()
{
	m_Handle = MV1LoadModel("Data/Enemy/ToolEnemy/ScrewdriverEnemy/Screwdriver.x");

	// アニメ名とインデックスを対応付けて登録
	std::unordered_map<std::string, int> driverAnim =
	{
		{"Attack",0},
		{"Walk",  1}
	};

	AnimationManager::GetInstance()->RegisterAnimations(m_Handle, driverAnim);
	SetupAttackSpheres();// 攻撃用の球の当たり判定を設定
	m_Params.attackSE = SE_TYPE_TOOLENEMY_DRIVER_ATTACK; // 攻撃SEを設定
	m_Params.deathSE = SE_TYPE_TOOLENEMY_DRIVER_DEAD; // 死亡SEを設定
}

void ScrewdriverEnemy::Start()
{
	m_Hp = m_Params.hp;
	m_AttackInterval = m_Params.attackInterval;
	m_AttackFlag = false;
	m_Speed = m_Params.speed;
	m_DetectionRange = m_Params.detectionRange;
	m_Active = true;
	m_State = EnemyState::Wander;
}

void ScrewdriverEnemy::Step()
{
	EnemyBase::Step();
}

void ScrewdriverEnemy::Update()
{
	EnemyBase::Update();
	// ★ 攻撃アニメ再生中なら Walk に上書きしない
	if (AnimationManager::GetInstance()->IsPlaying(m_Handle, "Attack"))
	{
		// Attack再生中なら何もしない
	}
	else if (m_State == EnemyState::Chase || m_State == EnemyState::Wander)
	{
		AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Walk", true);
	}
	AnimationManager::GetInstance()->UpdateAnimation(m_Handle);
}

// 呼ばれたオブジェクトの複製を作る関数
EnemyBase* ScrewdriverEnemy::Clone()
{
	// クローン用のオブジェクトを生成
	ScrewdriverEnemy* clone = new ScrewdriverEnemy;

	// パラメータ（設計データ）
	clone->SetParams(this->GetParams());
	clone->SetType(this->GetType());

	// 当たり判定（個体ごと）
	clone->m_SphereCollision = nullptr;
	clone->m_AttackSpheres.clear();
	clone->SetupAttackSpheres();
	// モデル
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 状態初期化（HP / フラグ）
	clone->ResetStatus();

	// Duplicate したハンドル用にアニメ登録をもう一度する
	std::unordered_map<std::string, int> driverAnim =
	{
		{"Attack",0},
		{"Walk",  1}
	};
	AnimationManager::GetInstance()->RegisterAnimations(clone->m_Handle, driverAnim);

	// 出来上がったクローンを返却
	return clone;
}
