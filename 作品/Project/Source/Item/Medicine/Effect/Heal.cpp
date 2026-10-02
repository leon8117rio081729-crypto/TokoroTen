#include "Heal.h"
#include "../../../Player/Player.h"
#include <memory>

Heal::Heal(int healAmount) : m_HealAmount(healAmount) {}
//回復
void Heal::Apply(Player* target) {
    if (!target) return;

    int hp = target->GetHp();
    int maxHp = target->GetHpMax();

    hp += m_HealAmount;
    if (hp > maxHp) hp = maxHp;

    target->SetHp(hp);
}

std::unique_ptr<ItemEffect> Heal::Clone() const
{
    return std::make_unique<Heal>(*this); // コピーコンストラクタで複製
}
