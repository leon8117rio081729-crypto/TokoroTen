#pragma once
#include "ItemEffect.h"

class Player;

class Stamina_Up : public ItemEffect {
    int m_Stamina_UpAmount;

public:
    Stamina_Up(int m_Hp_UpAmount);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
