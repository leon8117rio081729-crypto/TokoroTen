#pragma once
#include "EnemyShotBase.h"

class EnemyNormalShot : public EnemyShotBase
{
public:
	EnemyNormalShot();
	~EnemyNormalShot();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;

	// Update, Draw, FinはShotBaseの処理で十分なので不要

	// 複製、量産するためのクローン関数
	EnemyShotBase* Clone() override;

private:
	//弾が消えるまでの時間
	int m_ShotLife;
};
