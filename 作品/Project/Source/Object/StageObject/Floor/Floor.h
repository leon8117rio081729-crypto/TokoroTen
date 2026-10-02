#pragma once
#include "../StageObject.h"

enum FloorID
{
	FLOOR_00,
	FLOOR_MAX
};

class CollisionAABB;

// 床クラス
class Floor : public StageObject
{
public:
	Floor() = default;
	virtual ~Floor() = default;

	void Start() override;
	StageObject* Clone() override;
};
