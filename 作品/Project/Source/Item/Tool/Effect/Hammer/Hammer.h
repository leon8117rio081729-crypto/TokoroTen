#pragma once
#include "../../../Medicine/Effect/ItemEffect.h"

class Player;

class Hammer : public ItemEffect
{
	int m_Hammer; 

public:
    Hammer(int m_Hammer);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
