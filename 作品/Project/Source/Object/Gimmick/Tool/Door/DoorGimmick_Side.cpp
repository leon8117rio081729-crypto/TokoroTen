#include "DoorGimmick_Side.h"
#include "../../../../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../../../../MyMath/MyMath.h"
#include "../../../../Player/PlayerManager.h"
#include "../../../../Sound/SoundManager.h"

DoorGimmick_Side::DoorGimmick_Side()
{
	m_IsUsed = false;
	m_DoorGmimickFlag = false;
	m_RequiredTool = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_RequiredTool2 = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

DoorGimmick_Side::~DoorGimmick_Side()
{
	Fin();
}

void DoorGimmick_Side::Init()
{
	m_Active = true;
	m_IsUsed = false;
	m_DoorGmimickFlag = true;
	SetupCollisionAABB(m_Params);
}

void DoorGimmick_Side::Load()
{
	m_Handle = MV1LoadModel("Data/Object/StageObject/Door_Side.x");
	MV1SetScale(m_Handle, VGet(2.0f, 1.45f, 2.0f));
	m_UsedHandle = MV1LoadModel("Data/Object/Gimmick/Tool/Bar&Saw/Bar&Saw_Gimmick_Used_Side.x");
	MV1SetScale(m_UsedHandle, VGet(2.0f, 1.45f, 2.0f));
}

void DoorGimmick_Side::Start()
{
	m_Active = true;
	m_IsUsed = false;
	m_Pos = m_Params.pos;
}

void DoorGimmick_Side::Update()
{
}

void DoorGimmick_Side::OnPlayerInteract(Player* player)
{
    m_IsUsed = true;

    // 効果演出
	SoundManager::GetInstance()->PlaySE(SE_TYPE_GIMMICK_DOOR_OPEN);

    // 一度使ったら非表示（または消す）
    m_Active = false;
}

GimmickBase* DoorGimmick_Side::Clone()
{
	// クローン用のオブジェクトを生成
	DoorGimmick_Side* clone = new DoorGimmick_Side;

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
