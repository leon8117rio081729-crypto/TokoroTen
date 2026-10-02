#include "GimmickBase.h"
#include "../../../Inventory/Inventory.h"
#include "../../../Item/Tool/ToolItem.h"
#include "../../../Player/Player.h"
#include "../../../Color/Color.h"
#include "../../../Player/PlayerManager.h"
#include "../../../Collision/CollisionManager.h"
#include "Ladder/LadderGimmick.h"
#include "../../../Input/Input.h"


GimmickBase::GimmickBase()
    : m_Handle (0) 
    , m_UsedHandle (0) 
    , m_UseRadius(5.0f)
    , m_Active (true) 
    , m_IsUsed (false) 
    , m_Uiflag (false) 
    , m_CanLadder (false) 
    , m_UiCanLadder (false) 
    , m_DoorGmimickFlag (false) 
    , m_CollisionDisabled(false)
    , m_RequiredTool (ToolType::NONE)  // ★ デフォルト設定（JSONでも上書きされる）   
    , m_RequiredTool2 (ToolType::NONE)   // ★ デフォルト設定（JSONでも上書きされる）   
    , m_Pos (VGet(0.0f, 0.0f, 0.0f))     // 初期位置
{
}

GimmickBase::~GimmickBase()
{
	Fin();
}

void GimmickBase::Step()
{
    if (!m_Active) return;

    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    VECTOR diff = VSub(player->GetPos(), m_Pos);
    float dist = VSize(diff);

    // 範囲内ならUIを出す
    if (dist < DETECT_RANGE)
    {
        m_Uiflag = true;

        // 道具を持ってるか？
        bool hasTool = CheckToolEffect(player, m_RequiredTool,m_RequiredTool2);

        // UIの内容を決める
        if (hasTool)
        {
            // 「Xボタンで使用」などのインタラクトUI
            // Xボタンを押したら発動するようにする
            if (player->IsInteractPressed() && dist < INTERACT_RANGE)
            {
                OnPlayerInteract(player);
				player->GetInventory()->RemoveToolItem(); // 道具を消費               
            }
        }

        if (m_DoorGmimickFlag)
        {
            if (player->IsInteractPressed() && dist < INTERACT_RANGE)
            {
                OnPlayerInteract(player);             
            }
        }
    }
    else
    {
        m_Uiflag = false; // 範囲外ならUI消す
        m_UiCanLadder = false;
    }


    // --- はしごが使える状態（設置後） ---
    if (m_IsUsed)
    {
        m_CanLadder = true;
    }
    // --- はしごに登っていない場合 ---
    if (!player->IsOnLadder())
    {
        // UI表示（近づいたら）
        if (m_CanLadder && dist < DETECT_RANGE)
        {
            m_UiCanLadder = true;

            // インタラクトで登る
            if (player->IsInteractPressed() && dist < INTERACT_RANGE)
            {
                LadderGimmick* ladder = dynamic_cast<LadderGimmick*>(this);
                if (ladder)
                {
                    ladder->LadderAction(player);
                }
            }
        }
        else
        {
            m_UiCanLadder = false;
        }
    }
}

void GimmickBase::Update()
{
}

void GimmickBase::Draw()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (m_IsUsed)
    {
        // 使用後モデル
        MV1SetPosition(m_UsedHandle, m_Pos);
        MV1DrawModel(m_UsedHandle);
    }
    else
    {
        // 通常モデル
        MV1SetPosition(m_Handle, m_Pos);
        MV1DrawModel(m_Handle);
    }

    if (!m_Active) return;
    {
        if (m_Uiflag && !m_IsUsed)
        {
            bool hasTool = CheckToolEffect(player, m_RequiredTool, m_RequiredTool2);

            if(m_DoorGmimickFlag == true)
            {
                // ドアギミック用メッセージ
                DrawFormatString(700, 600, YELLOW, " ドアを開ける (X)");
            }
			

            else if (hasTool)
            {
                // 所持ツールの名前を取得
                ToolItem* currentTool = player->GetInventory()->GetCurrentToolItem();
                if (currentTool)
                {
                    auto currentType = currentTool->GetToolType();
                    std::string toolName = GetToolName(currentType);

                    DrawFormatString(700, 600, YELLOW, " %s を使う (X)", toolName.c_str());
                }
            }
            else
            {
                // 必要ツールがないとき
                if (m_RequiredTool2 == ToolType::NONE)
                {
                    DrawFormatString(700, 600, GetColor(255, 100, 100),
                        " %s が必要です", GetToolName(m_RequiredTool).c_str());
                }
                else
                {
                    DrawFormatString(700, 600, GetColor(255, 100, 100),
                        "%s または %s が必要です",
                        GetToolName(m_RequiredTool).c_str(),
                        GetToolName(m_RequiredTool2).c_str());
                }
            }
        }

        if (m_IsUsed) // はしごが使用可能状態になったあと
        {
            VECTOR diff = VSub(player->GetPos(), m_Pos);
            float dist = VSize(diff);

            if (player->IsOnLadder())
            {
                // ★ はしごに登っている間は常に表示
                DrawFormatString(925, 400, YELLOW, "梯子を途中で降りる (B)");
            }
            else
            {
                // ★ はしごの近くにいるときだけ表示
                if (dist < INTERACT_RANGE)
                {
                    DrawFormatString(700, 600, YELLOW, "梯子を上る (X)");
                }
            }
        }
    }

#ifdef _DEBUG
	// デバッグ：球で反応範囲確認
	DrawSphere3D(m_Pos, DETECT_RANGE, 16, GetColor(100, 100, 255), GetColor(100, 100, 255), FALSE);
	// デバッグ：球でインタラクト範囲確認
	DrawSphere3D(m_Pos, INTERACT_RANGE, 16, GetColor(255, 100, 100), GetColor(255, 100, 100), FALSE);
	// デバッグ：AABBコリジョン表示
    for (auto& data : m_CollisionAABBs)
    {
        VECTOR min = VSub(VAdd(m_Pos, data.localOffset), data.halfSize);
        VECTOR max = VAdd(VAdd(m_Pos, data.localOffset), data.halfSize);
        DrawCube3D(min, max, GetColor(0, 255, 255),GetColor(0, 255, 255), TRUE);
    }
#endif // _DEBUG
}

void GimmickBase::Fin()
{
    if (m_Handle > 0)
    {
        MV1DeleteModel(m_Handle);
        MV1DeleteModel(m_UsedHandle);
        m_Handle = -1;
		m_UsedHandle = -1;
    }
}

// プレイヤーが何の道具を持っているかチェック
bool GimmickBase::CheckToolEffect(Player* player, ToolType requiredTool, ToolType requiredTool2)
{
    ToolItem* currentTool = player->GetInventory()->GetCurrentToolItem();
    if (!currentTool) return false;

    ToolType type = currentTool->GetToolType();

    return (type == requiredTool || type == requiredTool2);
}

// 基本属性をコピーするヘルパー関数
void GimmickBase::CopyBaseAttributes(GimmickBase* clone) const
{
    clone->m_Pos = m_Pos;
    clone->m_Active = m_Active;
    clone->m_RequiredTool = m_RequiredTool;
	clone->m_RequiredTool2 = m_RequiredTool2;
}

void GimmickBase::SetupCollisionAABB(const GimmickParam& param)
{
    m_CollisionAABBs.clear();

    auto aabb = std::make_unique<CollisionAABB>();
    aabb->SetTargetPos(&m_Pos);
    aabb->SetEnable(true);

    // JSONの設定からAABBの大きさとオフセットを作る
    VECTOR halfSize = VGet(param.aabbHalfX, param.aabbHalfY, param.aabbHalfZ);
    VECTOR localPos = VGet(param.aabbOffsetX, param.aabbOffsetY, param.aabbOffsetZ);

    aabb->SetLocalPos(localPos);
    aabb->SetSize(VScale(halfSize, 2.0f)); // サイズは全幅指定

    GimmickCollisionData data;
    data.aabb = std::move(aabb);
    data.localOffset = localPos;
    data.halfSize = halfSize;

    m_CollisionAABBs.push_back(std::move(data));
}
