#pragma once
#include "../../../Medicine/Effect/ItemEffect.h"

class Player;

class Bar : public ItemEffect
{
	int m_Bar; 

public:
    Bar(int m_Bar);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
