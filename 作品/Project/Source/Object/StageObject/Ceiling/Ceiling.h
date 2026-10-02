#pragma once
#include "../StageObject.h"

enum CeilingID
{
	CEILING_00,
	CEILING_MAX
};

class CollisionAABB;

// 床クラス
class Ceiling : public StageObject
{
public:
	Ceiling() = default;
	virtual ~Ceiling() = default;

	void Start() override;
	StageObject* Clone() override;
};
