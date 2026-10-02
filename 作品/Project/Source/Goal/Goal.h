#pragma once
#include "DxLib.h"
// ヘッダーにインクルードを書きたくないので前方定義
class CollisionSphere;


// プレイヤークラス
class Goal
{
public:
	Goal();	// コンストラクタ
	~Goal();	// デストラクタ

public:
	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Step();	// ステップ
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

public:
	CollisionSphere* GetSphereCollisoin() { return m_SphereCollision; }
	bool GetGoalFlag() const { return m_IsActive; }
	void SetGoalFlag(bool flag) { m_IsActive = flag; }
	VECTOR GetPos() const { return m_Pos; }

private:
	int m_Handle;						// 画像ハンドル
	VECTOR m_Pos;						// 座標
	VECTOR m_Rot;						// 回転
	CollisionSphere* m_SphereCollision;	// 球の当たり判定
	bool m_IsActive;					// ゴールが有効かどうか
};

