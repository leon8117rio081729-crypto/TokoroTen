#pragma once
#include "DxLib.h"

class CollisionOBB
{
public:
    CollisionOBB();
    ~CollisionOBB();

public:
    void Draw(); // デバッグ描画
    void DrawBox3D(VECTOR min, VECTOR max, int color, int fillFlag);

    // 設定系
    void SetTargetPos(VECTOR* targetPos) { m_TargetPos = targetPos; }
    void SetLocalPos(VECTOR localPos) { m_LocalPos = localPos; }
    void SetSize(VECTOR size);
    void SetRot(VECTOR rot); // 回転（degree → radに変換）
    void SetWorldPos(VECTOR worldPos);

    // 取得系
    VECTOR GetCenter() const;
    VECTOR GetAxis(int i) const { return m_Axis[i]; }     // 3軸
    VECTOR GetHalfSize() const { return m_HalfSize; }
    VECTOR GetLocalPos() const { return m_LocalPos; }

    bool IsActive() const { return m_IsCollisionActive; }
    void SetEnable(bool e) { m_IsCollisionActive = e; }

public:
    // 衝突判定（AABB vs OBB）
    bool CheckAABB(const class CollisionAABB* aabb) const;

private:
    VECTOR* m_TargetPos;     // 参照元
    VECTOR  m_LocalPos;      // ローカル位置
    VECTOR  m_WorldPos;      // 直接指定
    bool    m_UseWorldPos;

    VECTOR  m_Rot;           // rad
    VECTOR  m_HalfSize;      // 1/2 サイズ

    VECTOR  m_Axis[3];       // 回転後軸（右・上・前）

    bool    m_IsCollisionActive;
};
