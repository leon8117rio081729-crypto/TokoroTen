#pragma once

#include "DxLib.h"
#include "../Effect/Effect.h"

class CollisionSphere;

constexpr float HOMING_RANGE = 5.0f;// ホーミングする範囲

class PlayerShotBase
{
public:
    PlayerShotBase();
    virtual ~PlayerShotBase();

    virtual void Init() = 0;
    virtual void Load() = 0;
    virtual void Start();
    virtual void Step();

	// 各ショット専用で処理を作る必要がない場合は基底クラスで共通処理にする
    virtual void Update();
    virtual void Draw();
    virtual void Fin();

	// 複製、量産するためのクローン関数
    virtual PlayerShotBase* Clone() = 0;

public:
    void SetPos(const VECTOR& pos) { m_Pos = pos; }
    void SetMove(const VECTOR& move) { m_Move = move; }
    void SetActive(bool active) { m_IsActive = active; }
    bool IsActive() const { return m_IsActive; }

    void Respawn(int life);
    CollisionSphere* GetSphereCollision() { return m_SphereCollision; }

    // エフェクト取得用の仮想関数（nullptrデフォルト）
    virtual class EffectBase* GetEffect() { return nullptr; }

protected:
    int m_Handle;
    Effect* m_Effect;   // 再生中のエフェクト
    VECTOR m_Pos;
    VECTOR m_Rot;
    VECTOR m_Move;

    bool m_IsActive;
    int m_ShotLife;

    CollisionSphere* m_SphereCollision;
};