#pragma once
#include "ShotBase.h"



class EnemyShot : public ShotBase
{
public:
	EnemyShot();
	~EnemyShot();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;

	// Update, Draw, FinはShotBaseの処理で十分なので不要

	// 複製、量産するためのクローン関数
	ShotBase* Clone() override;



private:
	//弾が消えるまでの時間
	int m_ShotLife;
	
};
