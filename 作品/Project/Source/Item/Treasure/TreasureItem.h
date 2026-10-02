#pragma once
#include <memory>
#include "../ItemBase.h"
#include "../Medicine/Effect/ItemEffect.h"


class TreasureItem : public ItemBase
{
protected:
    std::unique_ptr<ItemEffect> m_Effect;

public:
    TreasureItem(const std::string& name, const std::string& desc, ItemKey key, std::unique_ptr<ItemEffect> effect);

    void Use(Player* target) override;// アイテムの効果を適用する 
    void Apply(Player* target) override;  
    std::unique_ptr<ItemBase> Clone() const override;

    std::string GetName() const override; // アイテム名を取得する
	std::string GetDescription() const override; // アイテムの説明を取得する
};
