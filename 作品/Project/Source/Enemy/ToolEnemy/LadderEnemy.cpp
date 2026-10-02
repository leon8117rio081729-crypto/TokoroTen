#include "LadderEnemy.h"
#include "../../MyMath/MyMath.h"
#include "../EnemyParamsLoader.h"
#include "../../Collision/CollisionManager.h"
#include "../../Collision/CollisionSphere.h"
#include "../../Collision/CollisionAABB.h"
#include "../../EnemyShot/EnemyShotManager.h"
#include "../../Player/PlayerManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Effect/EffectManager.h"
#include "../../Animation/AnimationManager.h"
#include "../../Item/ItemManager.h"
#include "../../Item/Medicine/Effect/Heal.h"
#include "../../Item/Medicine/MedicineItem.h"
#include "../../Item/Tool/ToolItem.h"
#include "../../Item/Tool/Effect/Ladder/Ladder.h"


// 定数
#define ENEMY_DETECTIONRANGE 25.0f



LadderEnemy::LadderEnemy()
{
	m_Hp = 0;
	m_Speed = 0.0f;
	m_AttackInterval = 0;
	m_DetectionRange = 0.0f;
	m_EffectTimer = 0;
	m_State = EnemyState::Wander;
	m_AttackFlag = false;
	m_AttackAABB = nullptr;
}

LadderEnemy::~LadderEnemy()
{
}

void LadderEnemy::Init()
{
}

void LadderEnemy::Load()
{
	m_Handle = MV1LoadModel("Data/Enemy/ToolEnemy/LadderEnemy/Ladder.x");

	// アニメ名とインデックスを対応付けて登録
	std::unordered_map<std::string, int> ladderAnim =
	{
		{"Attack",0},
		{"Walk",  1}
	};

	AnimationManager::GetInstance()->RegisterAnimations(m_Handle, ladderAnim);

	//アニメーション再生用のアタッチインデックスを取得
	/*int animCount = MV1GetAnimNum(m_Handle);
	printfDx("LadderEnemy のアニメ数: %d\n", animCount);
	for (int i = 0; i < animCount; i++)
	{
		const char* name = MV1GetAnimName(m_Handle, i);
		printfDx("Anim[%d] = %s\n", i, name);
	}*/
}

void LadderEnemy::Start()
{
	m_Hp = m_Params.hp;
	m_AttackInterval = m_Params.attackInterval;
	m_AttackFlag = false;
	m_Speed = m_Params.speed;
	m_DetectionRange = m_Params.detectionRange;
	m_Active = true;
	m_State = EnemyState::Wander;
}

void LadderEnemy::Step()
{
	EnemyBase::Step();
}

void LadderEnemy::Update()
{
	EnemyBase::Update();
	AnimationManager::GetInstance()->UpdateAnimation(m_Handle);
}

// 呼ばれたオブジェクトの複製を作る関数
EnemyBase* LadderEnemy::Clone()
{
	// クローン用のオブジェクトを生成
	LadderEnemy* clone = new LadderEnemy;

	// 自身の中身をクローンにコピー
	*clone = *this;
	// パラメータをコピー
	clone->SetParams(this->GetParams());
	// 攻撃用のAABBの当たり判定を設定
	clone->m_AttackAABB = CollisionManager::GetInstance()->CreateAABB();
	clone->m_AttackAABB->SetTargetPos(&clone->m_Pos);
	clone->m_AttackAABB->SetLocalPos(VGet(0.0f, 2.0f, 4.0f));
	clone->m_AttackAABB->SetSize(VGet(4.0f, 3.0f, 4.0f));
	clone->m_AttackAABB->SetEnable(false);                   // 最初は無効化
	// 球の当たり判定を設定
	clone->m_SphereCollision = CollisionManager::GetInstance()->CreateSphere();
	clone->m_SphereCollision->SetTargetPos(&clone->m_Pos);
	clone->m_SphereCollision->SetLocalPos(VGet(0.0f, 0.5f, 0.0f));
	clone->m_SphereCollision->SetRadius(0.5f);
	//タイプを設定
	clone->SetType(1);

	// Duplicate したハンドル用にアニメ登録をもう一度する
	std::unordered_map<std::string, int> ladderAnim =
	{
		{"Attack",0},
		{"Walk",  1}
	};
	AnimationManager::GetInstance()->RegisterAnimations(clone->m_Handle, ladderAnim);

	// 画像はDuplicateする必要がある
	clone->m_Handle = MV1DuplicateModel(m_Handle);

	// 出来上がったクローンを返却
	return clone;
}

void LadderEnemy::Attack(Player* player)
{
	// プレイヤーとの距離を測る
	float distance = VSize(MyMath::VecSub(player->GetPos(), m_Pos));

	if (m_AttackFlag)
	{
		if (distance < 2.0f)  // 2.0f以内なら攻撃ヒット
		{
			// プレイヤーの向きに攻撃AABBを回転させる
			MATRIX rotY = MyMath::MatRotationYaw(m_Rot.y);
			VECTOR rotatedPos = MyMath::MatTransform(rotY, VGet(0.0f, 2.0f, 3.0f)); // ローカルオフセット
			m_AttackAABB->SetLocalPos(rotatedPos);
			m_AttackAABB->SetEnable(true);  // 有効化
			player->HitEnemy(); // プレイヤーにダメージ（1だけ減らす）
			SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_DAMAGE); // 攻撃音
			EffectManager::GetInstance()->PlayEffect(EFFECT_KILL, player->GetPos()); // ヒットエフェクト
		}
		else
		{
			m_AttackAABB->SetEnable(false); // 無効化
		}
	}
	else
	{
		m_AttackAABB->SetEnable(false); // 攻撃してないときは必ずOFF
	}

	// 攻撃後に待機
	m_AttackInterval = m_Params.attackInterval;
	m_AttackFlag = false;
}
