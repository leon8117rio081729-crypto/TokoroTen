#pragma once
#include "../../../Medicine/Effect/ItemEffect.h"

class Player;

class Ladder : public ItemEffect
{
	int m_Ladder; 

public:
    Ladder(int m_Ladder);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
