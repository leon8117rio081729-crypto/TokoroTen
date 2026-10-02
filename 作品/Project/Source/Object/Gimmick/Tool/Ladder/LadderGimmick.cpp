#include "LadderGimmick.h"
#include "../../../../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../../../../MyMath/MyMath.h"
#include "../../../../Player/PlayerManager.h"
#include "../../../../Sound/SoundManager.h"
#include "../../../../Effect/EffectManager.h"
#include "../../../../Player/Player.h" 

LadderGimmick::LadderGimmick()
{
	m_IsUsed = false;
	m_RequiredTool = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_RequiredTool2 = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_CanLadder = false;
	m_DisableCollisionY = 0.0f;
}

LadderGimmick::~LadderGimmick()
{
	Fin();
}

void LadderGimmick::Init()
{
	m_DisableCollisionY = 13.0f;
	m_Active = true;
	m_IsUsed = false;
	SetupCollisionAABB(m_Params);
}

void LadderGimmick::Load()
{
	m_Handle = MV1LoadModel("Data/Object/Gimmick/Tool/Ladder/Ladder_Gimmick.x");
	MV1SetScale(m_Handle, VGet(1.0f, 1.0f, 1.0f));
	m_UsedHandle = MV1LoadModel("Data/Object/Gimmick/Tool/Ladder/Ladder_Gimmick_Used.x");
	MV1SetScale(m_UsedHandle, VGet(1.0f, 1.0f, 1.0f));
}

void LadderGimmick::Start()
{
	m_Active = true;
	m_IsUsed = false;
	m_Pos = m_Params.pos;
}

void LadderGimmick::Update()
{
	Step();
	// はしご使用後、プレイヤーの Y が閾値を超えたら当たり判定を無効化する
	if (m_IsUsed && !m_CollisionDisabled)
	{
		Player* player = PlayerManager::GetInstance()->GetPlayer();
		if (player && player->GetPos().y > m_DisableCollisionY)
		{
			// AABB を無効化
			for (auto& data : m_CollisionAABBs)
			{
				if (data.aabb)
				{
					data.aabb->SetEnable(false);
				}
			}
			m_CollisionDisabled = true;
			m_CanLadder = false;
		}
	}
}

void LadderGimmick::OnPlayerInteract(Player* player)
{
	// 効果演出
	EffectManager::GetInstance()->PlayEffect(EFFECT_LADDER_USED, m_Pos);
	SoundManager::GetInstance()->PlaySE(SE_TYPE_GIMMICK_LADDER);
	// 一度使ったら非表示
	m_IsUsed = true;
	m_CanLadder = true;
}

void LadderGimmick::LadderAction(Player* player)
{
	if (m_CanLadder && !player->IsOnLadder())
	{
		player->SetOnLadder(true);

		// プレイヤーの位置を梯子に合わせる
		VECTOR p = player->GetPos();
		p.x = m_Pos.x - 1.5f;
		p.z = m_Pos.z - 2.0f;
		player->SetPos(p);
	}
}

GimmickBase* LadderGimmick::Clone()
{
	// クローン用のオブジェクトを生成
	LadderGimmick* clone = new LadderGimmick;

	// 基本情報をコピー
	CopyBaseAttributes(clone);
	// モデルハンドルを複製
	// クローンは再ロード（確実にスケール等が適用される）
	clone->Load();

	// 同じスケール・回転・位置を再設定
	MV1SetScale(clone->m_Handle, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(clone->m_Handle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(clone->m_Handle, clone->m_Pos);
	// 使用後モデルも同様に設定
	MV1SetScale(clone->m_UsedHandle, VGet(0.1f, 0.1f, 0.1f));
	MV1SetRotationXYZ(clone->m_UsedHandle, VGet(0.0f, 0.0f, 0.0f));
	MV1SetPosition(clone->m_UsedHandle, clone->m_Pos);

	clone->m_IsUsed = m_IsUsed;
	return clone;// 出来上がったクローンを返却
}
