#include "CollisionOBB.h"
#include "CollisionAABB.h"
#include "../MyMath/MyMath.h"
#include "DxLib.h"
#include <corecrt_math.h>

// コンストラクタ
CollisionOBB::CollisionOBB()
    : m_TargetPos(nullptr) 
    , m_LocalPos(VGet(0.0f, 0.0f, 0.0f))
    , m_WorldPos(VGet(0.0f, 0.0f, 0.0f))
    , m_UseWorldPos(false) 
    , m_Rot(VGet(0.0f, 0.0f, 0.0f))
    , m_HalfSize(VGet(1.0f, 1.0f, 1.0f) ) 
    , m_IsCollisionActive(true)  
{
    // 軸初期化（回転なし）
    m_Axis[0] = VGet(1.0f, 0.0f, 0.0f); // X
    m_Axis[1] = VGet(0.0f, 1.0f, 0.0f); // Y
    m_Axis[2] = VGet(0.0f, 0.0f, 1.0f); // Z
}

CollisionOBB::~CollisionOBB()
{
}

// Draw を回転を反映した頂点で描画する実装に置き換え
void CollisionOBB::Draw()
{
    VECTOR c = GetCenter();

    // 軸を使って 8 頂点を計算（中心 + Σ(sign_i * axis_i * half_i)）
    VECTOR corners[8];
    int idx = 0;
    for (int sx = -1; sx <= 1; sx += 2)
    {
        for (int sy = -1; sy <= 1; sy += 2)
        {
            for (int sz = -1; sz <= 1; sz += 2)
            {
                VECTOR v = c;
                v = MyMath::VecAdd(v, MyMath::VecScale(m_Axis[0], sx * m_HalfSize.x));
                v = MyMath::VecAdd(v, MyMath::VecScale(m_Axis[1], sy * m_HalfSize.y));
                v = MyMath::VecAdd(v, MyMath::VecScale(m_Axis[2], sz * m_HalfSize.z));
                corners[idx++] = v;
            }
        }
    }

    // 辺を描画（インデックスは上の生成順に合わせて接続）
    // 下側 0..3, 上側 4..7 (生成順に応じて線を引く)
    DrawLine3D(corners[0], corners[1], GetColor(255, 0, 0));
    DrawLine3D(corners[1], corners[3], GetColor(255, 0, 0));
    DrawLine3D(corners[3], corners[2], GetColor(255, 0, 0));
    DrawLine3D(corners[2], corners[0], GetColor(255, 0, 0));

    DrawLine3D(corners[4], corners[5], GetColor(255, 0, 0));
    DrawLine3D(corners[5], corners[7], GetColor(255, 0, 0));
    DrawLine3D(corners[7], corners[6], GetColor(255, 0, 0));
    DrawLine3D(corners[6], corners[4], GetColor(255, 0, 0));

    for (int i = 0; i < 4; ++i)
    {
        DrawLine3D(corners[i], corners[i + 4], GetColor(255, 0, 0));
    }

    // 軸もわかるように描く（任意）
    DrawLine3D(c, MyMath::VecAdd(c, MyMath::VecScale(m_Axis[0], m_HalfSize.x)), GetColor(0,255,0));
    DrawLine3D(c, MyMath::VecAdd(c, MyMath::VecScale(m_Axis[1], m_HalfSize.y)), GetColor(0,0,255));
    DrawLine3D(c, MyMath::VecAdd(c, MyMath::VecScale(m_Axis[2], m_HalfSize.z)), GetColor(255,255,0));
}

void CollisionOBB::SetSize(VECTOR size)
{
    m_HalfSize = VGet(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
}

void CollisionOBB::SetWorldPos(VECTOR worldPos)
{
    m_UseWorldPos = true;
    m_WorldPos = worldPos;
}

void CollisionOBB::SetRot(VECTOR rotDeg)
{
    m_Rot = VGet( rotDeg.x, rotDeg.y, rotDeg.z);

    // 回転行列生成
    MATRIX rotMat = MyMath::MatRotationXYZ(m_Rot.x,m_Rot.y,m_Rot.z);

    // OBB の軸を回転
    m_Axis[0] = MyMath::MatTransform(rotMat, VGet(1.0f, 0.0f, 0.0f));
    m_Axis[1] = MyMath::MatTransform(rotMat, VGet(0.0f, 1.0f, 0.0f));
    m_Axis[2] = MyMath::MatTransform(rotMat, VGet(0.0f, 0.0f, 1.0f));

    // 正規化（安全のため）
    m_Axis[0] = MyMath::VecNormalize(m_Axis[0]);
    m_Axis[1] = MyMath::VecNormalize(m_Axis[1]);
    m_Axis[2] = MyMath::VecNormalize(m_Axis[2]);
}

VECTOR CollisionOBB::GetCenter() const
{
    if (m_UseWorldPos)
    {
        return m_WorldPos;
    }
    // m_TargetPos が設定されていない場合はローカル位置を基準に返す（NULL参照回避）
    if (m_TargetPos != nullptr)
    {
        return MyMath::VecAdd(*m_TargetPos, m_LocalPos);
    }
    return m_LocalPos;
}

void CollisionOBB::DrawBox3D(VECTOR min, VECTOR max, int color, int fillFlag)
{
    // 8頂点を計算
    VECTOR v[8];
    v[0] = VGet(min.x, min.y, min.z);
    v[1] = VGet(max.x, min.y, min.z);
    v[2] = VGet(max.x, max.y, min.z);
    v[3] = VGet(min.x, max.y, min.z);
    v[4] = VGet(min.x, min.y, max.z);
    v[5] = VGet(max.x, min.y, max.z);
    v[6] = VGet(max.x, max.y, max.z);
    v[7] = VGet(min.x, max.y, max.z);

    // 線でボックスを描画
    DrawLine3D(v[0], v[1], color);
    DrawLine3D(v[1], v[2], color);
    DrawLine3D(v[2], v[3], color);
    DrawLine3D(v[3], v[0], color);

    DrawLine3D(v[4], v[5], color);
    DrawLine3D(v[5], v[6], color);
    DrawLine3D(v[6], v[7], color);
    DrawLine3D(v[7], v[4], color);

    DrawLine3D(v[0], v[4], color);
    DrawLine3D(v[1], v[5], color);
    DrawLine3D(v[2], v[6], color);
    DrawLine3D(v[3], v[7], color);

}

bool CollisionOBB::CheckAABB(const CollisionAABB* aabb) const
{
    if (!aabb) return false;
    if (!m_IsCollisionActive || !aabb->IsActive()) return false;

    // AABB の中心と半サイズ
    VECTOR aabbCenter = aabb->GetCenter();
    VECTOR aabbHalf = aabb->GetHalfSize();

    // OBB の中心と半サイズ、軸
    VECTOR obbCenter = GetCenter();
    VECTOR obbHalf = m_HalfSize;
    const VECTOR& A0 = m_Axis[0];
    const VECTOR& A1 = m_Axis[1];
    const VECTOR& A2 = m_Axis[2];

    // AABB の軸（ワールド軸）
    VECTOR B0 = VGet(1.0f, 0.0f, 0.0f);
    VECTOR B1 = VGet(0.0f, 1.0f, 0.0f);
    VECTOR B2 = VGet(0.0f, 0.0f, 1.0f);

    // 回転行列 R = [ dot(Ai, Bj) ]
    float R[3][3];
    R[0][0] = MyMath::VecDot(A0, B0);
    R[0][1] = MyMath::VecDot(A0, B1);
    R[0][2] = MyMath::VecDot(A0, B2);
    R[1][0] = MyMath::VecDot(A1, B0);
    R[1][1] = MyMath::VecDot(A1, B1);
    R[1][2] = MyMath::VecDot(A1, B2);
    R[2][0] = MyMath::VecDot(A2, B0);
    R[2][1] = MyMath::VecDot(A2, B1);
    R[2][2] = MyMath::VecDot(A2, B2);

    // 絶対値行列（数値安定化のためのイプシロン加算）
    const float EPSILON = 1e-6f;
    float AbsR[3][3];
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
        }
    }

    // AABB center を OBB 中心基準にしたベクトル t を OBB 軸で投影
    VECTOR tVec = MyMath::VecSub(aabbCenter, obbCenter);
    float t[3];
    t[0] = MyMath::VecDot(tVec, A0);
    t[1] = MyMath::VecDot(tVec, A1);
    t[2] = MyMath::VecDot(tVec, A2);

    // 1) OBB の主軸を検査
    for (int i = 0; i < 3; ++i)
    {
        float ra = (i == 0 ? obbHalf.x : (i == 1 ? obbHalf.y : obbHalf.z));
        float rb = aabbHalf.x * AbsR[i][0] + aabbHalf.y * AbsR[i][1] + aabbHalf.z * AbsR[i][2];
        if (fabsf(t[i]) > ra + rb) return false;
    }

    // 2) AABB の主軸（ワールド軸）を検査
    for (int j = 0; j < 3; ++j)
    {
        float ra = obbHalf.x * AbsR[0][j] + obbHalf.y * AbsR[1][j] + obbHalf.z * AbsR[2][j];
        float rb = (j == 0 ? aabbHalf.x : (j == 1 ? aabbHalf.y : aabbHalf.z));
        float tj = fabsf(MyMath::VecDot(tVec, (j == 0 ? B0 : (j == 1 ? B1 : B2))));
        if (tj > ra + rb) return false;
    }

    // 3) 交差軸（Ai x Bj）を検査
    // 9 軸
    // i = 0..2, j = 0..2
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int i1 = (i + 1) % 3;
            int i2 = (i + 2) % 3;
            int j1 = (j + 1) % 3;
            int j2 = (j + 2) % 3;

            float ra = ( (i1==0?obbHalf.x:(i1==1?obbHalf.y:obbHalf.z)) * AbsR[i2][j] ) +
                       ( (i2==0?obbHalf.x:(i2==1?obbHalf.y:obbHalf.z)) * AbsR[i1][j] );

            float rb = ( (j1==0?aabbHalf.x:(j1==1?aabbHalf.y:aabbHalf.z)) * AbsR[i][j2] ) +
                       ( (j2==0?aabbHalf.x:(j2==1?aabbHalf.y:aabbHalf.z)) * AbsR[i][j1] );

            float tval = fabsf( t[i2] * R[i1][j] - t[i1] * R[i2][j] );
            if (tval > ra + rb) return false;
        }
    }

    // 全ての分離軸で分離がなければ衝突している
    return true;
}