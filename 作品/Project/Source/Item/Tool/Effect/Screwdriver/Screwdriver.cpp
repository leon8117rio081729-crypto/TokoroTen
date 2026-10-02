#include "Screwdriver.h"
#include "../../../../Player/Player.h"
#include <memory>

Screwdriver::Screwdriver(int ScrewdriverAmount) : m_Screwdriver(ScrewdriverAmount) {}

void Screwdriver::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_Screwdriver;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Screwdriver::Clone() const
{
    return std::make_unique<Screwdriver>(*this); // コピーコンストラクタで複製
}
