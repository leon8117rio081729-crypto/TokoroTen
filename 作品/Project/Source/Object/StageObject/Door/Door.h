#pragma once
#include "../StageObject.h"

enum DoorID
{
	DOOR_00,
	DOOR_MAX
};

class CollisionAABB;

// 床クラス
class Door : public StageObject
{
public:
	Door() = default;
	virtual ~Door() = default;

	void Start() override;
	StageObject* Clone() override;
};
