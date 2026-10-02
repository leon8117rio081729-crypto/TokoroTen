#include "GoalManager.h"
#include "Goal.h"
#include "../Player/Player.h"
#include "../Player/PlayerManager.h"

// 静的変数の初期化
GoalManager* GoalManager::m_Instance = nullptr;

// コンストラクタ
GoalManager::GoalManager()
// 生成されていなければnullptr
	: m_Goal(nullptr)
{
}

// デストラクタ
GoalManager::~GoalManager()
{
	// 削除時の後始末忘れを防止する
	Fin();
}

void GoalManager::CreateGoal()
{
	// プレイヤーが生成されていなければ生成する
	if (!m_Goal)
	{
		m_Goal = new Goal;
	}
}

void GoalManager::Init()
{
	if (m_Goal)
	{
		m_Goal->Init();
	}
}

void GoalManager::Load()
{
	if (m_Goal)
	{
		m_Goal->Load();
	}
}

void GoalManager::Start()
{
	if (m_Goal)
	{
		m_Goal->Start();
	}
}

void GoalManager::Step()
{
	if (m_Goal)
	{
		m_Goal->Step();
	}
}

void GoalManager::Update()
{
	if (m_Goal)
	{
		m_Goal->Update();
	}
}

void GoalManager::Draw()
{
	if (m_Goal)
	{
		m_Goal->Draw();
	}
}

void GoalManager::Fin()
{
	if (m_Goal)
	{
		m_Goal->Fin();
	}
}
