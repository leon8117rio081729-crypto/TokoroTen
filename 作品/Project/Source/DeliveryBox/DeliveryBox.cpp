#include "DeliveryBox.h"
#include "../Player/Player.h"
#include "../Inventory/Inventory.h"
#include "../Item/Treasure/TreasureItem.h"
#include "../Input/Input.h"
#include "../Player/PlayerManager.h"
#include "../MyMath/MyMath.h"
#include "../Sound/SoundManager.h"
#include "../Color/Color.h"
#include "../Collision/CollisionAABB.h"
#include "../Collision/CollisionManager.h"

DeliveryBox* DeliveryBox::m_Instance = nullptr; // シングルトンインスタンス初期化

DeliveryBox::DeliveryBox() 
    : m_Pos (VGet(0.0f, 0.0f, 0.0f))   // 位置初期化
    , m_Size (VGet(1.0f, 1.0f, 1.0f))  // サイズ初期化
    , m_AABB (nullptr)    // AABB初期化
    , m_ModelHandle (0)   // モデルハンドル初期化
    , Uiflag (false)      // UI表示フラグ初期化
{
}

DeliveryBox::~DeliveryBox()
{
    
}

void DeliveryBox::Init()
{
	m_Pos = VGet(10.0f, 2.0f, -80.0f); // 納品箱の初期位置
}

void DeliveryBox::Load()
{
    m_ModelHandle = MV1LoadModel("Data/DeliveryBox/DeliveryBox.x"); // 納品箱のモデル
}

void DeliveryBox::Start()
{
    // AABBの当たり判定を設定
    m_AABB = CollisionManager::GetInstance()->CreateAABB();
    m_AABB->SetTargetPos(&m_Pos);
    m_AABB->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
    m_AABB->SetSize(VGet(9.0f, 5.0f, 4.5f));
    // モデルの初期位置を設定
	MV1SetPosition(m_ModelHandle, m_Pos);
	// モデルのスケールを設定（必要に応じて）
	MV1SetScale(m_ModelHandle, VGet(2.0f, 2.5f, 2.0f));
	Uiflag = false; // UI表示フラグ初期化
    m_DeliveryBoxes.clear();
    m_DeliveryBoxes.push_back(this);
}

void DeliveryBox::Update()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;
    VECTOR viewDir = MyMath::VecForwardZX(player->GetRot().y);
    VECTOR toItem = MyMath::VecSub(m_Pos, player->GetPos());
    float distSq = MyMath::VecSizeSq(m_Pos, player->GetPos());
    toItem = MyMath::VecNormalize(toItem);
    float dot = MyMath::VecDot(viewDir, toItem);
    if (distSq < m_DeliveryRadius * m_DeliveryRadius)
    {
        if (CheckDelivery(player))
        {
            if(!player->GetInventory()->IsOpenInventory())
            {
                // 納品可能なアイテムがある場合
                SetUIFlag(true);// UI表示（納品可能）
                if (Input::IsTriggerPadButton(PAD_INPUT_C))// Xボタンで納品
                {
                    // 納品処理：スコア加算
                    auto& inventory = player->GetInventory()->GetItems();
                    for (auto it = inventory.begin(); it != inventory.end();)
                    {
                        // TreasureItemのみ処理
                        if (auto treasure = dynamic_cast<TreasureItem*>(it->get()))
                        {
                            treasure->Apply(player);// スコア加算        
                            SoundManager::GetInstance()->PlaySE(SE_TYPE_ITEM_DELIVERY); // 納品音
                            player->GetInventory()->RemoveTreasureItem();// インベントリから削除
                            SetUIFlag(false); // UI非表示（納品後）
                            break;
                        }
                        ++it;
                    }
                }
            }
            else
            {
				SetUIFlag(false); // UI非表示（インベントリが開いている場合）
            }
			
		}
    }
    else
    {
        SetUIFlag(false); // UI非表示（納品不可）
    }
}

void DeliveryBox::Draw()
{
   // MV1SetPosition(m_ModelHandle, m_Pos);
    MV1DrawModel(m_ModelHandle);

    if (IsUIFlag())
    {
		// UI表示（納品可能）
		DrawFormatString(750, 600, YELLOW, "納品するには[X]を押してください");
	}

}

// 納品条件チェック
bool DeliveryBox::CheckDelivery(Player* player)
{
	const auto& items = player->GetInventory()->GetItems();// インベントリ内のアイテムを取得
    for (const auto& item : items)
    {
		if (dynamic_cast<TreasureItem*>(item.get()))// TreasureItemがあれば納品可能
        {
            return true;
        }
    }
    return false;
}
