#include "Hp_Up.h"
#include "../../../Player/Player.h"
Hp_Up::Hp_Up(int m_Hp_UpAmount)
{
	m_Hp_UpAmount = m_Hp_UpAmount;
}

void Hp_Up::Apply(Player* target)
{
	if (!target) return;
	int maxHp = target->GetHpMax();
	target->SetHpMax(maxHp + 20);  // 例：最大HPを20上昇
}

std::unique_ptr<ItemEffect> Hp_Up::Clone() const
{
	return std::make_unique<Hp_Up>(*this);
}
