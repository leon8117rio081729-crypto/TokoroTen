#pragma once
#include <list>
#include "GimmickBase.h"

constexpr float GIMMICK_DISTANCE = 175.0f; // この距離以上離れているギミックは描画しない

enum GimmickType : int
{
    LADDER_GIMMICK,
    HAMMER_GIMMICK,
    HAMMER_GIMMICK_SIDE,
    BAR_SAW_GIMMICK,
    BAR_SAW_GIMMICK_SIDE,
    DRIVER_GIMMICK,
    DRIVER_GIMMICK_SIDE,
	DOOR_GIMMICK,
	DOOR_GIMMICK_SIDE,
    GIMMICK_TYPE_MAX,
    GIMMICK_TYPE_NONE = -1
};

class GimmickManager
{
    GimmickManager();
    ~GimmickManager();

public:
    static void CreateInstance() { if (!m_Instance) m_Instance = new GimmickManager; }
    static GimmickManager* GetInstance() { return m_Instance; }
    static void DeleteInstance() { delete m_Instance; m_Instance = nullptr; }

public:
    void Init();
    void Load();
    void Start();
    void Step();
    void Update();
    void Draw();
    void Fin();

    GimmickBase* CreateGimmick(GimmickType type);
	const std::list<GimmickBase*> GetGimmickList() { return m_GimmickList; }
    std::vector<GimmickBase*>& GetGimmickObject() { return m_GimmickObjects; }
    std::vector<GimmickBase*> GetGimmickObjects();
    // 管理中の全ての当たり判定AABBを取得する
    std::vector<CollisionAABB*> GetAllCollisionAABBs();

private:
	std::vector<GimmickBase*> m_GimmickObjects;
    static GimmickManager* m_Instance;
    GimmickBase* m_OriginalGimmick[GIMMICK_TYPE_MAX];
    std::list<GimmickBase*> m_GimmickList;
};
