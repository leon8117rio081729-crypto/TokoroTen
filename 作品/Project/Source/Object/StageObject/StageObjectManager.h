#pragma once
#include "DxLib.h"
#include <vector>
#include <list>

constexpr float STAGEOBJECT_DISTANCE = 175.0f; // この距離以上離れているステージオブジェクトは描画しない

class CollisionAABB;
class CollisionOBB;
class StageObject;
class Floor;
class Wall;
class Door;
class Ceiling;

// プレイヤーオブジェクト管理クラス
class StageObjectManager
{
public:
	StageObjectManager();	// コンストラクタ
	~StageObjectManager();	// デストラクタ

	static void CreateInstance() { if (!m_Instance) m_Instance = new StageObjectManager; }
	static StageObjectManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

	// 床を生成する
	Floor* CreateFloor(int id);
	Floor* CreateFloor(int id, VECTOR pos, VECTOR rot, VECTOR scale);
	// 壁を生成する
	Wall* CreateWall(int id);
	Wall* CreateWall(int id, VECTOR pos, VECTOR rot, VECTOR scale);
	// ドアを生成する
	Door* CreateDoor(int id);
	Door* CreateDoor(int id, VECTOR pos, VECTOR rot, VECTOR scale);
	// 天井を生成する
	Ceiling* CreateCeiling(int id);
	Ceiling* CreateCeiling(int id, VECTOR pos, VECTOR rot, VECTOR scale);

	// 管理中の全てのステージオブジェクトを取得する
	std::vector<StageObject*>& GetStageObjects() { return m_StageObjects; }
	// 管理中の全ての当たり判定AABBを取得する
	std::vector<CollisionAABB*> GetAllCollisionAABBs();
	std::vector<CollisionOBB*> GetAllCollisionOBBs();

private:
	static StageObjectManager* m_Instance;
	std::vector<StageObject*> m_StageObjects;
	Floor* m_OriginalFloors;
	Wall* m_OriginalWalls;
	Door* m_OriginalDoors;
	Ceiling* m_OriginalCeilings;
	// テンプレート宣言
	template <typename T>
	T* CreateStageObject(T* originals, int id, int max, std::vector<StageObject*>& list);

	template <typename T>
	T* CreateStageObject(T* originals, int id, int max,
		std::vector<StageObject*>& list,
		VECTOR pos, VECTOR rot, VECTOR scale);
};

// テンプレート
template <typename T>
T* StageObjectManager::CreateStageObject(
	T* originals, int id, int max, std::vector<StageObject*>& list)
{
	if (id < 0 || id >= max) return nullptr;

	StageObject* obj = originals[id].Clone(); // オリジナルから複製
	if (!obj) return nullptr;
	m_StageObjects.push_back(obj);
	return static_cast<T*>(obj);
}

template <typename T>
T* StageObjectManager::CreateStageObject(
	T* originals, int id, int max, std::vector<StageObject*>& list,
	VECTOR pos, VECTOR rot, VECTOR scale)
{
	T* obj = CreateStageObject(originals, id, max, list);
	if (obj)
		obj->SetTransform(pos, rot, scale);
	return obj;
}