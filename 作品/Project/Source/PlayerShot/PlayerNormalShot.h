#pragma once
#include "PlayerShotBase.h"

class PlayerNormalShot : public PlayerShotBase
{
public:
	PlayerNormalShot();
	~PlayerNormalShot();
public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;

	// Update, Draw, FinはShotBaseの処理で十分なので不要

	// 複製、量産するためのクローン関数
	PlayerShotBase* Clone() override;

private:
	//弾が消えるまでの時間
	int m_ShotLife;
};
