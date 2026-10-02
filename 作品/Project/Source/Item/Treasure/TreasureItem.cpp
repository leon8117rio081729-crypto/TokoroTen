#include "TreasureItem.h"

TreasureItem::TreasureItem(const std::string& name, const std::string& desc, ItemKey key, std::unique_ptr<ItemEffect> effect)
	: ItemBase(ItemCategory::TREASURE, name, desc, key), m_Effect(std::move(effect)) {
}

void TreasureItem::Use(Player* target)
{
	if (m_Effect)
	{
		m_Effect->Apply(target);
	}
}

void TreasureItem::Apply(Player* target)
{
	if (m_Effect) 
	{
		m_Effect->Apply(target);
	}
}

std::unique_ptr<ItemBase> TreasureItem::Clone() const
{
	// 効果のコピーも必要
	return std::make_unique<TreasureItem>(m_Name, m_Description, m_Key, m_Effect->Clone());
}

std::string TreasureItem::GetName() const
{
	return m_Name;
}

std::string TreasureItem::GetDescription() const
{
	return m_Description;
}
