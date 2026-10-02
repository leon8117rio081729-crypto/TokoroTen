#include "CollisionManager.h"
#include "CollisionAABB.h"
#include "CollisionSphere.h"
#include "CollisionOBB.h"
#include "../Player/PlayerManager.h"
#include "../Goal/GoalManager.h"
#include "../Player/Player.h"
#include "../Goal/Goal.h"
#include "../DeliveryBox/DeliveryBox.h"
#include "../Enemy/EnemyManager.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../EnemyShot/EnemyShotManager.h"
#include "../PlayerShot/PlayerShotManager.h"
#include "../Object/Room/Tile/Tile.h"
#include "../MyMath/MyMath.h"
#include "../Object/StageObject/StageObjectManager.h"
#include "../Object/StageObject/StageObject.h"
#include "../Object/Gimmick/Tool/GimmickBase.h"
#include "../Object/Gimmick/Tool/GimmickManager.h"

// 静的変数の初期化
CollisionManager* CollisionManager::m_Instance = nullptr;

// コンストラクタ
CollisionManager::CollisionManager()
	: m_AABB{}
	, m_Sphere{}
	, m_OBB{}
{
}

// デストラクタ
CollisionManager::~CollisionManager()
{
	// 削除時の後始末忘れを防止する
	Fin();
}

void CollisionManager::Draw()
{
#ifdef _DEBUG
	// m_AABBを先頭から末尾までまわす範囲for文
	for (auto aabb : m_AABB)
	{
		if (aabb)
		{
			aabb->Draw();
		}
	}

	// m_Sphereを先頭から末尾までまわす範囲for文
	for (auto sphere : m_Sphere)
	{
		if (sphere)
		{
			sphere->Draw();
		}
	}

	// m_OBBを先頭から末尾までまわす範囲for文
	for (auto obb : m_OBB)
	{
		if (obb)
		{
			obb->Draw();
		}
	}
#endif // _DEBUG
}

void CollisionManager::Fin()
{
	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 使用されているところだけ削除して未使用状態にする
		if (m_AABB[i])
		{
			delete m_AABB[i];
			m_AABB[i] = nullptr;
		}
		if (m_Sphere[i])
		{
			delete m_Sphere[i];
			m_Sphere[i] = nullptr;
		}
	}
}

CollisionAABB* CollisionManager::CreateAABB()
{
	CollisionAABB* result = nullptr;

	// m_AABBを先頭から末尾までまわす範囲for文
	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 未使用のAABBか
		if (!m_AABB[i])
		{
			// AABBを生成して配列に保存
			m_AABB[i] = result = new CollisionAABB;
			break;
		}
	}

	return result;
}

void CollisionManager::DeleteAABB(CollisionAABB* targetAABB)
{
	// m_AABBを先頭から末尾までまわす範囲for文
	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 参照先が一致するAABBを探す
		if (m_AABB[i] == targetAABB)
		{
			// 見つかったら削除
			delete targetAABB;
			// 未使用状態にするためnullptr
			m_AABB[i] = nullptr;
			break;
		}
	}
}

// Sphereを生成する
CollisionSphere* CollisionManager::CreateSphere()
{
	CollisionSphere* result = nullptr;

	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 未使用のSphereか
		if (!m_Sphere[i])
		{
			// Sphereを生成して配列に保存
			m_Sphere[i] = result = new CollisionSphere;
			break;
		}
	}

	return result;
}

// Sphereを削除する
void CollisionManager::DeleteSphere(CollisionSphere* targetSphere)
{
	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 参照先が一致するSphereを探す
		if (m_Sphere[i] == targetSphere)
		{
			// 見つかったら削除
			delete targetSphere;
			// 未使用状態にするためnullptr
			m_Sphere[i] = nullptr;
			break;
		}
	}
}

// OBBを生成する
CollisionOBB* CollisionManager::CreateOBB()
{
	CollisionOBB* result = nullptr;

	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 未使用のOBBか
		if (!m_OBB[i])
		{
			// OBBを生成して配列に保存
			m_OBB[i] = result = new CollisionOBB;
			break;
		}
	}

	return result;
}

// OBBを削除する
void CollisionManager::DeleteOBB(CollisionOBB* targetOBB)
{
	for (int i = 0; i < COLLISION_MAX; i++)
	{
		// 参照先が一致するOBBを探す
		if (m_OBB[i] == targetOBB)
		{
			// 見つかったら削除
			delete targetOBB;
			// 未使用状態にするためnullptr
			m_OBB[i] = nullptr;
			break;
		}
	}
}

void CollisionManager::CheckCollision()
{
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	EnemyBase* enemy = EnemyManager::GetInstance()->GetEnemies();
	EnemyManager* enemymanager = EnemyManager::GetInstance();
	auto objects_S = StageObjectManager::GetInstance()->GetStageObjects();
	auto objects_G = GimmickManager::GetInstance()->GetGimmickObjects();
	auto deliveryBoxes = DeliveryBox::GetInstance()->GetDeliveryBoxes();
	CollisionAABB* playerAABB = player->GetAABB();
	CollisionAABB* attackAABB = player->GetAttackAABB();
	// プレイヤーの球の当たり判定取得
	CollisionSphere* playerSphere = player->GetSphereCollision();
	// 敵とプレイヤーの当たり判定
	std::list<EnemyBase*> enemyList = EnemyManager::GetInstance()->GetEnemyList();
	// 敵弾とプレイヤーの球の当たり判定取得
	std::list<EnemyShotBase*> enemyShotList = EnemyShotManager::GetInstance()->GetShotList();
	// ゴールとプレイヤーの当たり判定
	Goal* goal = GoalManager::GetInstance()->GetGoal();

	// プレイヤーとギミックの当たり判定
    player->CheckHitGimmickObjects(objects_G);
	// プレイヤーとオブジェクトの当たり判定
	player->CheckHitStageObjects(objects_S);
	// プレイヤーと納品箱の当たり判定
	player->CheckHitDeliveryBox(deliveryBoxes);

	// 敵とギミック・オブジェクトの当たり判定
	for (auto enemies : enemyList)
	{
		if (!enemies) continue;
		// 敵とギミックの当たり判定
		enemies->CheckHitGimmickObjects(objects_G);
		// 敵とオブジェクトの当たり判定
		enemies->CheckHitStageObjects(objects_S);
	}

	// ゴールとプレイヤーの当たり判定
	if (CheckAABBSphereCollision(playerAABB, goal->GetSphereCollisoin()))
	{
		SoundManager::GetInstance()->PlaySE(SE_TYPE_SCENE_GOAL);
		player->HitGoal();
		enemymanager->Start();
		return;
	}

	// 敵とプレイヤーの当たり判定取得
	for (auto enemy : enemyList)
	{
		if (enemy->GetActive() && !enemy->GetIsDead())
		{
			CollisionSphere* enemySphere = enemy->GetSphereCollision();
			if (!enemySphere) continue;
			if (!playerSphere) continue;

			// プレイヤーと敵の当たり判定
			if (playerSphere->CheckSphere(enemySphere))
			{
				// プレイヤー側・敵側ともに当たり判定球のワールド座標を基準にする
				VECTOR playerPos = playerSphere->GetWorldPos();
				VECTOR enemyPos = enemySphere->GetWorldPos();
			}

			// ★ 攻撃と敵の当たり判定
			if (attackAABB && attackAABB->IsActive())
			{
				if (CollisionManager::CheckAABBSphereCollision(player->GetAttackAABB(), enemy->GetSphereCollision()))
				{
					enemy->Damage();   // 敵にダメージを与える
					player->GetAttackAABB()->SetActive(false);// プレイヤーの攻撃判定を無効化
				}
			}

			// プレイヤー弾と敵の当たり判定取得
			for(auto playerShot : PlayerShotManager::GetInstance()->GetShotList())
			{
				if (playerShot->IsActive())
				{
					CollisionSphere* playerShotSphere = playerShot->GetSphereCollision();
					if (enemySphere->CheckSphere(playerShotSphere))
					{
						SoundManager::GetInstance()->PlaySE(SE_TYPE_ENEMY_DAMAGE);
						enemy->Damage();
						playerShot->SetActive(false);
					}
				}
			}

			// 敵弾とプレイヤーの当たり判定取得
			for (auto enemyShot : enemyShotList)
			{
				if (enemyShot->IsActive())
				{
					CollisionSphere* enemyShotSphere = enemyShot->GetSphereCollision();
					if (playerSphere->CheckSphere(enemyShotSphere))
					{
						SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_DAMAGE);
						player->HitEnemyShot();
						enemyShot->SetActive(false);
					}
				}
			}
		}
	}
	// プレイヤー弾とステージオブジェクト
	for (auto shot : PlayerShotManager::GetInstance()->GetShotList())
	{
		if (!shot->IsActive()) continue;

		CollisionSphere* shotSphere = shot->GetSphereCollision();

		for (auto obj : objects_S)
		{
			CollisionAABB* objAABB = obj->GetAABB();
			if (!objAABB || !objAABB->IsActive()) continue;

			if (CheckSphereAABBCollision(shotSphere, objAABB))
			{
				shot->SetActive(false);
				break;
			}
		}
	}
	// プレイヤー弾とギミック
	for (auto shot : PlayerShotManager::GetInstance()->GetShotList())
	{
		if (!shot->IsActive()) continue;

		CollisionSphere* shotSphere = shot->GetSphereCollision();

		for (auto gimmick : objects_G)
		{
			if (!gimmick->GetActive()) continue;

			const auto& datas = gimmick->GetCollisionAABBData();

			for (const auto& data : datas)
			{
				CollisionAABB* aabb = data.aabb.get();
				if (!aabb) continue;

				aabb->SetWorldPos(
					MyMath::VecAdd(gimmick->GetPos(), data.localOffset));

				if (CheckSphereAABBCollision(shotSphere, aabb))
				{
					shot->SetActive(false);
					break;
				}
			}

			if (!shot->IsActive())
				break;
		}
	}
}

bool CollisionManager::CheckSphereCollision(CollisionSphere* s1, CollisionSphere* s2)
{
	if (!s1 || !s2) return false;
	if (!s1->IsEnable() || !s2->IsEnable()) return false;
	return s1->CheckSphere(s2);
}

bool CollisionManager::CheckSphereAABBCollision(CollisionSphere* s, CollisionAABB* aabb)
{
	return s->CheckAABB(aabb);
}

bool CollisionManager::CheckAABBCollision(CollisionAABB* aabb1, CollisionAABB* aabb2)
{
	if (!aabb1 || !aabb2) return false;
	if (!aabb1->IsActive() || !aabb2->IsActive()) return false;

	// AABB1の中心とサイズ
	VECTOR pos1 = VAdd(aabb1->GetTargetPos(), aabb1->GetLocalPos());
	VECTOR size1 = aabb1->GetSize();
	VECTOR half1 = VScale(size1, 0.5f);

	// AABB2の中心とサイズ
	VECTOR pos2 = VAdd(aabb2->GetTargetPos(), aabb2->GetLocalPos());
	VECTOR size2 = aabb2->GetSize();
	VECTOR half2 = VScale(size2, 0.5f);

	// 各軸ごとに重なっているか判定
	bool overlapX = fabsf(pos1.x - pos2.x) <= (half1.x + half2.x);
	bool overlapY = fabsf(pos1.y - pos2.y) <= (half1.y + half2.y);
	bool overlapZ = fabsf(pos1.z - pos2.z) <= (half1.z + half2.z);

	return overlapX && overlapY && overlapZ;
}


CollisionSphere* CollisionManager::CreateTempSphere(VECTOR pos, float radius)
{
	CollisionSphere* sphere = new CollisionSphere();
	// SetTargetPos はポインタ参照なので参照切れが危険 → ワールド座標として安全に保持する
	sphere->SetLocalPos(VGet(0, 0, 0));
	sphere->SetRadius(radius);

	// pos を直接ワールド座標として設定（CollisionSphere 側に SetWorldPos があればそれを使う想定）
	sphere->SetWorldPos(pos);

	return sphere;
}

// レイ判定を使ったカメラ当たり判定
bool CollisionManager::CheckCameraCollisionRay(const VECTOR& start, const VECTOR& end, VECTOR* correctedPos, float padding)
{
	if (!correctedPos) return false;

	VECTOR rd = MyMath::VecSub(end, start);

	// rd がほぼゼロなら判定不要
	if (fabsf(rd.x) < 1e-6f && fabsf(rd.y) < 1e-6f && fabsf(rd.z) < 1e-6f)
	{
		*correctedPos = end;
		return false;
	}

	float nearestT = 2.0f;
	bool hitAny = false;

	// ステージオブジェクト群に対してレイ判定
	auto stageObjects = StageObjectManager::GetInstance()->GetStageObjects();
	for (auto obj : stageObjects)
	{
		if (!obj) continue;
		CollisionAABB* objAABB = obj->GetAABB();
		if (!objAABB || !objAABB->IsActive()) continue;

		VECTOR aMin = objAABB->GetMin();
		VECTOR aMax = objAABB->GetMax();

		// レイの起点がこの AABB 内なら無視する（起点内での t=0 ヒットを防ぐ）
		if (start.x >= aMin.x && start.x <= aMax.x &&
			start.y >= aMin.y && start.y <= aMax.y &&
			start.z >= aMin.z && start.z <= aMax.z)
		{
			continue;
		}

		float t = 0.0f;
		if (CollisionAABB::RayIntersectsAABB(start, rd, aMin, aMax, &t))
		{
			if (t >= 0.0f && t <= 1.0f && t < nearestT)
			{
				nearestT = t;
				hitAny = true;
			}
		}
	}

	// ギミック群に対してレイ判定（複数AABBを持つ想定）
	auto gimmicks = GimmickManager::GetInstance()->GetGimmickObjects();
	for (auto gimmick : gimmicks)
	{
		if (!gimmick) continue;
		if (!gimmick->GetActive()) continue;

		const auto& datas = gimmick->GetCollisionAABBData();
		for (const auto& data : datas)
		{
			CollisionAABB* gaabb = data.aabb.get();
			if (!gaabb) continue;

			gaabb->SetWorldPos(MyMath::VecAdd(gimmick->GetPos(), data.localOffset));
			if (!gaabb->IsActive()) continue;

			VECTOR aMin = gaabb->GetMin();
			VECTOR aMax = gaabb->GetMax();

			// 起点が AABB 内ならスキップ
			if (start.x >= aMin.x && start.x <= aMax.x &&
				start.y >= aMin.y && start.y <= aMax.y &&
				start.z >= aMin.z && start.z <= aMax.z)
			{
				continue;
			}

			float t = 0.0f;
			if (CollisionAABB::RayIntersectsAABB(start, rd, aMin, aMax, &t))
			{
				if (t >= 0.0f && t <= 1.0f && t < nearestT)
				{
					nearestT = t;
					hitAny = true;
				}
			}
		}
	}

	if (!hitAny)
	{
		*correctedPos = end;
		return false;
	}

	// ヒット点の少し手前へ詰める（padding）
	VECTOR hitPos = MyMath::VecAdd(start, MyMath::VecScale(rd, nearestT));
	float rdLenSq = MyMath::VecSizeSq(start, end);
	if (rdLenSq > 1e-6f)
	{
		float invLen = 1.0f / sqrtf(rdLenSq);
		VECTOR dir = MyMath::VecScale(rd, invLen);
		*correctedPos = MyMath::VecAdd(hitPos, MyMath::VecScale(dir, -padding));
	}
	else
	{
		*correctedPos = hitPos;
	}

	return true;
}

// 既存の CheckCameraCollision は後方互換のため新関数を使う
bool CollisionManager::CheckCameraCollision(const CollisionSphere* camSphere, VECTOR* correctedPos)
{
	if (!camSphere || !correctedPos) return false;
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	if (!player) return false;

	VECTOR start = player->GetPos();
	start.y = 0.5f;
	VECTOR end = camSphere->GetWorldPos();

	// 標準パディング値 0.2f を使用
	return CheckCameraCollisionRay(start, end, correctedPos, 0.2f);
}

 // AABBとSphere 判定
bool CollisionManager::CheckAABBSphereCollision(CollisionAABB* aabb, CollisionSphere* sphere)
{
	VECTOR aabbCenter = VAdd(aabb->GetTargetPos(), aabb->GetLocalPos());// AABBの中心座標
	VECTOR aabbHalfSize = VScale(aabb->GetSize(), 0.5f);// AABBの半分のサイズ

	VECTOR sphereCenter = VAdd(sphere->GetTargetPos(), sphere->GetLocalPos());// 球の中心座標
	float radius = sphere->GetRadius();// 球の半径

	// AABBの各軸における最近接点を求める
	float dx = fmaxf(aabbCenter.x - aabbHalfSize.x, fminf(sphereCenter.x, aabbCenter.x + aabbHalfSize.x)) - sphereCenter.x;
	float dy = fmaxf(aabbCenter.y - aabbHalfSize.y, fminf(sphereCenter.y, aabbCenter.y + aabbHalfSize.y)) - sphereCenter.y;
	float dz = fmaxf(aabbCenter.z - aabbHalfSize.z, fminf(sphereCenter.z, aabbCenter.z + aabbHalfSize.z)) - sphereCenter.z;
	// 最近接点と球の中心との距離の二乗を計算
	float distSq = dx * dx + dy * dy + dz * dz;
	// 距離の二乗が半径の二乗以下なら当たっている
	return distSq <= radius * radius;
}

