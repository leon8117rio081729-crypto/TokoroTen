#include "Saw.h"
#include "../../../../Player/Player.h"
#include <memory>

Saw::Saw(int SawAmount) : m_Saw(SawAmount) {}

void Saw::Apply(Player* target) {
    if (!target) return;
    int score = target->GetScore();
    score += m_Saw;
    target->SetScore(score);
}

std::unique_ptr<ItemEffect> Saw::Clone() const
{
    return std::make_unique<Saw>(*this); // コピーコンストラクタで複製
}
