#pragma once
#include "../../../../Object/Gimmick/Tool/GimmickBase.h"

class CollisionSphere;

class HammerGimmick : public GimmickBase
{
public:
	HammerGimmick();
	~HammerGimmick();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Update() override; // プレイヤーとの距離を測り、状態を更新
	// プレイヤーとの当たり判定処理
	void OnPlayerInteract(Player* player);
	// 複製、量産するためのクローン関数
	GimmickBase* Clone() override;

};

