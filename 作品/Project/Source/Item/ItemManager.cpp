#include "ItemManager.h"
#include "DxLib.h"
#include "../Item/Medicine/MedicineItem.h"
#include "../Item/Medicine/Effect/Heal.h"
#include "../Item/Medicine/Effect/Hp_Up.h"
#include "../Item/Medicine/Effect/Stamina_Up.h"
#include "../Item/Treasure/TreasureItem.h"
#include "../Item/Treasure/Effect/Treasure.h"
#include "../Item/Tool/ToolItem.h"
#include "../Item/Tool/Effect/Ladder/Ladder.h"
#include "../Item/Tool/Effect/Hammer/Hammer.h"
#include "../MyMath/MyMath.h"
#include "../Player/PlayerManager.h"
#include "../Input/Input.h"
#include "../Inventory/Inventory.h"
#include "../Sound/SoundManager.h"
#include "../Item/ItemObject.h"
#include "../Player/Player.h"
#include "../Color/Color.h"

ItemManager* ItemManager::m_Instance = nullptr;

ItemManager::ItemManager()
    : m_PickupRadius (ITEM_PICKUP_RADIUS) // アイテムを拾える距離
    , m_ModelHandle (0) 
    , m_respawnTime(0.0f)
    , m_InventoryMax (false) 
{
}

ItemManager::~ItemManager()
{
    Fin();
}

void ItemManager::Init()
{
}

void ItemManager::Load() 
{
	// モデル読み込み 
    //回復薬
    ItemResource heal;
    heal.modelHandle = MV1LoadModel("Data/Item/Medicine/Heal/Heal.x");
    heal.graphHandle = LoadGraph("Data/Item/Medicine/Heal/Heal_G.png");
    m_ItemResources[ItemKey::MEDICINE_HEAL] = heal;

	// HPアップ
	ItemResource hp_up;
	hp_up.modelHandle = MV1LoadModel("Data/Item/Medicine/HP/Green_Pill.x");
	hp_up.graphHandle = LoadGraph("Data/Item/Medicine/HP/HP_UP_G.png");
	m_ItemResources[ItemKey::MEDICINE_HP_UP] = hp_up;
   
	// スタミナアップ
	ItemResource stamina_up;
	stamina_up.modelHandle = MV1LoadModel("Data/Item/Medicine/Stamina/Yellow_Pill.x");
	stamina_up.graphHandle = LoadGraph("Data/Item/Medicine/Stamina/Stamina_UP_G.png");
	m_ItemResources[ItemKey::MEDICINE_STAMINA_UP] = stamina_up;
   
    // 十字架
	ItemResource cross;
	cross.modelHandle = MV1LoadModel("Data/Item/Treasure/Cross/Cross.x");
	cross.graphHandle = LoadGraph("Data/Item/Treasure/Cross/Cross_G.png");
	m_ItemResources[ItemKey::TREASURE_CROSS] = cross;
    
	// 宝石1(紫)
	ItemResource gem1;
	gem1.modelHandle = MV1LoadModel("Data/Item/Treasure/Gem/Gem.x");
	gem1.graphHandle = LoadGraph("Data/Item/Treasure/Gem/Gem1_G.png");
    m_ItemResources[ItemKey::TREASURE_GEM1] = gem1;
	// 宝石2(緑)
	ItemResource gem2;
	gem2.modelHandle = MV1LoadModel("Data/Item/Treasure/Gem/Gem2.x");
    gem2.graphHandle = LoadGraph("Data/Item/Treasure/Gem/Gem2_G.png");
	m_ItemResources[ItemKey::TREASURE_GEM2] = gem2;
	// 宝石3(赤)
	ItemResource gem3;
    gem3.modelHandle = MV1LoadModel("Data/Item/Treasure/Gem/Gem3.x");
	gem3.graphHandle = LoadGraph("Data/Item/Treasure/Gem/Gem3_G.png");
	m_ItemResources[ItemKey::TREASURE_GEM3] = gem3;

	// 星のオブジェ
	ItemResource star;
	star.modelHandle = MV1LoadModel("Data/Item/Treasure/Interior/Star.x");
	star.graphHandle = LoadGraph("Data/Item/Treasure/Interior/Star_G.png");
	m_ItemResources[ItemKey::TREASURE_STAR] = star;
    
	// ティーポット
	ItemResource teapot;
	teapot.modelHandle = MV1LoadModel("Data/Item/Treasure/Interior/Teapot.x");
	teapot.graphHandle = LoadGraph("Data/Item/Treasure/Interior/teapot_G.png");
	m_ItemResources[ItemKey::TREASURE_TEAPOT] = teapot;

	// はしご
	ItemResource ladder;
	ladder.modelHandle = MV1LoadModel("Data/Enemy/ToolEnemy/LadderEnemy/Ladder.x");
	ladder.graphHandle = LoadGraph("Data/Enemy/ToolEnemy/LadderEnemy/Ladder_G.png");
	m_ItemResources[ItemKey::TOOL_LADDER] = ladder;

	// ハンマー
	ItemResource hammer;
	hammer.modelHandle = MV1LoadModel("Data/Enemy/ToolEnemy/HammerEnemy/Hammer.x");
	hammer.graphHandle = LoadGraph("Data/Enemy/ToolEnemy/HammerEnemy/Hammer_G.png");
	m_ItemResources[ItemKey::TOOL_HAMMER] = hammer;

	// バール
	ItemResource bar;
	bar.modelHandle = MV1LoadModel("Data/Enemy/ToolEnemy/BarEnemy/Bar.x");
	bar.graphHandle = LoadGraph("Data/Enemy/ToolEnemy/BarEnemy/Bar_G.png");
	m_ItemResources[ItemKey::TOOL_BAR] = bar;

	// ノコギリ
	ItemResource saw;
	saw.modelHandle = MV1LoadModel("Data/Enemy/ToolEnemy/SawEnemy/Saw.x");
	saw.graphHandle = LoadGraph("Data/Enemy/ToolEnemy/SawEnemy/Saw_G.png");
	m_ItemResources[ItemKey::TOOL_SAW] = saw;
   
	// ドライバー
	ItemResource driver;
	driver.modelHandle = MV1LoadModel("Data/Enemy/ToolEnemy/ScrewdriverEnemy/Screwdriver.x");
	driver.graphHandle = LoadGraph("Data/Enemy/ToolEnemy/ScrewdriverEnemy/Driver_G.png");
	m_ItemResources[ItemKey::TOOL_DRIVER] = driver;
}

void ItemManager::Start() 
{
    m_respawnTime = 300.0f; // リスポーン時間（秒）
    // ※今後の課題：jsonでアイテムの出現位置を設定できるようにする
    // 回復薬（HPを20回復）
    SpawnItem(std::make_unique<MedicineItem>("回復薬", "HPを20回復", ItemKey::MEDICINE_HEAL, std::make_unique<Heal>(20)), VGet(46.0f, 1.0f, 75.0f), ItemKey::MEDICINE_HEAL,true, m_respawnTime);
	// 回復薬（HPを30回復）
    SpawnItem(std::make_unique<MedicineItem>("回復薬", "HPを30回復", ItemKey::MEDICINE_HEAL, std::make_unique<Heal>(MEDICINE_HEAL_AMOUNT)), VGet(-24.0f, 1.0f, -17.0f), ItemKey::MEDICINE_HEAL, true, m_respawnTime);
    SpawnItem(std::make_unique<MedicineItem>("回復薬", "HPを30回復", ItemKey::MEDICINE_HEAL, std::make_unique<Heal>(MEDICINE_HEAL_AMOUNT)), VGet(-24.0f, 1.0f, -45.0f), ItemKey::MEDICINE_HEAL, true, m_respawnTime);
		    
     // HPアップ薬（HP上限+20）
    SpawnItem(std::make_unique<MedicineItem>("HP強化薬", "HP最大値が20上昇", ItemKey::MEDICINE_HP_UP, std::make_unique<Hp_Up>(MEDICINE_HP_UP_AMOUNT)), VGet(-74.0f, 1.0f, 11.0f), ItemKey::MEDICINE_HP_UP, true, m_respawnTime);
    SpawnItem(std::make_unique<MedicineItem>("HP強化薬", "HP最大値が20上昇", ItemKey::MEDICINE_HP_UP, std::make_unique<Hp_Up>(MEDICINE_HP_UP_AMOUNT)), VGet(36.0f, 1.0f, -10.0f), ItemKey::MEDICINE_HP_UP, true, m_respawnTime);

    // スタミナアップ薬（スタミナ上限+20）
    SpawnItem(std::make_unique<MedicineItem>("強走薬", "スタミナ最大値が20上昇", ItemKey::MEDICINE_STAMINA_UP, std::make_unique<Stamina_Up>(MEDICINE_STAMINA_UP_AMOUNT)), VGet(-77.0f, 1.0f, -44.0f), ItemKey::MEDICINE_STAMINA_UP, true, m_respawnTime);
    SpawnItem(std::make_unique<MedicineItem>("強走薬", "スタミナ最大値が20上昇", ItemKey::MEDICINE_STAMINA_UP, std::make_unique<Stamina_Up>(MEDICINE_STAMINA_UP_AMOUNT)), VGet(23.0f, 1.0f, -9.5f), ItemKey::MEDICINE_STAMINA_UP, true, m_respawnTime);

    // 十字架（スコア+50）
	SpawnItem(std::make_unique<TreasureItem>("十字架", "スコア＋50", ItemKey::TREASURE_CROSS, std::make_unique<Treasure>(TREASURE_CROSS_AMOUNT)), VGet(32.0f, 1.0f, -66.0f), ItemKey::TREASURE_CROSS, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("十字架", "スコア＋50", ItemKey::TREASURE_CROSS, std::make_unique<Treasure>(TREASURE_CROSS_AMOUNT)), VGet(0.1f, 1.0f, 73.0f), ItemKey::TREASURE_CROSS, true, m_respawnTime);

    // 空の回復薬（スコア+20）
    SpawnItem(std::make_unique<TreasureItem>("空の回復薬", "スコア＋20", ItemKey::MEDICINE_HEAL, std::make_unique<Treasure>(TREASURE_EMPTY_HEAL_AMOUNT)), VGet(285.0f, 1.0f, -0.7f), ItemKey::MEDICINE_HEAL, true, m_respawnTime);
    SpawnItem(std::make_unique<TreasureItem>("空の回復薬", "スコア＋20", ItemKey::MEDICINE_HEAL, std::make_unique<Treasure>(TREASURE_EMPTY_HEAL_AMOUNT)), VGet(-24.0f, 1.0f, 67.0f), ItemKey::MEDICINE_HEAL, true, m_respawnTime);
    SpawnItem(std::make_unique<TreasureItem>("空の回復薬", "スコア＋20", ItemKey::MEDICINE_HEAL, std::make_unique<Treasure>(TREASURE_EMPTY_HEAL_AMOUNT)), VGet(-25.0f, 1.0f, 37.0f), ItemKey::MEDICINE_HEAL, true, m_respawnTime);

	// 宝石1（スコア+30）
	SpawnItem(std::make_unique<TreasureItem>("宝石(紫)", "スコア＋30", ItemKey::TREASURE_GEM1, std::make_unique<Treasure>(TREASURE_GEM1_AMOUNT)), VGet(285.0f, 1.0f, -10.0f), ItemKey::TREASURE_GEM1, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("宝石(紫)", "スコア＋30", ItemKey::TREASURE_GEM1, std::make_unique<Treasure>(TREASURE_GEM1_AMOUNT)), VGet(75.0f, 1.0f, 78.0f), ItemKey::TREASURE_GEM1, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("宝石(紫)", "スコア＋30", ItemKey::TREASURE_GEM1, std::make_unique<Treasure>(TREASURE_GEM1_AMOUNT)), VGet(155.0f, 1.0f, 50.0f), ItemKey::TREASURE_GEM1, true, m_respawnTime);

	// 宝石2（スコア+40）
	SpawnItem(std::make_unique<TreasureItem>("宝石(緑)", "スコア＋40", ItemKey::TREASURE_GEM2, std::make_unique<Treasure>(TREASURE_GEM2_AMOUNT)), VGet(265.0f, 1.0f, -60.0f), ItemKey::TREASURE_GEM2, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("宝石(緑)", "スコア＋40", ItemKey::TREASURE_GEM2, std::make_unique<Treasure>(TREASURE_GEM2_AMOUNT)), VGet(-74.0f, 1.0f, 62.0f), ItemKey::TREASURE_GEM2, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("宝石(緑)", "スコア＋40", ItemKey::TREASURE_GEM2, std::make_unique<Treasure>(TREASURE_GEM2_AMOUNT)), VGet(281.0f, 1.0f, 90.0f), ItemKey::TREASURE_GEM2, true, m_respawnTime);

	// 宝石3（スコア+50）
	SpawnItem(std::make_unique<TreasureItem>("宝石(赤)", "スコア＋50", ItemKey::TREASURE_GEM3, std::make_unique<Treasure>(TREASURE_GEM3_AMOUNT)), VGet(200.0f, 1.0f, -50.0f), ItemKey::TREASURE_GEM3, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("宝石(赤)", "スコア＋50", ItemKey::TREASURE_GEM3, std::make_unique<Treasure>(TREASURE_GEM3_AMOUNT)), VGet(-61.0f, 1.0f, -45.0f), ItemKey::TREASURE_GEM3, true, m_respawnTime);

	// 星のオブジェ（スコア+100）
	SpawnItem(std::make_unique<TreasureItem>("星のオブジェ", "スコア＋100", ItemKey::TREASURE_STAR, std::make_unique<Treasure>(TREASURE_STAR_AMOUNT)), VGet(50.0f, 1.0f, 40.0f), ItemKey::TREASURE_STAR, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("星のオブジェ", "スコア＋100", ItemKey::TREASURE_STAR, std::make_unique<Treasure>(TREASURE_STAR_AMOUNT)), VGet(279.0f, 1.0f, 19.0f), ItemKey::TREASURE_STAR, true, m_respawnTime);

	// ティーポット（スコア+80）
	SpawnItem(std::make_unique<TreasureItem>("ティーポット", "スコア＋80", ItemKey::TREASURE_TEAPOT, std::make_unique<Treasure>(TREASURE_TEAPOT_AMOUNT)), VGet(23.0f, 1.0f, 76.0f), ItemKey::TREASURE_TEAPOT, true, m_respawnTime);
	SpawnItem(std::make_unique<TreasureItem>("ティーポット", "スコア＋80", ItemKey::TREASURE_TEAPOT, std::make_unique<Treasure>(TREASURE_TEAPOT_AMOUNT)), VGet(255.0f, 1.0f, 95.0f), ItemKey::TREASURE_TEAPOT, true,m_respawnTime);
}

void ItemManager::Update()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    VECTOR viewDir = MyMath::VecForwardZX(player->GetRot().y);

    FieldItem* nearestItem = nullptr;   // ★ 一番近いアイテムを保持
    float nearestDistSq = FLT_MAX;      // ★ 最小距離（二乗）

    // ===================== アイテム候補探索 =====================
    for (auto& item : m_FieldItems)
    {
        if (!item.active) {
            // リスポーン処理
            if (item.respawn) {
                item.respawnTimer -= 1.0f / 60.0f;
                if (item.respawnTimer <= 0.0f) {
                    if (item.prototype) {
                        item.item = item.prototype->Clone();
                        auto it = m_ItemResources.find(item.modelKey);
                        if (it != m_ItemResources.end())
                        {
                            item.modelHandle = MV1DuplicateModel(it->second.modelHandle);
                        }

                        item.active = true;
                    }
                }
            }
            continue;
        }

        item.SetHighlight(false);
        item.SetUIFlag(false); // ★ UI初期化（全アイテム非表示）

        VECTOR toItem = MyMath::VecSub(item.GetPosition(), player->GetPos());
        float distSq = MyMath::VecSizeSq(item.GetPosition(), player->GetPos());
        toItem = MyMath::VecNormalize(toItem);
        float dot = MyMath::VecDot(viewDir, toItem);
        bool inSight = (dot > SIGHT_DOT_THRESHOLD);

        // 拾える条件                アイテムの方を向いていたら↓
        if (distSq < m_PickupRadius * m_PickupRadius /*&& inSight*/)
        {
            // ★ 一番近いアイテムだけ保持
            if (distSq < nearestDistSq) {
                nearestDistSq = distSq;
                nearestItem = &item;
            }
        }
    }

    // ===================== 実際の拾い処理 =====================
    if (nearestItem && !player->GetInventory()->IsOpenInventory())
    {
        nearestItem->SetHighlight(true);
        nearestItem->SetUIFlag(true);

        if (Input::IsTriggerPadButton(PAD_INPUT_C)) // Xボタン
        {
            auto inventory = player->GetInventory();
            auto clonedItem = nearestItem->GetItem()->Clone();

			// どのインベントリに入れるか判定
            if (clonedItem->GetCategory() == ItemCategory::TOOL)
            {
                if (!inventory->IsToolInventoryMax())
                {
                    // 空きがあれば通常の追加
                    nearestItem->SetToolUIFlag(true);
                    inventory->AddTool(std::move(clonedItem));
                    MV1DeleteModel(nearestItem->GetModelHandle());
                    SoundManager::GetInstance()->PlaySE(SE_TYPE_ITEM_PICKUP);

                    nearestItem->active = false;
                    if (nearestItem->respawn) {
                        nearestItem->respawnTimer = nearestItem->respawnTime;
                    }
                }
                else
                {
                    // 満杯なら入れ替え
                    std::unique_ptr<ItemBase> oldTool = inventory->ReplaceTool(std::move(clonedItem));
                    if (oldTool)
                    {
						// 古いツールをフィールドにドロップする
                        MV1DeleteModel(nearestItem->GetModelHandle());
                        // nearestItem に古いツール本体を移す
                        nearestItem->item = std::move(oldTool);

						// モデルも差し替え
                        nearestItem->modelKey = nearestItem->item->GetKey();
                        auto it2 = m_ItemResources.find(nearestItem->modelKey);
                        if (it2 != m_ItemResources.end())
                        {
                            nearestItem->modelHandle =
                                MV1DuplicateModel(it2->second.modelHandle);
                        }
                        else
                        {
                            nearestItem->modelHandle = -1;
                        }
                        nearestItem->active = true;
                        // 拾った音鳴らす
                        SoundManager::GetInstance()->PlaySE(SE_TYPE_ITEM_PICKUP);
                    }
                }
            }
            else
            {
                if (!inventory->IsInventoryMax())
                {
                    inventory->AddItem(std::move(clonedItem));
                    SoundManager::GetInstance()->PlaySE(SE_TYPE_ITEM_PICKUP);

                    nearestItem->active = false;
                    if (nearestItem->respawn) {
                        nearestItem->respawnTimer = nearestItem->respawnTime;
                        MV1DeleteModel(nearestItem->modelHandle);
                    }
                    else {
                        MV1DeleteModel(nearestItem->modelHandle);
                    }
                }
            }
        }
    }

    // ===================== 非アクティブアイテムの完全削除 =====================
    m_FieldItems.erase(
        std::remove_if(
            m_FieldItems.begin(),
            m_FieldItems.end(),
            [](const FieldItem& item) {
                return !item.active && !item.respawn;
            }
        ),
        m_FieldItems.end()
    );
}

// 描画
void ItemManager::Draw() 
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    VECTOR playerPos = player->GetPos();

    for (const auto& item : m_FieldItems)
    {
        VECTOR pos = item.GetPosition();
        int handle = item.GetModelHandle();

        if (!item.active) continue; // ★ 非表示中は描画しない

        float dist = VSize(VSub(playerPos, pos));

        if (dist > ITEM_DISTANCE) continue; // 遠いから描画しない

		// アイテムの位置にモデルを配置
        MV1SetPosition(handle, pos);
        MV1DrawModel(handle);

		// UI表示
        if (item.IsUIFlag())
        {
			// アイテム名、説明を表示
			DrawFormatString(750, 550, WHITE, "%s", item.GetItem()->GetName().c_str());
			DrawFormatString(750, 570, WHITE, "%s", item.GetItem()->GetDescription().c_str());
			// アイテムを拾うメッセージ表示
            if (item.GetItem()->GetCategory() == ItemCategory::TOOL)
            {
                if (Inventory::GetInstance()->IsToolInventoryMax())
                {
                    DrawFormatString(750, 600, YELLOW, "ツールを入れ替える　(X)");
                }
                else
                {
                    DrawFormatString(750, 600, YELLOW, "ツールを拾う (X)");
                }
            }
            else
            {
                if (Inventory::GetInstance()->IsInventoryMax())
                {
                    DrawFormatString(750, 600, RED, "インベントリがいっぱいです");
                }
                else
                {
                    DrawFormatString(750, 600, WHITE, "アイテムを拾う (X)");
                }
            }
        }
		
    } 
#ifdef _DEBUG
    //  デバッグ描画
    DebugDraw();
#endif // DEBUG
}

void ItemManager::Fin()
{
	// フィールド上のアイテムを削除
    for (const auto& item : m_FieldItems) 
    {
        MV1DeleteModel(item.GetModelHandle());
    }
    m_FieldItems.clear();

	// モデルキャッシュを削除
    for (auto& kv : m_ModelCache)
    {
        MV1DeleteModel(kv.second);
    }
    m_ModelCache.clear();

	// リスポーンタイマーをリセット
	m_respawnTime = 0.0f;
}

const void ItemManager::GetModelCache(ItemModel key, int handle)
{
    m_ModelCache[key] = handle;
}

int const ItemManager::GetItemGraph(ItemKey key)
{
    auto it = m_ItemResources.find(key);
    if (it != m_ItemResources.end())
        return it->second.graphHandle;

    return -1;
}

void ItemManager::DebugDraw()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    if (!player) return;

    // プレイヤー位置・方向ベクトル
    VECTOR playerPos = player->GetPos();
    VECTOR viewDir = MyMath::VecForwardZX(player->GetRot().y);

    // 視線を線で描画（5.0fの長さ）
    VECTOR endPos = VAdd(playerPos, VScale(viewDir, 5.0f));
    DrawLine3D(playerPos, endPos, GREEN);

    for (const auto& item : m_FieldItems)       
    {
        if (!item.active) continue; // 非アクティブは描画しない

        VECTOR pos = item.GetPosition();

        // ① アイテムの拾得範囲（半径 m_PickupRadius の球）
        DrawSphere3D(pos, m_PickupRadius, 16, GetColor(0, 0, 255), GetColor(0, 0, 255), false);

        // ② プレイヤーとアイテムを結ぶ線
        DrawLine3D(playerPos, pos, YELLOW);

        // ③ UIフラグが立っているアイテムは赤枠
        if (item.IsUIFlag())
        {
            DrawSphere3D(pos, m_PickupRadius * 1.05f, 16, RED, RED, false);
        }
    }
}