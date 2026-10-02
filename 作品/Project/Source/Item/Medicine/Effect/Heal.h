#pragma once
#include "ItemEffect.h"

class Player;

class Heal : public ItemEffect
{
    int m_HealAmount;

public:
    Heal(int healAmount);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
