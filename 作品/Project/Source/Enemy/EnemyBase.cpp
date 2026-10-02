#include "EnemyBase.h"
#include "../MyMath/MyMath.h"
#include "../Color/Color.h"
#include "../Player/Player.h"
#include "../Collision/CollisionSphere.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../Animation/AnimationManager.h"
#include "../Player/PlayerManager.h"
#include "../Item/ItemManager.h"
#include "../Item/Tool/ToolItem.h"
#include "../Item/Tool/Effect/Ladder/Ladder.h"
#include "../Item/Tool/Effect/Hammer/Hammer.h"
#include "../Item/Tool/Effect/Bar/Bar.h"
#include "../Item/Tool/Effect/Saw/Saw.h"
#include "../Item/Tool/Effect/Screwdriver/Screwdriver.h"
#include "../Object/StageObject/StageObject.h"
#include "../Object/StageObject/StageObjectManager.h"
#include "../Object/Gimmick/Tool/GimmickBase.h"
#include "../Object/Gimmick/Tool/GimmickManager.h"
#include "../Enemy/EnemyManager.h"

EnemyBase::EnemyBase()
	: m_Handle (0) 
	, m_Hp (0) 
	, m_AttackInterval (0) 
	, m_Speed (0.0f) 
	, m_DetectionRange (0.0f) 
	, m_EffectTimer (0) 
	, m_State (EnemyState::Wander) 
	, m_AttackFlag (false) 
	, m_Pos (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Rot (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Move (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Active (true) 
	, m_HitThisAnim (false) 
	, m_IsAttacking (false) 
	, m_WanderTimer (0) 
	, m_IsPausing (false) 
	, m_PauseTimer (0) 
	, m_IsDead (false) 
	, m_RespawnRegistered (false) 
	, m_FirstKilled (false) 
	, m_AttackRotY(0.0f)
	, m_AttackSEPlayed(false)
	, m_Chase(false)
{
	m_SphereCollision = new CollisionSphere();
	m_SphereCollision->SetTargetPos(&m_Pos);
	m_SphereCollision->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
	m_SphereCollision->SetRadius(1.0f);// 適切な半径に設定
}

EnemyBase::~EnemyBase()
{
	// nullptrチェックをしてから削除
	if (m_SphereCollision)
	{
		delete m_SphereCollision;
		m_SphereCollision = nullptr;
	}
	
	Fin();
}

void EnemyBase::Step()
{
	// 重力処理
	m_Move.y -= GRAVITY;

	// 攻撃間隔のカウントダウン
	m_AttackInterval--;

	// 移動量を加算
	m_Pos = MyMath::VecAdd(m_Pos, m_Move);

	// 地面より下に行かないように
	if (m_Pos.y < 0.5f)
	{
		m_Pos.y = 0.5f;
		m_Move.y = 0.0f;
	}
}

void EnemyBase::Update()
{
	if (m_Active)
	{
		Player* player = PlayerManager::GetInstance()->GetPlayer();
		// プレイヤーが存在しない場合は何もしない
		if (!m_Active) return;

		// プレイヤーとの距離を測る
		VECTOR playerPos = player->GetPos();
		float distance = VSize(MyMath::VecSub(playerPos, m_Pos));

		// 状態の遷移
		if (distance < m_DetectionRange && CanSeePlayer(player))
		{
			m_State = EnemyState::Chase;
		}
		else
		{
			if (m_State != EnemyState::Wander)
			{
				// ★ Wanderに入った瞬間に強制リセット
				m_WanderTimer = 0;
				m_IsPausing = false;

				// 攻撃中フラグや判定をリセット
				m_AttackFlag = false;
				m_IsAttacking = false;
				m_HitThisAnim = false;
				// 攻撃判定OFF
				for (auto& s : m_AttackSpheres)
					s.sphere->SetEnable(false);
			}
			m_State = EnemyState::Wander;
		}
		// 状態に応じた行動
		switch (m_State)
		{
		case EnemyState::Wander:
			// ランダムにうろうろする
			Wander();
			break;
		case EnemyState::Chase:
			// プレイヤーを追いかける
			Chase(player);
			// 攻撃処理
			if (m_AttackFlag)
			{
				// 攻撃中は毎フレーム処理
				Attack(player);
			}
			else if (m_AttackInterval <= 0)
			{
				float distance = VSize(MyMath::VecSub(player->GetPos(), m_Pos));
				if (distance < m_Params.attackRange && CanSeePlayer(player)) // 射程チェック
				{
					m_AttackFlag = true;
				}
			}
			break;
		}

		//HPが0になったら死亡
		if (m_Hp <= 0 && !m_IsDead)
		{
			//死亡処理
			Dead();
			return;
		}

		m_Pos = MyMath::VecAdd(m_Pos, m_Move);

		MV1SetPosition(m_Handle, m_Pos);
		MV1SetRotationXYZ(m_Handle, m_Rot);
	}
}

void EnemyBase::Draw()
{
	if (m_Active)
	{
		// エフェクトタイマー処理
		m_EffectTimer++; // エフェクトタイマーをカウントアップ
		if (m_Active && m_EffectTimer >= 24) // 24フレームに1回だけエフェクトを回復
		{
			EffectManager::GetInstance()->PlayEffect(EFFECT_ENEMY_TOOL_IDLE, MyMath::VecAdd(m_Pos, VGet(0.0f, 8.0f, 1.0f))); // エフェクト
			m_EffectTimer = 0; // エフェクトタイマーをリセット
		}

		MV1DrawModel(m_Handle);
	}
}

void EnemyBase::Fin()
{
	if (m_Handle != 0)
	{
		MV1DeleteModel(m_Handle);
		m_Handle = 0;
	}
}

void EnemyBase::HitShot()
{
}

void EnemyBase::Damage()
{
	if (m_IsDead) return;      // ★ 死亡後は絶対ダメージを受けない
	if (!m_Active) return;

	// HPを1減らす
	m_Hp--;
	// 死亡音と重ならないようにする
	if (m_Hp > 0)
	{
		SoundManager::GetInstance()->PlaySE(SE_TYPE_ENEMY_DAMAGE); // ダメージ音
	}
}

void EnemyBase::DebugDraw()
{
#ifdef _DEBUG
	if (!m_Active) return;
	// ===== 当たり判定球を描画 =====
	DrawSphere3D(m_SphereCollision->GetWorldPos(), m_SphereCollision->GetRadius(), 16, DARKLIME, DARKLIME, TRUE);
	// ===== 検知範囲を描画 =====
	DrawSphere3D(m_Pos, m_DetectionRange, 16, RED, RED, FALSE); // 赤いワイヤーフレーム
	// ===== 攻撃判定を描画 =====
	DrawSphere3D(m_Pos, m_Params.attackRange, 16, BLUE, BLUE, FALSE); // 青いワイヤーフレーム
	// ===== 攻撃判定球の当たり判定を描画 =====
	for (auto& s : m_AttackSpheres)
	{
		if (s.sphere->IsEnable())
		{
			// 敵の向きに合わせてローカルオフセットを回転
			MATRIX rotY = MyMath::MatRotationYaw(m_Rot.y);
			VECTOR rotatedOffset = MyMath::MatTransform(rotY, s.localOffset);

			// 敵のワールド位置に加算して最終位置を求める
			VECTOR spherePos = VAdd(m_Pos, rotatedOffset);

			DrawSphere3D(spherePos, s.sphere->GetRadius(), 8, YELLOW, YELLOW, FALSE);
		}
	}	
#endif // _DEBUG
}

// プレイヤーを追いかける
void EnemyBase::Chase(Player* player)
{
	m_Chase = true;
	// 攻撃中は追いかけない
	if (m_IsAttacking)
	{
		m_Move.x = 0.0f;
		m_Move.z = 0.0f;
		return;
	}

	VECTOR playerPos = player->GetPos();
	VECTOR direction = MyMath::VecSub(playerPos, m_Pos);
	direction.y = -10.0f; // 地面を這う動き

	// プレイヤーが近すぎるときにバタつかないように制限
	float distance = VSize(direction);
	if (distance > 1.0f) {
		direction = VNorm(direction); // 単位ベクトルに正規化
		m_Move.x = direction.x * m_Speed;
		m_Move.z = direction.z * m_Speed;
		// ====== ★ 進行方向を向く ======
		// 移動方向がある場合のみ回転
		if (MyMath::VecLong(m_Move) > 0.0f)
		{
			VECTOR dir = MyMath::VecNormalize(m_Move);
			m_Rot.y = atan2f(dir.x, dir.z);
		}
	}
	else
	{
		// プレイヤーとほぼ同じ位置なら移動しない
		m_Move.x = 0.0f;
		m_Move.z = 0.0f;
	}
}

// ランダムにうろうろする
void EnemyBase::Wander()
{
	m_Chase = false;
	if (m_IsPausing)
	{
		m_Move.x = 0.0f;
		m_Move.z = 0.0f;

		m_PauseTimer--;
		if (m_PauseTimer <= 0)
		{
			m_IsPausing = false;
			m_WanderTimer = 0;
		}
		return;
	}

	if (m_WanderTimer <= 0)
	{
		// 停止モードに入る確率（歩き回りを優先したいなら低めにする）
		if (rand() % 10 == 0) // 10%の確率で停止
		{
			m_IsPausing = true;
			m_PauseTimer = 30 + rand() % 30;
			return;
		}

		// 新しいランダム方向を決める
		float randX = (float)(rand() % 200 - 100) / 100.0f;
		float randZ = (float)(rand() % 200 - 100) / 100.0f;

		VECTOR dir = VGet(randX, 0.0f, randZ);
		if (VSize(dir) < 0.01f) dir = VGet(1.0f, 0.0f, 0.0f);
		dir = VNorm(dir);

		// ★ 最低限のスピードを保証（ゼロ移動防止）
		float speed = (m_Speed > 0.05f ? m_Speed : 0.05f);
		m_Move.x = dir.x * speed;
		m_Move.z = dir.z * speed;

		// ====== ★ 進行方向を向く処理 ======
		// 移動方向がある場合のみ回転
		if (MyMath::VecLong(m_Move) > 0.0f)
		{
			VECTOR dir = MyMath::VecNormalize(m_Move);
			m_Rot.y = atan2f(dir.x, dir.z);
		}

		m_WanderTimer = 60 + rand() % 120;
	}

	m_WanderTimer--;

	// 範囲外に出たら反転
	if (m_Pos.x < AREA_MIN_X || m_Pos.x > AREA_MAX_X)
	{
		m_Move.x = -m_Move.x;
		m_WanderTimer = 0;
	}
	if (m_Pos.z < AREA_MIN_Z || m_Pos.z > AREA_MAX_Z)
	{
		m_Move.z = -m_Move.z;
		m_WanderTimer = 0;
	}
}

// 死亡処理
void EnemyBase::Dead()
{
	if (m_IsDead) return;
	m_IsDead = true;
	m_Active = false;
	SoundManager::GetInstance()->PlaySE(m_Params.deathSE); // 倒した音
	EffectManager::GetInstance()->PlayEffect(EFFECT_KILL, m_Pos); // エフェクト

	// 攻撃判定OFF
	for (auto& s : m_AttackSpheres)
	{
		s.sphere->SetEnable(false);
	}
		
	// 初回撃破時のみ道具をドロップ
	if (!m_FirstKilled)
	{
		// JSONから設定されたドロップ情報を使う
		if (m_Params.dropType == "Tool")
		{
			// ドロップする道具の種類に応じて生成
			// 梯子をドロップする場合
			if (m_Params.dropEffect == "Ladder")
			{
				ItemManager::GetInstance()->SpawnItem(std::make_unique<ToolItem>(m_Params.dropName, m_Params.dropDescription, ItemKey::TOOL_LADDER, std::make_unique<Ladder>(m_Params.value), ToolType::LADDER), m_Pos, ItemKey::TOOL_LADDER, false);
			}
			// ハンマーをドロップする場合
			else if (m_Params.dropEffect == "Hammer")
			{
				ItemManager::GetInstance()->SpawnItem(std::make_unique<ToolItem>(m_Params.dropName, m_Params.dropDescription, ItemKey::TOOL_HAMMER, std::make_unique<Hammer>(m_Params.value), ToolType::HAMMER), m_Pos, ItemKey::TOOL_HAMMER, false);
			}
			// バールをドロップする場合
			else if (m_Params.dropEffect == "Bar")
			{
				ItemManager::GetInstance()->SpawnItem(std::make_unique<ToolItem>(m_Params.dropName, m_Params.dropDescription, ItemKey::TOOL_BAR, std::make_unique<Bar>(m_Params.value), ToolType::BAR), m_Pos, ItemKey::TOOL_BAR, false);
			}
			// ノコギリをドロップする場合
			else if (m_Params.dropEffect == "Saw")
			{
				ItemManager::GetInstance()->SpawnItem(std::make_unique<ToolItem>(m_Params.dropName, m_Params.dropDescription, ItemKey::TOOL_SAW, std::make_unique<Saw>(m_Params.value), ToolType::SAW), m_Pos, ItemKey::TOOL_SAW, false);
			}
			// ドライバーをドロップする場合
			else if (m_Params.dropEffect == "Screwdriver")
			{
				ItemManager::GetInstance()->SpawnItem(std::make_unique<ToolItem>(m_Params.dropName, m_Params.dropDescription, ItemKey::TOOL_DRIVER, std::make_unique<Screwdriver>(m_Params.value), ToolType::DRIVER), m_Pos, ItemKey::TOOL_DRIVER, false);
			}
		}
	}

	// リスポーン登録は一度だけ
	if (!m_RespawnRegistered)
	{
		m_RespawnRegistered = true;
		// リスポーン登録
		EnemyManager::GetInstance()->OnEnemyDead(this, 120000);// 2分後にリスポーン
	}

}

// 攻撃処理
void EnemyBase::Attack(Player* player)
{
	if (!m_AttackFlag) return;

	AnimationManager* anim = AnimationManager::GetInstance();

	// 攻撃アニメが再生されていなければ再生
	if (!anim->IsPlaying(m_Handle, "Attack"))
	{
		anim->PlayAnimation(m_Handle, "Attack", false);
		m_HitThisAnim = false;         // 今回のアニメではまだヒットしていない
		m_AttackSEPlayed = false;      // 攻撃SE未再生
		m_IsAttacking = true;          // 攻撃モーション中
		// ★ 攻撃判定ON（全部ON）
		for (auto& s : m_AttackSpheres)
			s.sphere->SetEnable(true);
	}

	// 攻撃中は移動を止める
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;

	// フレーム取得
	float frame = anim->GetCurrentFrame(m_Handle);

	// 攻撃判定をフレーム範囲内でかつまだヒットしていない場合のみ
	if (frame >= m_Params.attackHitStart && frame <= m_Params.attackHitEnd)
	{
		// 向きに応じて各Sphereのローカルオフセットを更新
		MATRIX rotY = MyMath::MatRotationYaw(m_Rot.y);
		// ★ 各スフィアを敵の向きに合わせて更新
		for (auto& data : m_AttackSpheres)
		{
			VECTOR rotated = MyMath::MatTransform(rotY, data.localOffset);
			data.sphere->SetLocalPos(rotated);
			data.sphere->SetEnable(true);	

		}

		// ★ 攻撃音を一度だけ鳴らす
		if (!m_AttackSEPlayed)
		{
			SoundManager::GetInstance()->PlaySE(m_Params.attackSE);// 攻撃音
			m_AttackSEPlayed = true;
		}

		// ★ 当たり判定
		if (!m_HitThisAnim)
		{
			for (auto& data : m_AttackSpheres)
			{
				if (CollisionManager::CheckSphereCollision(data.sphere, player->GetSphereCollision()))
				{
					player->HitEnemy(m_Params.melee);
					SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_DAMAGE);
					EffectManager::GetInstance()->PlayEffect(EFFECT_KILL, player->GetPos());
					m_HitThisAnim = true;

					// ★ 一撃のみ有効にしたい場合 → 攻撃判定OFF
					for (auto& s : m_AttackSpheres)
						s.sphere->SetEnable(false);
					break;
				}
			}

		}
	}
	else
	{
		// 攻撃判定OFF
		for (auto& s : m_AttackSpheres)
			s.sphere->SetEnable(false);
	}

	// 攻撃アニメが終了したらフラグをリセット
	if (anim->CheckAnimationFinish(m_Handle))
	{
		m_AttackInterval = m_Params.attackInterval;
		m_AttackFlag = false;
		m_HitThisAnim = false;
		m_IsAttacking = false;
		m_AttackSEPlayed = false;
		// 攻撃判定OFF
		for (auto& s : m_AttackSpheres)
			s.sphere->SetEnable(false);
	}
}

void EnemyBase::SetupAttackSpheres()
{
	// 自分の本体当たり判定を初期化
	if (m_SphereCollision) {
		delete m_SphereCollision;
	}
	m_SphereCollision = new CollisionSphere();
	m_SphereCollision->SetTargetPos(&m_Pos);
	m_SphereCollision->SetLocalPos(m_Params.SphereCollisionPosition);
	m_SphereCollision->SetRadius(m_Params.SphereCollisionRadius);
	m_SphereCollision->SetEnable(true);

	// 攻撃スフィア群の初期化
	m_AttackSpheres.clear();
	for (int i = 0; i < m_Params.attackSphereCount; i++)
	{
		CollisionSphere* s = CollisionManager::GetInstance()->CreateSphere();
		s->SetTargetPos(&m_Pos);
		s->SetRadius(m_Params.attackSphereRadius);
		s->SetEnable(false);

		float z = m_Params.attackOffsetZ + i * m_Params.attackSphereSpacing;
		VECTOR offset = VGet(m_Params.attackOffsetX, m_Params.attackOffsetY, z);

		m_AttackSpheres.push_back({ s, offset });
	}
}

// 線分とAABBの交差判定
bool EnemyBase::IntersectSegmentAABB(VECTOR start, VECTOR end, CollisionAABB* box)
{
	VECTOR dir = MyMath::VecSub(end, start);

	float tmin = 0.0f;
	float tmax = 1.0f;

	VECTOR min = box->GetMin();
	VECTOR max = box->GetMax();

	for (int i = 0; i < 3; i++)
	{
		float s = (&start.x)[i];
		float d = (&dir.x)[i];
		float bmin = (&min.x)[i];
		float bmax = (&max.x)[i];

		if (fabs(d) < 1e-6f)
		{
			if (s < bmin || s > bmax)
				return false;
		}
		else
		{
			float ood = 1.0f / d;
			float t1 = (bmin - s) * ood;
			float t2 = (bmax - s) * ood;

			if (t1 > t2) std::swap(t1, t2);

			tmin = max(tmin, t1);
			tmax = min(tmax, t2);

			if (tmin > tmax)
				return false;
		}
	}
	return true;
}

bool EnemyBase::CanSeePlayer(Player* player)
{
	VECTOR from = m_Pos;
	from.y += 5.0f;

	VECTOR to = player->GetPos();
	to.y += 5.0f;

	auto& objs = StageObjectManager::GetInstance()->GetStageObjects();
	auto gimmicks = GimmickManager::GetInstance()->GetGimmickObject();

	// ステージオブジェクト
	for (auto obj : objs)
	{
		if (!obj) continue;

		CollisionAABB* box = obj->GetAABB();
		if (!box) continue;

		if (IntersectSegmentAABB(from, to, box))
		{
			return false; // 壁が遮った
		}
	}
	// ギミック
	for (auto gimmick : gimmicks)
	{
		if (!gimmick) continue;
		// 使用済みギミックは無視
		if (gimmick->m_IsUsed) continue;
		const auto& aabbs = gimmick->GetCollisionAABBData();

		for (const auto& data : aabbs)
		{
			if (!data.aabb) continue;

			if (IntersectSegmentAABB(from, to, data.aabb.get()))
				return false;
		}
	}

	return true; // 見えてる
}


void EnemyBase::ResetStatus()
{
	m_IsDead = false;
	m_Active = true;
	m_RespawnRegistered = false;
	m_Hp = m_Params.hp;
}

void EnemyBase::CheckHitStageObjects(const std::vector<StageObject*>& objects)
{
	if (!m_SphereCollision) return;

	for (auto obj : objects)
	{
		if (!obj) continue;
		CollisionAABB* objAABB = obj->GetAABB();
		CollisionOBB* objOBB = obj->GetOBB();
		if (!objAABB) continue;

		// 球の中心と半径を取得
		VECTOR sphereCenter = m_SphereCollision->GetWorldPos();
		float radius = m_SphereCollision->GetRadius();

		// AABB の最小/最大を取得（ワールド座標）
		VECTOR objMin = objAABB->GetMin();
		VECTOR objMax = objAABB->GetMax();

		// AABB 上の最近接点を求める (clamp)
		VECTOR closest;
		closest.x = max(objMin.x, min(sphereCenter.x, objMax.x));
		closest.y = max(objMin.y, min(sphereCenter.y, objMax.y));
		closest.z = max(objMin.z, min(sphereCenter.z, objMax.z));

		// 中心と最近接点の差分（球と箱の最短ベクトル）
		VECTOR delta = MyMath::VecSub(sphereCenter, closest);
		float distSq = MyMath::VecSizeSq(sphereCenter, closest);

		// 重なり判定
		if (distSq < radius * radius)
		{
			float dist = sqrtf(distSq);
			float penetration = radius - dist;
			VECTOR push;

			// 距離がほとんどゼロなら Y+ 方向へ押し出す（中心が箱内部にある場合の代替）
			if (dist > 1e-6f)
			{
				VECTOR dir = MyMath::VecNormalize(delta);
				push = MyMath::VecScale(dir, penetration);
			}
			else
			{
				push = VGet(0.0f, penetration, 0.0f);
			}

			if (fabsf(push.y) > fabsf(push.x) && fabsf(push.y) > fabsf(push.z))
			{
				m_Move.y = 0.0f;
			}

			// 位置を押し戻す
			m_Pos = MyMath::VecAdd(m_Pos, push);

			// 更新した球中心を次の判定に反映（チェーン反応防止のため）
			sphereCenter = MyMath::VecAdd(sphereCenter, push);
			m_SphereCollision->SetWorldPos(sphereCenter);
		}
	}
}

void EnemyBase::CheckHitGimmickObjects(const std::vector<GimmickBase*>& gimmicks)
{
	if (gimmicks.empty()) return;

	// 球の中心と半径を取得（一度だけ）
	VECTOR sphereCenter = m_SphereCollision->GetWorldPos();
	float radius = m_SphereCollision->GetRadius();

	for (auto gimmick : gimmicks)
	{
		if (!gimmick) continue;
		if (!gimmick->m_Active || gimmick->m_IsUsed) continue;
		const auto& datas = gimmick->GetCollisionAABBData();
		for (const auto& data : datas)
		{
			CollisionAABB* gaabb = data.aabb.get();
			if (!gaabb) continue;

			// gaabb をギミックの座標に合わせる（内部で localOffset を使う想定）
			gaabb->SetWorldPos(VAdd(gimmick->GetPos(), data.localOffset));

			// AABB の最小/最大（ワールド座標）
			VECTOR aMin = gaabb->GetMin();
			VECTOR aMax = gaabb->GetMax();

			// AABB 上の最近接点を求める（clamp）
			VECTOR closest;
			closest.x = max(aMin.x, min(sphereCenter.x, aMax.x));
			closest.y = max(aMin.y, min(sphereCenter.y, aMax.y));
			closest.z = max(aMin.z, min(sphereCenter.z, aMax.z));

			// 中心と最近接点の差分
			VECTOR delta = MyMath::VecSub(sphereCenter, closest);
			float distSq = MyMath::VecSizeSq(sphereCenter, closest);

			// 重なり判定
			if (distSq < radius * radius)
			{
				float dist = sqrtf(distSq);
				float penetration = radius - dist;
				VECTOR push;

				// 正規化可能ならその方向へ、そうでなければ Y+ に押し出す
				if (dist > 1e-6f)
				{
					VECTOR dir = MyMath::VecNormalize(delta);
					push = MyMath::VecScale(dir, penetration);
				}
				else
				{
					push = VGet(0.0f, penetration, 0.0f);
				}

				// 垂直方向の押し戻しなら垂直速度を止める
				if (fabsf(push.y) > fabsf(push.x) && fabsf(push.y) > fabsf(push.z))
				{
					m_Move.y = 0.0f;
				}

				// 位置を押し戻す
				m_Pos = MyMath::VecAdd(m_Pos, push);

				// 更新した球中心を次の判定に反映（チェーン反応防止のため）
				sphereCenter = MyMath::VecAdd(sphereCenter, push);
				m_SphereCollision->SetWorldPos(sphereCenter);
			}
		}
	}
}