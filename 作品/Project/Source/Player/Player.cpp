#include "Player.h"
#include "../Input//Input.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionOBB.h"
#include "../Collision/CollisionSphere.h"
#include "../Object/StageObject/StageObject.h"
#include "../DeliveryBox/DeliveryBox.h"
#include "../Scene/SceneManager.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../UI/Message/MessageManager.h"
#include "../Animation/AnimationManager.h"
#include "../Object/StageObject/StageObjectManager.h"
//#include "../PlayerShot/PlayerNormalShot.h"
#include "../PlayerShot/PlayerShotManager.h"
#include "../Item/Medicine/MedicineItem.h"
#include "../Item/Medicine/Effect/Heal.h"
#include "../Item/Medicine/Effect/Hp_Up.h"
#include "../Item/Medicine/Effect/Stamina_Up.h"
#include "../Item/ItemBase.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"
#include "../Color/Color.h"
#include "../Object/Gimmick/Tool/GimmickBase.h"

Player* Player::m_Instance = nullptr;

// コンストラクタ
Player::Player()
// コンストラクタではメンバ変数を0初期化するくらい
// ややこしい処理はしないこと
	: m_StageLevel (0)  // ステージレベル
	, m_Handle (0) // 3Dモデルのハンドル
	, m_LifeHandle (0) // プレイヤーのライフ画像のハンドル
	, m_FlashingCount (0)  // 点滅カウント
	, m_WasVisible (true)  // 前フレームの表示状態
	, m_Hp (0)  // プレイヤーのHP
	, m_Hp_MAX (0)  // プレイヤーのHP最大値
	, m_Stamina (0)  // スタミナ
	, m_Stamina_MAX (0)  // スタミナ最大値
	, m_Speed(0) // プレイヤーの移動速度
	, m_Speed_MAX(0) // プレイヤーの移動速度最大値
	, m_StaminaTimer (0)  // スタミナのタイマー
	, m_Enemy (0)  // 倒した敵の数
	, m_Score (0)  // スコア
	, m_Quota (0)  // ノルマ
	, m_DamageInterval (0)  // ダメージを受ける間隔
	, m_ShotDamageInterval (0)  // ショットダメージを受ける間隔
	, m_AnimTime (0.0f)  // アニメーションの時間
	, m_MoveFlag (false)  // 移動フラグ
	, m_JumpingFlag (false)  // ジャンプフラグ
	, m_DashFlag (false)  // ダッシュフラグ
	, m_AttackFlag (false)  // 攻撃フラグ
	, m_AttackEfectFlag(false)  // 攻撃エフェクトフラグ
	, m_ShotFlag(false)  // ショットフラグ
	, m_ShotEfectFlag(false)  // ショットエフェクトフラグ
	, m_OnLadder (false)  // はしごに乗っているかどうか
	, m_GoalActive (false)  // ゴール条件を満たしているかどうか
	, m_CanGoalSEPlayed (false)  // ゴール可能SEが再生されたかどうか
	, m_Inventory (Inventory::GetInstance())  // インベントリ取得
	, m_HitThisAnim (false)  // 今回の攻撃アニメでヒット済みか
	, m_InputMove (VGet(0.0f, 0.0f, 0.0f)) 
	, m_StatsFont (-1) 
	, m_State (PlayerState::NORMAL) 
	  // 座標、回転、スケール、移動量、前フレームの座標と当たり判定を初期化
	, m_Pos (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Rot (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Scale (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Move (VGet(0.0, 0.0f, 0.0f)) 
	, m_PrevPos (VGet(0.0, 0.0f, 0.0f)) 
	, m_AABB (nullptr) 
	, m_AttackAABB (nullptr) 
	, m_SphereCollision (nullptr) 
{	
}

// デストラクタ
Player::~Player()
{
	// 終了処理を呼べば削除した時の後始末忘れを防げる
	Fin();
}

// 初期化
void Player::Init()
{
}

// ロード
void Player::Load()
{
	// ステータス用フォントハンドルを取得
	m_StatsFont = MessageManager::GetInstance()->GetFontHandle(FontId::UI_PLAYER_STATS);

	// 3Dモデルをロードする
	m_Handle = MV1LoadModel("Data/Player/Human.x");
	// プレイヤーのライフ画像をロードする
	m_LifeHandle = LoadGraph("Data/Player/Life.png");

	// アニメ名とインデックスを対応付けて登録
	std::unordered_map<std::string, int> playerAnim = 
	{
		{"Attack",0},
		{"Idle",  1},
		{"Jump",  2},
		{"Walk",  3}
	};
	// アニメーションを登録
	AnimationManager::GetInstance()->RegisterAnimations(m_Handle, playerAnim);
}

// 開始
void Player::Start()
{
	m_StageLevel = 1; // ステージレベルを1に設定
	m_Quota = QUOTA; // ノルマを設定
	m_State = PlayerState::START; // ゲーム開始フラグを立てる
	// モデルの座標、回転、スケール値を格納する変数
	m_Pos = VGet(0.0f, 2.0f, -65.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);
	m_Scale = VGet(1.0f, 1.0f, 1.0f);
	// 移動量を初期化
	m_Move = VGet(0.0, 0.0f, 0.0f);
	//プレイヤーのHP
	m_Hp = HP; // プレイヤーのHP初期値
	m_Hp_MAX = HP_MAX; // プレイヤーのHP最大値
	m_Stamina = STAMINA_MAX; // スタミナ初期値
	m_Stamina_MAX = STAMINA_MAX; // スタミナ最大値
	m_AnimTime = ANIM_TIME;
	m_DamageInterval = DAMAGE_TIME; // ダメージを受ける間隔
	// AABBの当たり判定を設定
	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	m_AABB->SetTargetPos(&m_Pos);
	m_AABB->SetLocalPos(VGet(0.0f, 2.0f, 0.0f));
	m_AABB->SetSize(VGet(2.5f, 5.0f, 2.5f));
	// 攻撃用のAABBの当たり判定を設定
	m_AttackAABB = CollisionManager::GetInstance()->CreateAABB();
	m_AttackAABB->SetTargetPos(&m_Pos);
	m_AttackAABB->SetLocalPos(VGet(0.0f, 2.0f, 4.0f)); 
	m_AttackAABB->SetSize(VGet(4.0f, 3.0f, 4.0f));
	m_AttackAABB->SetEnable(false);                   // 最初は無効化
	// 球の当たり判定を設定
	m_SphereCollision = CollisionManager::GetInstance()->CreateSphere();
	m_SphereCollision->SetTargetPos(&m_Pos);
	m_SphereCollision->SetLocalPos(VGet(0.0f, 2.0f, 0.0f));
	m_SphereCollision->SetRadius(2.0f);

}

// ステップ
void Player::Step()
{
	// 移動量は毎フレームリセット
	m_Move = VGet(0.0, m_Move.y, 0.0f);
	m_MoveFlag = false; // 移動フラグをリセット
	m_Move.y -= GRAVITY; // 落下加速

	//移動
	if (SceneManager::GetInstance()->m_IsPaused == false)
	{
		VECTOR leftStick = Input::GetLeftStick();   // 左スティック = 移動
		VECTOR rightStick = Input::GetRightStick(); // 右スティック = 視点

		// 移動処理
		if (MyMath::VecLong(leftStick)!= 0.0f)
		{
			leftStick.z = leftStick.y;
			leftStick.y = 0.0f;
		    if (m_DashFlag)
			{
				m_Move = MyMath::VecAdd(m_Move, MyMath::VecScale(leftStick, (MOVE_SPEED * 3)));
			}
			else
			{
				m_Move = MyMath::VecAdd(m_Move, MyMath::VecScale(leftStick, MOVE_SPEED));
			}
			m_MoveFlag = true;

			// ★ プレイヤーの向きを変える処理（倒した方向を正面にする）
			CameraBase* camera = CameraManager::GetInstance()->GetCamera(CAMERA);
			VECTOR cameraRot = camera->GetRot();
			MATRIX rotY = MyMath::MatRotationYaw(cameraRot.y);
			m_Move = MyMath::MatTransform(rotY, m_Move);
			VECTOR dir = MyMath::VecNormalize(m_Move);
			m_Rot.y = atan2f(dir.x, dir.z);
		}
		
		// ジャンプ（Aボタン）
		if (!m_JumpingFlag && Input::IsTriggerPadButton(PAD_INPUT_A))
		{
			m_Move.y = JUMP_POWER;// ジャンプの初速を与える
			m_JumpingFlag = true;// ジャンプフラグを立てる
			// ジャンプアニメーション再生
			AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Jump", false);
		}

		// 攻撃(Bボタン)
		if (!m_AttackFlag && Input::IsTriggerPadButton(PAD_INPUT_B))
		{
			m_AttackFlag = true; // 攻撃フラグを立てる
			m_AttackEfectFlag = true; // 攻撃エフェクトフラグを立てる
			// 攻撃アニメーション再生
			AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Attack", false);
		}

		// 遠距離攻撃(R2)
		if (Input::IsTriggerRT())
		{
			// 遠距離攻撃フラグを立てる
			m_ShotFlag = true;
			m_ShotEfectFlag = true;
		}
	}

	// ダッシュ（Lスティック押込み）
	if (Input::IsTriggerPadButton(PAD_INPUT_START))
	{
		// ダッシュフラグを切り替える
		if (!m_DashFlag)
		{
			SetDashFlag(true);
		}
		else
		{
			SetDashFlag(false);
			m_StaminaTimer = 0;// スタミナタイマーをリセット
		}
	}

	//HPが0になったら死亡
	if (m_Hp <= 0)
	{
		//死亡処理
		PlayerDead();
	}
}

// 更新
void Player::Update()
{
	m_Move.y -= GRAVITY; // 落下加速	
	m_DamageInterval--;  // 無敵時間

	// ステージ全AABBの配列をもらう
	auto stage = StageObjectManager::GetInstance()->GetAllCollisionAABBs();
	auto stageOBB = StageObjectManager::GetInstance()->GetAllCollisionOBBs();

	// ★ 攻撃判定のON/OFFをフレームで制御
	if (m_AttackFlag)
	{
		float frame = AnimationManager::GetInstance()->GetCurrentFrame(m_Handle);

		// 例：5～15フレームだけ攻撃判定ON
		if (frame >= 5.0f && frame <= 15.0f)
		{
			// プレイヤーの向きに攻撃AABBを回転させる
			MATRIX rotY = MyMath::MatRotationYaw(m_Rot.y);
			VECTOR rotatedPos = MyMath::MatTransform(rotY, VGet(0.0f, 2.0f, 3.0f)); // ローカルオフセット
			m_AttackAABB->SetLocalPos(rotatedPos);
			m_AttackAABB->SetEnable(true);  // 有効化
			if (m_AttackEfectFlag)
			{
				// 攻撃エフェクトを再生する
				EffectManager::GetInstance()->PlayEffect(EFFECT_PLAYER_ATTACK, VAdd(m_Pos, rotatedPos));// エフェクト
				m_AttackEfectFlag = false;
			}
			
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

	// 遠距離攻撃
	if (m_ShotFlag)
	{
		m_ShotFlag = false;
		m_ShotEfectFlag = false;
		VECTOR dir;

		dir.x = sinf(m_Rot.y);
		dir.y = 0.0f;
		dir.z = cosf(m_Rot.y);

		dir = VNorm(dir);
		// 弾を発射する
		PlayerShotManager::GetInstance()->FireShot(m_Pos, dir, PlayerShotType::PLAYER_NORMAL, 0.6f, 80);
		SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_SHOT);// 弾のSEを再生
	}

	//スタミナ管理
	if (!m_DashFlag)
	{
		m_StaminaTimer++; // スタミナタイマーをカウントアップ
		if (m_StaminaTimer >= 4) // 4フレームに1回だけスタミナを回復
		{
			m_Stamina++; // スタミナ回復
			m_StaminaTimer = 0; // スタミナタイマーをリセット
		}
		if (m_Stamina >= m_Stamina_MAX)
		{
			m_Stamina = m_Stamina_MAX; // スタミナが最大値を超えないようにする
		}
	}
	else
	{
		// ダッシュ移動中はスタミナを減らす
		if (m_MoveFlag)
		{
			m_StaminaTimer++; // スタミナタイマーをカウントアップ
			if (m_StaminaTimer >= 5)  // 5フレームに1回だけ減らす
			{
				m_Stamina--;// ダッシュ中はスタミナ消費
				m_StaminaTimer = 0;// スタミナタイマーをリセット
			}
		}
		// スタミナが0になったらダッシュを無効化
		if (m_Stamina <= 0)
		{
			m_Stamina = 0; // スタミナがマイナスにならないようにする
			SetDashFlag(false); // ダッシュフラグをオフ
		}
	}

	// 回転値のキャップ処理(0～2πの値にする)
	if (m_Rot.y < 0.0f)
	{
		m_Rot.y += DX_TWO_PI_F;
	}
	else if (m_Rot.y > DX_TWO_PI_F)
	{
		m_Rot.y -= DX_TWO_PI_F;
	}

	//アニメーション終了チェック
	if (AnimationManager::GetInstance()->CheckAnimationFinish(m_Handle))
	{
		m_AttackFlag = false; // 攻撃フラグを下ろす
	}

	// 攻撃アニメーション終了チェック
	if (!m_AttackFlag)
	{
		// ジャンプ時間経過チェック
		if (!m_JumpingFlag)
		{
			//待機モーションと移動モーションの切り替え
			if (m_MoveFlag)
			{
				// 歩行アニメーション再生
				AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Walk", true);
			}
			else
			{
				//移動していないので待機アニメーションを再生
				AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Idle", true);
			}
		}
	}

	// ゴール条件を満たしているかチェック
	if (m_Score >= m_Quota)
	{
		m_GoalActive = true;
		if (!m_CanGoalSEPlayed)
		{
			m_CanGoalSEPlayed = true;
			SoundManager::GetInstance()->PlaySE(SE_TYPE_SCENE_CANGOAL);
			MessageManager::GetInstance()->ShowMessage(660, 170, WHITE, UI_LARGE, "ゴール出現！", 3000);
		}
	}
	else
	{
		m_GoalActive = false;
	}

	//メッセージ管理更新
	if (m_State == PlayerState::START)
	{
		MessageManager::GetInstance()->ShowMessage(460, 170, WHITE, UI_LARGE, "お宝を集めて納品しよう！", 5000);
		m_State = PlayerState::NORMAL;
	}


	//Y軸の移動制限
	if (m_Pos.y > PLAYER_POS_Y_MAX)
	{
		m_Pos.y = PLAYER_POS_Y_MAX;
	}
	if (m_Pos.y < GROUND_Y)
	{
		m_Pos.y = GROUND_Y;
	}
	//Z軸の移動制限
	if (m_Pos.z > PLAYER_POS_Z_MAX)
	{
		m_Pos.z = PLAYER_POS_Z_MAX;
	}
	if (m_Pos.z < PLAYER_POS_Z_MIN)
	{
		m_Pos.z = PLAYER_POS_Z_MIN;
	}

	// 梯子に乗っている場合の処理
	if (m_OnLadder)
	{
		LadderMove();
		MV1SetPosition(m_Handle, m_Pos);
		MV1SetRotationXYZ(m_Handle, m_Rot);
		MV1SetScale(m_Handle, m_Scale);
		return; // 梯子中は他の処理スキップ
	}
	
	// クリア条件を満たしたらクリアシーンへ移行
	if (m_StageLevel >= 3)
	{
		SoundManager::GetInstance()->StopBGM(BGM_TYPE_PLAY);
		MessageManager::GetInstance()->ClearMessage();
		SceneManager::GetInstance()->ChangeScene(CLEAR);// CLEAR シーンへ移行
	}

	// 移動前の座標を記録
	m_PrevPos = m_Pos;

	// 移動量を反映
	m_Pos = MyMath::VecAdd(m_Pos, m_Move);

	// 3Dモデルの座標を設定する
	MV1SetPosition(m_Handle, m_Pos);
	// 3Dモデルの回転値を設定する
	MV1SetRotationXYZ(m_Handle, m_Rot);
	// 3Dモデルのスケールを設定する
	MV1SetScale(m_Handle, m_Scale);
	//アニメーションの更新
	AnimationManager::GetInstance()->UpdateAnimation(m_Handle);
}

// 描画
void Player::Draw()
{
	// ダメージ状態のときは点滅させる
	if (m_State == PlayerState::DAMAGE)
	{
		int time = GetNowCount();
		bool isVisible = ((time / 100) % 2 == 0);

		// 表示状態が切り替わった瞬間だけカウント
		if (isVisible != m_WasVisible)
		{
			m_FlashingCount++;
			m_WasVisible = isVisible;
		}
		// 3Dモデルを描画する
		if (isVisible)
		{
			MV1DrawModel(m_Handle);
		}

		// 表示↔非表示 を10回切り替えたら終了（5回点滅）
		if (m_FlashingCount >= 10)
		{
			m_State = PlayerState::NORMAL;
			m_FlashingCount = 0;
			m_WasVisible = true;
			m_DamageInterval = DAMAGE_TIME;
		}
	}
	else
	{
		// 3Dモデルを描画する
		MV1DrawModel(m_Handle);
	}

#ifdef _DEBUG
	// 座標を描画する
	DrawFormatString(0, 0, WHITE, "座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);
#endif // _DEBUG	
}

void Player::DrawUI()
{
	// 操作説明を描画する
	DrawFormatStringToHandle(1170, 0, WHITE, m_StatsFont, "Lスティック：移動操作");
	DrawFormatStringToHandle(1170, 30, WHITE, m_StatsFont, "Rスティック：視点操作");
	DrawFormatStringToHandle(1170, 60, WHITE, m_StatsFont, "L押し込み：ダッシュ");
	DrawFormatStringToHandle(1170, 90, WHITE, m_StatsFont, "A：ジャンプ");
	DrawFormatStringToHandle(1170, 120, WHITE, m_StatsFont, "B：パンチ");
	DrawFormatStringToHandle(1170, 150, WHITE, m_StatsFont, "R2：ショット");
	DrawFormatStringToHandle(1170, 180, WHITE, m_StatsFont, "START：ポーズ画面");
	// スコア数を描画する
	DrawFormatStringToHandle(20, 50, RED, m_StatsFont, "スコア：%d / ノルマ：%d", m_Score, m_Quota);
	// プレイヤーのライフを描画する
	DrawFormatStringToHandle(20, 80, DARKLIME, m_StatsFont, "HP：%d/%d", m_Hp, m_Hp_MAX);
	// スタミナを描画する
	DrawFormatStringToHandle(20, 110, GOLD, m_StatsFont,"スタミナ：%d/%d",m_Stamina, m_Stamina_MAX);
	if (m_DashFlag)
	{
		DrawFormatStringToHandle(20, 140, GOLD, m_StatsFont, "Dash:ON");
	}
	DrawFormatStringToHandle(20, 170, WHITE, m_StatsFont, "ステージレベル：%d", m_StageLevel);
}

// 終了
void Player::Fin()
{
	// モデルをメモリから削除
	MV1DeleteModel(m_Handle);
}

void Player::CheckHitStageObjects(const std::vector<StageObject*>& objects)
{
	for (auto obj : objects)
	{
		if (!obj) continue;
		CollisionAABB* objAABB = obj->GetAABB();
		CollisionOBB* objOBB  = obj->GetOBB();

		// AABB vs AABB
		if (objAABB && m_AABB && m_AABB->CheckAABB(objAABB))
		{
			VECTOR objMin = objAABB->GetMin();
			VECTOR objMax = objAABB->GetMax();
			VECTOR objCenter = objAABB->GetCenter();
			VECTOR playerMin = m_AABB->GetMin();
			VECTOR playerMax = m_AABB->GetMax();
			VECTOR playerCenter = m_AABB->GetCenter();
			float overlapX = min(playerMax.x, objMax.x) - max(playerMin.x, objMin.x);
			float overlapY = min(playerMax.y, objMax.y) - max(playerMin.y, objMin.y);
			float overlapZ = min(playerMax.z, objMax.z) - max(playerMin.z, objMin.z);

			float minOverlap = overlapX;
			VECTOR push = VGet((playerCenter.x < objCenter.x ? -1.0f : 1.0f), 0.0f, 0.0f);

			if (overlapY < minOverlap)
			{
				minOverlap = overlapY;
				push = VGet(0.0f, (playerCenter.y < objCenter.y ? -1.0f : 1.0f), 0.0f);
				m_Move.y = 0.0f; // Y方向の移動量をリセット
				m_JumpingFlag = false; // ジャンプフラグを下ろす
			}

			if (overlapZ < minOverlap)
			{
				minOverlap = overlapZ;
				push = VGet(0.0f, 0.0f, (playerCenter.z < objCenter.z ? -1.0f : 1.0f));
			}

			push = MyMath::VecScale(push, minOverlap);

			m_Pos = MyMath::VecAdd(m_Pos, push);
		}
		// OBB vs AABB (obj の OBB とプレイヤーの AABB を比較)
		if (objOBB && m_AABB && objOBB->CheckAABB(m_AABB))
		{
			m_Pos.x = m_PrevPos.x;
			m_Pos.z = m_PrevPos.z;
		}
	}
}

void Player::CheckHitGimmickObjects(const std::vector<GimmickBase*>& gimmicks)
{
	if (gimmicks.empty()) return;
	for (auto gimmick : gimmicks)
	{
		// 無効 or 使用済みのギミックは当たり判定を無視する
		if (!gimmick) continue;
		if (!gimmick->m_Active || gimmick->m_IsUsed) continue;
		const auto& datas = gimmick->GetCollisionAABBData();
		for (const auto& data : datas)
		{
			CollisionAABB* gimmickAABB = data.aabb.get();
			if (!gimmickAABB) continue;
			// ギミックの AABB をワールド位置に合わせる
			gimmickAABB->SetWorldPos(VAdd(gimmick->GetPos(), data.localOffset));
			if (m_AABB->CheckAABB(gimmickAABB))
			{

				VECTOR gimmickMin = gimmickAABB->GetMin();
				VECTOR gimmickMax = gimmickAABB->GetMax();
				VECTOR gimmickCenter = gimmickAABB->GetCenter();
				VECTOR playerMin = m_AABB->GetMin();
				VECTOR playerMax = m_AABB->GetMax();
				VECTOR playerCenter = m_AABB->GetCenter();
				float overlapX = min(playerMax.x, gimmickMax.x) - max(playerMin.x, gimmickMin.x);
				float overlapY = min(playerMax.y, gimmickMax.y) - max(playerMin.y, gimmickMin.y);
				float overlapZ = min(playerMax.z, gimmickMax.z) - max(playerMin.z, gimmickMin.z);

				float minOverlap = overlapX;
				VECTOR push = VGet((playerCenter.x < gimmickCenter.x ? -1.0f : 1.0f), 0.0f, 0.0f);

				if (overlapY < minOverlap)
				{
					minOverlap = overlapY;
					push = VGet(0.0f, (playerCenter.y < gimmickCenter.y ? -1.0f : 1.0f), 0.0f);
				}

				if (overlapZ < minOverlap)
				{
					minOverlap = overlapZ;
					push = VGet(0.0f, 0.0f, (playerCenter.z < gimmickCenter.z ? -1.0f : 1.0f));
				}

				push = MyMath::VecScale(push,minOverlap);

				m_Pos = MyMath::VecAdd(m_Pos, push);
			}
		}
	}
}

void Player::CheckHitDeliveryBox(const std::vector<DeliveryBox*>& boxes)
{
	if (boxes.empty()) return;

	for (auto box : boxes)
	{
		if (!box) continue;
		CollisionAABB* boxAABB = box->GetAABB();

		// AABB vs AABB
		if (boxAABB && m_AABB && m_AABB->CheckAABB(boxAABB))
		{
			VECTOR boxMin = boxAABB->GetMin();
			VECTOR boxMax = boxAABB->GetMax();
			VECTOR boxCenter = boxAABB->GetCenter();
			VECTOR playerMin = m_AABB->GetMin();
			VECTOR playerMax = m_AABB->GetMax();
			VECTOR playerCenter = m_AABB->GetCenter();
			float overlapX = min(playerMax.x, boxMax.x) - max(playerMin.x, boxMin.x);
			float overlapY = min(playerMax.y, boxMax.y) - max(playerMin.y, boxMin.y);
			float overlapZ = min(playerMax.z, boxMax.z) - max(playerMin.z, boxMin.z);

			float minOverlap = overlapX;
			VECTOR push = VGet((playerCenter.x < boxCenter.x ? -1.0f : 1.0f), 0.0f, 0.0f);

			if (overlapY < minOverlap)
			{
				minOverlap = overlapY;
				push = VGet(0.0f, (playerCenter.y < boxCenter.y ? -1.0f : 1.0f), 0.0f);
				m_Move.y = 0.0f; // Y方向の移動量をリセット
				m_JumpingFlag = false; // ジャンプフラグを下ろす
			}

			if (overlapZ < minOverlap)
			{
				minOverlap = overlapZ;
				push = VGet(0.0f, 0.0f, (playerCenter.z < boxCenter.z ? -1.0f : 1.0f));
			}

			push = MyMath::VecScale(push, minOverlap);

			m_Pos = MyMath::VecAdd(m_Pos, push);

		}
	}
}

void Player::StopMove()
{
	// ★横方向の動きだけ止めて落下は続ける
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;
}

void Player::LadderMove()
{
	// 梯子中は重力を無効化
	m_Move.y = 0.0f;

	VECTOR stick = Input::GetLeftStick();

	// 上方向（スティック上）
	if (stick.y > 0.3f)
	{
		m_Move.y = 0.15f; // 上昇
	}
	// 下方向（スティック下）
	else if (stick.y < -0.3f)
	{
		m_Move.y = -0.15f; // 降下
	}
	else
	{
		m_Move.y = 0.0f;
	}

	// 梯子中の移動は上下だけ
	m_Move.x = 0.0f;
	m_Move.z = 0.0f;

	// 梯子アニメーション再生
	AnimationManager::GetInstance()->PlayAnimation(m_Handle, "Walk", true);
	
	// Y移動を反映
	m_Pos = MyMath::VecAdd(m_Pos, m_Move);

	// 梯子から降りる（Bボタン)か梯子の高さを超えたら
	if (Input::IsTriggerPadButton(PAD_INPUT_B)||m_Pos.y > 16.0f)
	{
		m_OnLadder = false;
		// 梯子の位置にプレイヤーを合わせる
		VECTOR p = GetPos();
		p.z = m_Pos.z + 2.0f;
		SetPos(p);
		m_MoveFlag = false;
		m_AttackFlag = false;
	}
}

void Player::HitEnemy(int value)
{	
	// 点滅（DAMAGE）中は無敵
	if (m_State == PlayerState::DAMAGE)
	{
		return;
	}
	//ダメージを受ける
	if (m_DamageInterval <= 0)
	{
		m_State = PlayerState::DAMAGE;
		SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_DAMAGE);
		m_Hp -= value;
		m_DamageInterval = DAMAGE_TIME;
	}
}

void Player::HitEnemyShot()
{
	// 点滅（DAMAGE）中は無敵
	if (m_State == PlayerState::DAMAGE)
	{
		return;
	}

	//無敵時間(弾から)
	m_ShotDamageInterval--;

	//ダメージを受ける
	if (m_ShotDamageInterval <= 0)
	{
		m_State = PlayerState::DAMAGE;
		SoundManager::GetInstance()->PlaySE(SE_TYPE_PLAYER_DAMAGE);
		m_Hp -= 2;
		m_ShotDamageInterval = SHOT_DAMAGE_TIME;
	}
}

void Player::HitGoal()
{
	MessageManager::GetInstance()->ShowMessage(460, 170, WHITE, UI_LARGE, "ステージレベルUP！", 3000);
	m_GoalActive = false; // ゴール条件をリセット
	m_CanGoalSEPlayed = false; // ゴール可能SE再生フラグをリセット
	m_StageLevel++; // ステージレベルを上げる
	m_Score = m_Score - m_Quota; // スコアをリセット
	m_Quota = QUOTA * m_StageLevel; // ノルマを増やす
}

void Player::PlayerDead()
{
	// プレイヤーが死亡した時の処理
	EffectManager::GetInstance()->StopAllEffects(); // 全エフェクト停止
	SoundManager::GetInstance()->PlaySEWait(SE_TYPE_PLAYER_DEAD);// 死亡SE再生
	SoundManager::GetInstance()->StopBGM(BGM_TYPE_PLAY);// BGM停止
	SceneManager::GetInstance()->ChangeScene(LOSE);// LOSE シーンへ移行(GameOver)
}

void Player::OnStomp()
{
	m_Move.y = JUMP_POWER / 1; // 敵を踏んだ時の反動
}

bool Player::IsInteractPressed() const
{
	return Input::IsTriggerPadButton(PAD_INPUT_C);
}
