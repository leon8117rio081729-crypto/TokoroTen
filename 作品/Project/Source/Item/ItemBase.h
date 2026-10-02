#pragma once
#include "ItemType.h"
#include <string>
#include <memory>

class Player; // 前方宣言

enum class ItemKey
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
    struct hash<ItemKey> {
        size_t operator()(const ItemKey& key) const noexcept {
            return static_cast<size_t>(key);
        }
    };
}
// アイテムのリソース情報をまとめる構造体
struct ItemResource
{
    int modelHandle;   // 3Dモデル
    int graphHandle;   // UI画像
};

class ItemBase {
protected:
    ItemCategory m_Category;
    std::string m_Name;
    std::string m_Description;
    ItemKey m_Key;

public:
    ItemBase(ItemCategory category, const std::string& name, const std::string& desc, ItemKey key)
        : m_Category(category), m_Name(name), m_Description(desc) ,m_Key(key){
    }

    virtual ~ItemBase() {}

    virtual void Use(Player* target) = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetDescription() const = 0;
	virtual ItemKey GetKey() const { return m_Key; }
	// アイテムのカテゴリを取得する
    ItemCategory GetCategory() const { return m_Category; }
	// アイテムを複製するためのプロトタイプメソッド
    virtual std::unique_ptr<ItemBase> Clone() const = 0;
	// アイテムの効果を適用する
    virtual void Apply(Player* player) = 0;
};
