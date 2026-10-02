#include "StageObjectManager.h"
#include "Floor/Floor.h"
#include "Wall/Wall.h"
#include "Door/Door.h"
#include "Ceiling/Ceiling.h"
#include "json.hpp"
#include "../../Player/Player.h"
#include "../../Player/PlayerManager.h"

using json = nlohmann::json;

StageObjectManager* StageObjectManager::m_Instance = nullptr;

StageObjectManager::StageObjectManager()
	: m_OriginalFloors (nullptr) 
	, m_OriginalWalls(nullptr)
	,m_OriginalDoors (nullptr) 
	,m_OriginalCeilings (nullptr) 
{
	m_StageObjects.clear();
}

StageObjectManager::~StageObjectManager()
{
	Fin();
}

void StageObjectManager::Init()
{
	// 複製元となるクラスを生成
	m_OriginalFloors = new Floor[FLOOR_MAX];
	m_OriginalWalls = new Wall[WALL_MAX];
	m_OriginalDoors = new Door[DOOR_MAX];
	m_OriginalCeilings = new Ceiling[CEILING_MAX];

	for (int i = 0; i < WALL_MAX; i++)
	{
		m_OriginalWalls[i].SetID(i);
	}
}

void StageObjectManager::Load()
{
	// 床をロード
	m_OriginalFloors[FLOOR_00].Load("Data/Object/StageObject/Floor.x");
	// 壁をロード
	m_OriginalWalls[WALL_00].Load("Data/Object/StageObject/Wall00.x");//横
	m_OriginalWalls[WALL_01].Load("Data/Object/StageObject/Wall01.x");//縦
	m_OriginalWalls[WALL_02].Load("Data/Object/StageObject/Wall02.x");//横上
	m_OriginalWalls[WALL_03].Load("Data/Object/StageObject/Wall03.x");//縦上
	// ドアをロード
	m_OriginalDoors[DOOR_00].Load("Data/Object/StageObject/Door.x");
	// 天井をロード
	m_OriginalCeilings[CEILING_00].Load("Data/Object/StageObject/Ceiling.x");
}

void StageObjectManager::Start()
{
	for (auto obj : m_StageObjects)
	{
		obj->Start();
	}
}

void StageObjectManager::Update()
{
	for (auto obj : m_StageObjects)
	{
		obj->Update();
	}
}

void StageObjectManager::Draw()
{
	Player* player = PlayerManager::GetInstance()->GetPlayer();
	VECTOR playerPos = player->GetPos();

	for (auto obj : m_StageObjects)
	{
		VECTOR pos = obj->GetPos();

		float dist = VSize(VSub(playerPos, pos));

		if (dist > STAGEOBJECT_DISTANCE) continue; // 遠いから描画しない

		obj->Draw();
	}
}

void StageObjectManager::Fin()
{
	delete[] m_OriginalFloors;
	delete[] m_OriginalWalls;
	delete[] m_OriginalDoors;
	delete[] m_OriginalCeilings;
}

// 床を生成する
Floor* StageObjectManager::CreateFloor(int id)
{
	return CreateStageObject(m_OriginalFloors, id, FLOOR_MAX, m_StageObjects);
}

// 床を生成する（位置、回転、スケール指定あり）
Floor* StageObjectManager::CreateFloor(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	return CreateStageObject(m_OriginalFloors, id, FLOOR_MAX, m_StageObjects, pos, rot, scale);
}

// 壁を生成する
Wall* StageObjectManager::CreateWall(int id)
{
	return CreateStageObject(m_OriginalWalls, id, WALL_MAX, m_StageObjects);
}

// 壁を生成する（位置、回転、スケール指定あり）
Wall* StageObjectManager::CreateWall(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	return CreateStageObject(m_OriginalWalls, id, WALL_MAX, m_StageObjects, pos, rot, scale);
}

// ドアを生成する
Door* StageObjectManager::CreateDoor(int id)
{
	return CreateStageObject(m_OriginalDoors, id, DOOR_MAX, m_StageObjects);
}

// ドアを生成する（位置、回転、スケール指定あり）
Door* StageObjectManager::CreateDoor(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	return CreateStageObject(m_OriginalDoors, id, DOOR_MAX, m_StageObjects, pos, rot, scale);
}

// 天井を生成する
Ceiling* StageObjectManager::CreateCeiling(int id)
{
	return CreateStageObject(m_OriginalCeilings, id, CEILING_MAX, m_StageObjects);
}

// 天井を生成する（位置、回転、スケール指定あり）
Ceiling* StageObjectManager::CreateCeiling(int id, VECTOR pos, VECTOR rot, VECTOR scale)
{
	return CreateStageObject(m_OriginalCeilings, id, CEILING_MAX, m_StageObjects, pos, rot, scale);
}

std::vector<CollisionAABB*> StageObjectManager::GetAllCollisionAABBs()
{
	std::vector<CollisionAABB*> list;
	list.reserve(m_StageObjects.size());

	for (auto* obj : m_StageObjects)
	{
		if (!obj) continue;

		CollisionAABB* aabb = obj->GetAABB();
		if (aabb)
			list.push_back(aabb);
	}

	return list;
}

std::vector<CollisionOBB*> StageObjectManager::GetAllCollisionOBBs()
{
	std::vector<CollisionOBB*> list;
	list.reserve(m_StageObjects.size());

	for (auto* obj : m_StageObjects)
	{
		if (!obj) continue;
		CollisionOBB* obb = obj->GetOBB();
		if (obb)
			list.push_back(obb);
	}
	return list;
}
