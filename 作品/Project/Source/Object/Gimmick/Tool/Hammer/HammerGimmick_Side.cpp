#include "HammerGimmick_Side.h"
#include "../../../../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../../../../MyMath/MyMath.h"
#include "../../../../Player/PlayerManager.h"
#include "../../../../Sound/SoundManager.h"
#include "../../../../Effect/EffectManager.h"

HammerGimmick_Side::HammerGimmick_Side()
{
	m_IsUsed = false;
	m_RequiredTool = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_RequiredTool2 = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

HammerGimmick_Side::~HammerGimmick_Side()
{
	Fin();
}

void HammerGimmick_Side::Init()
{
	m_Active = true;
	m_IsUsed = false;
	SetupCollisionAABB(m_Params);
}

void HammerGimmick_Side::Load()
{
	m_Handle = MV1LoadModel("Data/Object/Gimmick/Tool/Hammer/Hammer_Gimmick_Side.x");
	MV1SetScale(m_Handle, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(m_Handle, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
	m_UsedHandle = MV1LoadModel("Data/Object/Gimmick/Tool/Hammer/Hammer_Gimmick_Used_Side.x");
	MV1SetScale(m_UsedHandle, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(m_UsedHandle, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
}

void HammerGimmick_Side::Start()
{
	m_Active = true;
	m_IsUsed = false;
	m_Pos = m_Params.pos;
}

void HammerGimmick_Side::Update()
{
}

void HammerGimmick_Side::OnPlayerInteract(Player* player)
{
	// 効果演出
	EffectManager::GetInstance()->PlayEffect(EFFECT_HAMMER_USED, m_Pos);
	SoundManager::GetInstance()->PlaySE(SE_TYPE_GIMMICK_HAMMER);
    // 一度使ったら非表示
    m_Active = false;
	m_IsUsed = true;
}

GimmickBase* HammerGimmick_Side::Clone()
{
	// クローン用のオブジェクトを生成
	HammerGimmick_Side* clone = new HammerGimmick_Side;

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
