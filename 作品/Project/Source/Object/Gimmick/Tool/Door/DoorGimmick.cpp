#include "DoorGimmick.h"
#include "../../../../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../../../../MyMath/MyMath.h"
#include "../../../../Player/PlayerManager.h"
#include "../../../../Sound/SoundManager.h"

DoorGimmick::DoorGimmick()
{
	m_IsUsed = false;
	m_DoorGmimickFlag = false;
	m_RequiredTool = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_RequiredTool2 = ToolType::NONE; // ★ デフォルト設定（JSONでも上書きされる）
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

DoorGimmick::~DoorGimmick()
{
	Fin();
}

void DoorGimmick::Init()
{
	m_Active = true;
	m_IsUsed = false;
	m_DoorGmimickFlag = true;
	SetupCollisionAABB(m_Params);
}

void DoorGimmick::Load()
{
	m_Handle = MV1LoadModel("Data/Object/StageObject/Door.x");
	MV1SetScale(m_Handle, VGet(1.97f, 1.45f, 1.6f));
	m_UsedHandle = MV1LoadModel("Data/Object/Gimmick/Tool/Bar&Saw/Bar&Saw_Gimmick_Used.x");
	MV1SetScale(m_UsedHandle, VGet(1.97f, 1.42f, 1.6f));
}

void DoorGimmick::Start()
{
	m_Active = true;
	m_IsUsed = false;
	m_Pos = m_Params.pos;
}

void DoorGimmick::Update()
{
}

void DoorGimmick::OnPlayerInteract(Player* player)
{
    m_IsUsed = true;
    // 効果演出
	SoundManager::GetInstance()->PlaySE(SE_TYPE_GIMMICK_DOOR_OPEN);
    // 一度使ったら非表示
    m_Active = false;
}

GimmickBase* DoorGimmick::Clone()
{
	// クローン用のオブジェクトを生成
	DoorGimmick* clone = new DoorGimmick;

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
