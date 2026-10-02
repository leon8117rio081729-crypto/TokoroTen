#include "Treasure.h"
#include "../../../Player/Player.h"
#include <memory>

Treasure::Treasure(int treasureAmount) : m_TreasureScore(treasureAmount) {}

//スコア
void Treasure::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_TreasureScore;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Treasure::Clone() const
{
    return std::make_unique<Treasure>(*this); // コピーコンストラクタで複製
}
