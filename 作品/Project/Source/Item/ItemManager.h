#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <DxLib.h>
#include "../Item/ItemBase.h"
#include "../Memory/Memory.h"

constexpr float ITEM_DISTANCE = 150.0f;
static constexpr float SIGHT_DOT_THRESHOLD = 0.75f; // 視界のドット積閾値
static constexpr float ITEM_PICKUP_RADIUS = 5.0f; // アイテムを拾える距離

// 各アイテムの初期効果量
static constexpr int MEDICINE_HEAL_AMOUNT = 30;
static constexpr int MEDICINE_HP_UP_AMOUNT = 20;
static constexpr int MEDICINE_STAMINA_UP_AMOUNT = 20;
static constexpr int TREASURE_CROSS_AMOUNT = 50;
static constexpr int TREASURE_EMPTY_HEAL_AMOUNT = 20;
static constexpr int TREASURE_GEM1_AMOUNT = 30;
static constexpr int TREASURE_GEM2_AMOUNT = 40;
static constexpr int TREASURE_GEM3_AMOUNT = 50;
static constexpr int TREASURE_STAR_AMOUNT = 100;
static constexpr int TREASURE_TEAPOT_AMOUNT = 80;

// アイテムモデルの種類
enum class ItemModel
{
    MEDICINE_HEAL,
    MEDICINE_HP_UP,
    MEDICINE_STAMINA_UP,
    TREASURE_CROSS,
    TREASURE_GEM1,
    TREASURE_GEM2,
    TREASURE_GEM3,
    TREASURE_STAR,
	TREASURE_TEAPOT,
	TOOL_LADDER,
	TOOL_HAMMER,
	TOOL_BAR,
	TOOL_SAW,
	TOOL_DRIVER,
};

// ハッシュ関数の特殊化
namespace std {
    template <>
    struct hash<ItemModel> {
        inline std::size_t operator()(const ItemModel& key) const noexcept {
            return static_cast<std::size_t>(key);
        }
    };
}

struct FieldItem 
{
    std::unique_ptr<ItemBase> item;
    std::unique_ptr<ItemBase> prototype;
    VECTOR position;
	int modelHandle; // モデルハンドル
	bool highlight = false; // ハイライト表示フラグ
	bool Toolhighlight = false; // ツールハイライト表示フラグ
    bool respawn = false;       // ★ リスポーンするか
    float respawnTime = 0.0f;   // ★ 復活までの時間（秒）
    float respawnTimer = 0.0f;  // ★ 現在の残り時間
    bool active = true;         // ★ 今表示されているかどうか
    ItemKey modelKey;   // ★ 復活時に使うモデルの種類を保持
    FieldItem(std::unique_ptr<ItemBase> i, VECTOR pos, int handle, bool r, float time, ItemKey key)
        : item(std::move(i)), position(pos) , modelHandle(handle), respawn(r), respawnTime(time), respawnTimer(0.0f), active(true),modelKey(key)
    {    // ★ 元の prototype を保存（Clone で独立コピー）
        if (item) 
        {
            prototype = item->Clone();
        }
    }
    
    std::unique_ptr<ItemBase> TakeItem() { return std::move(item); }
    ItemBase* GetItem() const { return item.get(); }
    VECTOR GetPosition() const { return position; }
    int GetModelHandle() const { return modelHandle; }
    void SetHighlight(bool val) { highlight = val; }
    bool IsHighlighted() const { return highlight; }
	void SetUIFlag(bool flag) { highlight = flag; } // UI表示フラグ
	bool IsUIFlag() const { return highlight; } // UI表示フラグ取得
	void SetToolUIFlag(bool flag) { Toolhighlight = flag; } // UI表示フラグ
	bool IsToolUIFlag() const { return Toolhighlight; } // UI表示フラグ取得
};

class ItemManager {
public:
    ItemManager();
    ~ItemManager();
    static void CreateInstance() { if (!m_Instance) m_Instance = new ItemManager; }
    // マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
    static ItemManager* GetInstance() { return m_Instance; }
    // 使わなくなったら削除する際の削除関数
    static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

    void Init();  // アイテムの初期配置
	void Load();  // モデル読み込み
	void Start(); // 開始
    void Update();  // プレイヤーと接触しているか判定
    void Draw();  // 表示
	void DebugDraw(); // デバッグ描画
    void Fin();
	int GetItemModel() const { return m_ModelHandle; }
    const  void GetModelCache(ItemModel key, int handle);
    int const GetItemGraph(ItemKey key); 

    // ▼ テンプレート
    // アイテムをフィールドに出現させる
    template <typename T>
    void SpawnItem(std::unique_ptr<T> item, const VECTOR& pos, ItemKey key, bool respawn, float respawnTime = 3.0f)
    {
        static_assert(std::is_base_of_v<ItemBase, T>, "T must derive from ItemBase");

        int modelHandle = -1;
        // ★ ItemResourceから取得
        auto it = m_ItemResources.find(key);
        if (it != m_ItemResources.end())
        {
            modelHandle = MV1DuplicateModel(it->second.modelHandle);
        }

        T* raw = item.release();
        std::unique_ptr<ItemBase> basePtr(static_cast<ItemBase*>(raw));

        // FieldItem に key を渡す
        m_FieldItems.emplace_back(FieldItem(std::move(basePtr), pos, modelHandle, respawn, respawnTime, key));
    }

private:
	int m_ModelHandle; // モデルハンドル
    float m_respawnTime; // リスポーン時間（秒）
    static ItemManager* m_Instance;
    std::unordered_map<ItemKey, ItemResource> m_ItemResources;
	std::unordered_map<ItemModel, int> m_ModelCache; // モデルキャッシュ
    std::vector<FieldItem> m_FieldItems;  // フィールド上のアイテム
    float m_PickupRadius;  // 取得できる距離
    bool m_InventoryMax; // インベントリいっぱいどうか
};
