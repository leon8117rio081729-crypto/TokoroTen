#pragma once
#include "../../Medicine/Effect/ItemEffect.h"

class Player;

class Treasure : public ItemEffect
{
	int m_TreasureScore; // 加算スコア量

public:
    Treasure(int m_TreasureScore);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
