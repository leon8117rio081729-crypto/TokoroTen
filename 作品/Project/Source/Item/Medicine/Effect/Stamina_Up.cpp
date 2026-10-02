#include "Stamina_Up.h"
#include "../../../Player/Player.h"
Stamina_Up::Stamina_Up(int m_Stamina_UpAmount)
{
	m_Stamina_UpAmount = m_Stamina_UpAmount;
}

void Stamina_Up::Apply(Player* target)
{
	if (!target) return;
	int maxStamina = target->GetStaminaMax();
	target->SetStaminaMax(maxStamina + 20);  // 例：最大HPを20上昇
}

std::unique_ptr<ItemEffect> Stamina_Up::Clone() const
{
	return std::make_unique<Stamina_Up>(*this);
}
