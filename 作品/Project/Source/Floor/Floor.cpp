#include "Floor.h"

#define DEFAULT_POS VGet(0.0f, 0.0f, 0.0f)

Floor* Floor::m_Instance = nullptr;

// コンストラクタ
Floor::Floor()
{
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

// デストラクタ
Floor::~Floor()
{
	Fin();
}

void Floor::Init()
{

}

void Floor::Load()
{
	m_Handle = MV1LoadModel("Data/Floor/Floor.x");
}

void Floor::Start()
{
	m_Pos = DEFAULT_POS;
}

void Floor::Step()
{
}

void Floor::Update()
{
	//MV1SetPosition(m_Handle, m_Pos);
}

void Floor::Draw()
{
	MV1DrawModel(m_Handle);
}

void Floor::Fin()
{
	MV1DeleteModel(m_Handle);
}

