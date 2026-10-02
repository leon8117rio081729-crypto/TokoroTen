#include "Ladder.h"
#include "../../../../Player/Player.h"
#include <memory>

Ladder::Ladder(int LadderAmount) : m_Ladder(LadderAmount) {}

void Ladder::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_Ladder;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Ladder::Clone() const
{
    return std::make_unique<Ladder>(*this); // コピーコンストラクタで複製
}
