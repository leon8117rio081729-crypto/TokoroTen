#include "GimmickManager.h"
#include "GimmickParamsLoader.h"
#include "Ladder/LadderGimmick.h"
#include "Hammer/HammerGimmick.h"
#include "Hammer/HammerGimmick_Side.h"
#include "Bar&Saw/Bar&SawGimmick.h"
#include "Bar&Saw/Bar&SawGimmick_Side.h"
#include "Screwdriver/ScrewdriverGimmick.h"
#include "Screwdriver/ScrewdriverGimmick_Side.h"
#include "Door/DoorGimmick.h"
#include "Door/DoorGimmick_Side.h"
#include "../../../Collision/CollisionAABB.h"
#include "../../../Player/Player.h"
#include "../../../Player/PlayerManager.h"

GimmickManager* GimmickManager::m_Instance = nullptr;

GimmickManager::GimmickManager()
    :m_OriginalGimmick{}
{
}

GimmickManager::~GimmickManager()
{
    Fin();
}

void GimmickManager::Init()
{
    // クローン元を登録
    m_OriginalGimmick[LADDER_GIMMICK]       = new LadderGimmick();
    m_OriginalGimmick[HAMMER_GIMMICK]       = new HammerGimmick();
	m_OriginalGimmick[HAMMER_GIMMICK_SIDE]  = new HammerGimmick_Side();
    m_OriginalGimmick[BAR_SAW_GIMMICK]      = new Bar_SawGimmick();
	m_OriginalGimmick[BAR_SAW_GIMMICK_SIDE] = new Bar_SawGimmick_Side();
	m_OriginalGimmick[DRIVER_GIMMICK]       = new ScrewdriverGimmick();
	m_OriginalGimmick[DRIVER_GIMMICK_SIDE]  = new ScrewdriverGimmick_Side();
	m_OriginalGimmick[DOOR_GIMMICK]         = new DoorGimmick();
	m_OriginalGimmick[DOOR_GIMMICK_SIDE]    = new DoorGimmick_Side();

    // ★ 初期化
    for (int i = 0; i < GIMMICK_TYPE_MAX; i++)
    {
        if (m_OriginalGimmick[i])
            m_OriginalGimmick[i]->Init();
    }

    // JSONロード
    GimmickParamsLoader::LoadAllGimmickParams("Data/Object/Gimmick/Tool/ToolGimmick_Stage1.json");
}

void GimmickManager::Load()
{
    for (int i = 0; i < GIMMICK_TYPE_MAX; i++)
        if (m_OriginalGimmick[i]) m_OriginalGimmick[i]->Load();
}

void GimmickManager::Start()
{

    for (auto& gimmick : m_GimmickList)
    {
        gimmick->Start();
    }

	// JSONからギミック配置
    const auto& gimmickParams = GimmickParamsLoader::GetAll();

    for (const auto& param : gimmickParams)
    {
        GimmickBase* g = CreateGimmick(param.type);
		g->Load();// モデル読み込み
		g->SetParams(param);// パラメータを設定
		g->SetPos(param.pos);// 配置位置を設定     
        g->SetRequiredTool(param.tool);// ギミックごとにツール条件を設定できるようにする
		g->SetRequiredTool2(param.tool2);// ギミックごとにツール条件2を設定できるようにする
        g->Init();                   // コリジョンなどの初期化
    }
}

void GimmickManager::Step()
{
    for (auto& gimmick : m_GimmickList)
        gimmick->Step();
}

void GimmickManager::Update()
{
    for (auto& gimmick : m_GimmickList)
        gimmick->Update();
}

void GimmickManager::Draw()
{
    Player* player = PlayerManager::GetInstance()->GetPlayer();
    VECTOR playerPos = player->GetPos();

    for (auto& gimmick : m_GimmickList)
    {
        VECTOR pos = gimmick->GetPos();

        float dist = VSize(VSub(playerPos, pos));

        if (dist > GIMMICK_DISTANCE) continue; // 遠いから描画しない

        gimmick->Draw();
    }
        
}

void GimmickManager::Fin()
{
    // ギミック削除
    for (auto& gimmick : m_GimmickList)
    {
        delete gimmick;
    }
    m_GimmickList.clear();

    // クローン元削除
    for (int i = 0; i < GIMMICK_TYPE_MAX; i++)
    {
        delete m_OriginalGimmick[i];
        m_OriginalGimmick[i] = nullptr;
    }
}

GimmickBase* GimmickManager::CreateGimmick(GimmickType type)
{
    if (!m_OriginalGimmick[type]) return nullptr;

    GimmickBase* gimmick = m_OriginalGimmick[type]->Clone();
    m_GimmickList.push_back(gimmick);
	m_GimmickObjects.push_back(gimmick);
    return gimmick;
}

std::vector<GimmickBase*> GimmickManager::GetGimmickObjects()
{
    // m_GimmickList の内容をコピーして vector を返す
    return std::vector<GimmickBase*>(m_GimmickList.begin(), m_GimmickList.end());
}
