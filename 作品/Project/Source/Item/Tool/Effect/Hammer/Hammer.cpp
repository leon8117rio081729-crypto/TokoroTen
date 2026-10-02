#include "Hammer.h"
#include "../../../../Player/Player.h"
#include <memory>

Hammer::Hammer(int HammerAmount) : m_Hammer(HammerAmount) {}

void Hammer::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_Hammer;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Hammer::Clone() const
{
    return std::make_unique<Hammer>(*this); // コピーコンストラクタで複製
}
