#pragma once
#include "../../../Medicine/Effect/ItemEffect.h"

class Player;

class Saw : public ItemEffect
{
	int m_Saw; 

public:
    Saw(int m_Saw);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
