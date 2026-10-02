#pragma once
#include <memory>
class Player; // 前方宣言

class ItemEffect {
public:
    virtual ~ItemEffect() = default;
    virtual void Apply(Player* target) = 0;
	virtual std::unique_ptr<ItemEffect> Clone() const = 0; // アイテム効果を複製するためのメソッド
};
