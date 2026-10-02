#include "Bar.h"
#include "../../../../Player/Player.h"
#include <memory>

Bar::Bar(int BarAmount) : m_Bar(BarAmount) {}

void Bar::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_Bar;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Bar::Clone() const
{
    return std::make_unique<Bar>(*this); // コピーコンストラクタで複製
}
