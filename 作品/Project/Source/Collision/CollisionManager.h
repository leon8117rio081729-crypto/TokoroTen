#pragma once
#include <DxLib.h>
#include <vector>
// Box.hをインクルードしなくて済むように前方定義
class CollisionAABB;
class CollisionSphere;
class CollisionOBB;

constexpr int COLLISION_MAX = 2048;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

public:
	// マネージャーインスタンス管理
	static void CreateInstance() { if (!m_Instance) m_Instance = new CollisionManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static CollisionManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Draw();	// 描画
	void Fin();		// 終了

public:
	// AABBを生成する
	CollisionAABB* CreateAABB();
	// AABBを削除する
	void DeleteAABB(CollisionAABB* targetAABB);
	// Sphereを生成する
	CollisionSphere* CreateSphere();
	// Sphereを削除する
	void DeleteSphere(CollisionSphere* targetSphere);
	// OBBを生成する
	CollisionOBB* CreateOBB();
	// OBBを削除する
	void DeleteOBB(CollisionOBB* targetOBB);



public:
	// 当たり判定のチェック
	void CheckCollision();
	//bool CheckAABB(class CollisionAABB* other);
	static bool CheckSphereCollision(CollisionSphere* s1, CollisionSphere* s2);
	static bool CheckSphereAABBCollision(CollisionSphere* s, CollisionAABB* aabb); 
	static bool CheckAABBCollision(CollisionAABB* aabb1, CollisionAABB* aabb2);
	// カメラ用の一時Sphereを生成（newで作って呼び出し元でdeleteする）
	CollisionSphere* CreateTempSphere(VECTOR pos, float radius);

	// カメラのSphereとブロック群の衝突チェック
	bool CheckCameraCollision(const CollisionSphere* camSphere, VECTOR* correctedPos);

	// カメラ用のRay vs AABB 衝突チェック
	static bool CheckCameraCollisionRay(const VECTOR& start, const VECTOR& end, VECTOR* correctedPos, float padding = 0.2f);

	// 汎用 AABB vs Sphere 判定
	static bool CheckAABBSphereCollision(CollisionAABB* aabb, CollisionSphere* sphere);

private:
	// CollisionManagerインスタンス
	static CollisionManager* m_Instance;
	// 当たり判定管理用配列
	CollisionAABB* m_AABB[COLLISION_MAX];
	CollisionSphere* m_Sphere[COLLISION_MAX];
	CollisionOBB* m_OBB[COLLISION_MAX];
};
