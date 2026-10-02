#include "Goal.h"
#include "../Input//Input.h"
#include "../MyMath/MyMath.h"
#include "../Collision/CollisionManager.h"
#include "../Collision/CollisionSphere.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"
#include "../Color/Color.h"

constexpr float ROTATION_SPEED = 0.05f;
constexpr float MOVE_SPEED = 0.1f;

// コンストラクタ
Goal::Goal()
// コンストラクタではメンバ変数を0初期化するくらい
// ややこしい処理はしないこと
	: m_Handle (0) 
	, m_Pos (VGet(0.0f, 0.0f, 0.0f)) 
	, m_Rot (VGet(0.0f, 0.0f, 0.0f)) 
	, m_SphereCollision (nullptr) 
	, m_IsActive (false) 
{
}

// デストラクタ
Goal::~Goal()
{
	// 終了処理を呼べば削除した時の後始末忘れを防げる
	Fin();
}

// 初期化
void Goal::Init()
{
}

// ロード
void Goal::Load()
{
	// 3Dモデルをロードする
	m_Handle = MV1LoadModel("Data/Goal/Goal.x");
}

// 開始
void Goal::Start()
{
	// モデルの座標、回転、スケール値を格納する変数
	m_Pos = VGet(35.0f, 15.0f, 75.0f);
	m_Rot = VGet(0.0f, 0.0f, 0.0f);

	// 球の当たり判定を設定
	m_SphereCollision = CollisionManager::GetInstance()->CreateSphere();
	m_SphereCollision->SetTargetPos(&m_Pos);
	m_SphereCollision->SetLocalPos(VGet(0.0f, 1.0f, 0.0f));
	m_SphereCollision->SetRadius(1.0f);
}

// ステップ
void Goal::Step()
{
	// Y軸回転
	m_Rot.y -= ROTATION_SPEED;

	// 回転値のキャップ処理(0～2πの値にする)
	if (m_Rot.y < 0.0f)
	{
		m_Rot.y += DX_TWO_PI_F;
	}
}

// 更新
void Goal::Update()
{
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	if (player->GetGoalActive())
	{
		m_Pos = VGet(35.0f, 15.0f, 75.0f);
		// 3Dモデルの座標を設定する
		MV1SetPosition(m_Handle, m_Pos);
		// 3Dモデルの回転値を設定する
		MV1SetRotationXYZ(m_Handle, m_Rot);
	}
	else
	{
		// 3Dモデルの座標を設定する
		m_Pos = VGet(0.0f, -100.0f, 0.0f); // 見えない位置に移動
	}
}

// 描画
void Goal::Draw()
{
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	if (player->GetGoalActive())
	{
		// 3Dモデルを描画する
		MV1DrawModel(m_Handle);
	}
}

// 終了
void Goal::Fin()
{
	// モデルをメモリから削除
	MV1DeleteModel(m_Handle);
}
