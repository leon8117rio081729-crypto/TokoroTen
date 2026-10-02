#include "MedicineItem.h"

MedicineItem::MedicineItem(const std::string& name, const std::string& desc, ItemKey key, std::unique_ptr<ItemEffect> effect)
	: ItemBase(ItemCategory::MEDICINE, name, desc, key), m_Effect(std::move(effect)) {
}

void MedicineItem::Use(Player* target)
{
	if (m_Effect)
	{
		m_Effect->Apply(target);
	}
}

void MedicineItem::Apply(Player* player)
{
	if (m_Effect)
	{
		m_Effect->Apply(player);
	}
}

std::unique_ptr<ItemBase> MedicineItem::Clone() const 
{
	return std::make_unique<MedicineItem>(m_Name, m_Description, m_Key, m_Effect->Clone());
}

std::string MedicineItem::GetName() const
{
	return m_Name;
}

std::string MedicineItem::GetDescription() const
{
	return m_Description;
}
