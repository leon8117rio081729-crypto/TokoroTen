#pragma once
#include "ItemEffect.h"

class Player;

class Hp_Up : public ItemEffect {
    int m_Hp_UpAmount;

public:
    Hp_Up(int m_Hp_UpAmount);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
