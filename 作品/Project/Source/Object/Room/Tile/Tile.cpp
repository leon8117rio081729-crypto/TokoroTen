#include "Tile.h"
#include "../../../Collision/CollisionManager.h"
#include "../../../Collision/CollisionAABB.h"

#define DEFAULT_POS VGet(0.0f, -1.2f, 0.0f)

Tile* Tile::m_Instance = nullptr;

// コンストラクタ
Tile::Tile()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
	m_AABB = nullptr;
	m_Tile = nullptr;
}

// デストラクタ
Tile::~Tile()
{
	Fin();
}

void Tile::Init()
{

}

void Tile::Load()
{
	m_Handle = MV1LoadModel("Data/Object/Room/Tile/Tile.x");
}

void Tile::Start()
{
	m_Pos = DEFAULT_POS;
	// 当たり判定を設定
	m_AABB = CollisionManager::GetInstance()->CreateAABB();
	m_AABB->SetTargetPos(&m_Pos);
	m_AABB->SetLocalPos(VGet(0.0f, 0.0f, 0.0f));
	m_AABB->SetSize(VGet(1.0f, 1.0f, 1.0f));
}

void Tile::Step()
{
}

void Tile::Update()
{
	MV1SetPosition(m_Handle, m_Pos);
}

void Tile::Draw()
{
	MV1DrawModel(m_Handle);
}

void Tile::Fin()
{
	MV1DeleteModel(m_Handle);
}

