#pragma once
#include "../../../Medicine/Effect/ItemEffect.h"

class Player;

class Screwdriver : public ItemEffect
{
	int m_Screwdriver; 

public:
    Screwdriver(int m_Screwdriver);
    void Apply(Player* target) override;
    std::unique_ptr<ItemEffect> Clone() const override;
};
