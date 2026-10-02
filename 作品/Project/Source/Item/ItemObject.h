#pragma once
#include <memory>
#include "ItemBase.h"
#include "DxLib.h"

class ItemObject {
public:
    ItemObject(std::unique_ptr<ItemBase> item, VECTOR pos)
        : m_Item(std::move(item)), m_Position(pos) {
    }

    ItemBase* GetItem() const { return m_Item.get(); }
    VECTOR GetPosition() const { return m_Position; }
    
        
    
private:
    std::unique_ptr<ItemBase> m_Item;
    VECTOR m_Position;
};
