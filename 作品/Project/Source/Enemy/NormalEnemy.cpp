#include "NormalEnemy.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionSphere.h"
#include "../EnemyShot/EnemyShotManager.h"
#include "../Player/PlayerManager.h"
#include "../Animation/AnimationManager.h"
#include "../Sound/SoundManager.h"

constexpr int SHOT_INTERVAL = 100;

NormalEnemy::NormalEnemy()
{
	m_ShotInterval = 0;
	m_Hp = 0;
	m_Speed = 0.0f;
	m_AttackInterval = 0;
	m_DetectionRange = 0.0f;
	m_EffectTimer = 0;
	m_State = EnemyState::Wander;
	m_AttackFlag = false;
}

NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Init()
{
}

void NormalEnemy::Load()
{
	m_Handle = MV1LoadModel("Data/Enemy/NormalEnemy/Normal_Enemy.x");

	// アニメ名とインデックスを対応付けて登録
	std::unordered_map<std::string, int> NormalEneAnim =
	{
		{"Walk",0},
		{"Attack",1}
	};

	AnimationManager::GetInstance()->RegisterAnimations(m_Handle, NormalEneAnim);
	SetupAttackSpheres();// 攻撃用の球の当たり判定を設定
	m_Params.attackSE = SE_TYPE_ENEMY_SHOT; // 攻撃SEを設定
	m_Params.deathSE = SE_TYPE_NONE; // 死亡SEを設定
}

void NormalEnemy::Start()
{
	m_ShotInterval = SHOT_INTERVAL;
	m_Hp = m_Params.hp;
	m_AttackInterval = m_Params.attackInterval;
	m_AttackFlag = false;
	m_Speed = m_Params.speed;
	m_DetectionRange = m_Params.detectionRange;
	m_Active = true;
	m_State = EnemyState::Wander;
}

void NormalEnemy::Step()
{
	EnemyBase::Step();
	AnimationManager* anim = AnimationManager::GetInstance();
	EnemyShotManager* shot = EnemyShotManager::GetInstance();
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	m_ShotInterval--;
	// 一定時間ごとにショット
	if (m_Active)
	{
		// フレーム取得
		float frame = anim->GetCurrentFrame(m_Handle);
		// チェイス状態のときのみ攻撃
		if (m_Chase)
		{
			if (m_ShotInterval <= 0)
			{
				AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Attack", false);
				SoundManager::GetInstance()->PlaySE(m_Params.attackSE);
				// 攻撃判定をフレーム範囲内でかつまだヒットしていない場合のみ
				if (frame >= m_Params.attackHitStart && frame <= m_Params.attackHitEnd)
				{
					// 敵の位置からプレイヤー方向を算出
					VECTOR dir = VSub(player->GetPos(), m_Pos);
					dir = VNorm(dir);
					VECTOR shotpos = VAdd(m_Pos, VGet(0.0f, 8.0f, 0.0f)); // 少し上から撃つ
					shot->FireShot(shotpos, dir, ENEMY_NORMAL, 0.6f ,80);

					m_ShotInterval = SHOT_INTERVAL;
				}
			}

		}
	}
}

void NormalEnemy::Update()
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
EnemyBase* NormalEnemy::Clone()
{
	// クローン用のオブジェクトを生成
	NormalEnemy* clone = new NormalEnemy;

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
	std::unordered_map<std::string, int> ladderAnim =
	{
		{"Attack",0},
		{"Walk",  1}
	};
	AnimationManager::GetInstance()->RegisterAnimations(clone->m_Handle, ladderAnim);

	// 出来上がったクローンを返却
	return clone;
}

