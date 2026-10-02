#include "ToolItem.h"

ToolItem::ToolItem(const std::string& name, const std::string& desc, ItemKey key, std::unique_ptr<ItemEffect> effect, ToolType type,ToolType type2)
	: ItemBase(ItemCategory::TOOL, name, desc, key), m_Effect(std::move(effect)), m_ToolType(type) ,m_ToolType2(type2){
}

void ToolItem::Use(Player* target)
{
	if (m_Effect)
	{
		m_Effect->Apply(target);
	}
}

void ToolItem::Apply(Player* target)
{
	if (m_Effect) 
	{
		m_Effect->Apply(target);
	}
}

std::unique_ptr<ItemBase> ToolItem::Clone() const
{
	// 効果のコピーも必要
	return std::make_unique<ToolItem>(m_Name, m_Description, m_Key, m_Effect->Clone(), m_ToolType, m_ToolType2);
}

std::string ToolItem::GetName() const
{
	return m_Name;
}

std::string ToolItem::GetDescription() const
{
	return m_Description;
}
