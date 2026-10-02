#pragma once
#include "../StageObject.h"

enum WallID
{
	WALL_00,
	WALL_01,
	WALL_02,
	WALL_03,
	WALL_MAX
};

class CollisionAABB;

// 床クラス
class Wall : public StageObject
{
public:
	Wall() = default;
	virtual ~Wall() = default;

	void Start() override;
	void SetID(int id) { m_id = id; }
	StageObject* Clone() override;
private:
	int m_id = 0;
};
