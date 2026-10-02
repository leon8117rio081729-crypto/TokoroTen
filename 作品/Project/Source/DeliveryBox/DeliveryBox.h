#pragma once
#include <vector>
#include "../Item/ItemBase.h"
#include "DxLib.h"

class CollisionAABB;
class Player;

class DeliveryBox
{
public:

    DeliveryBox();	// コンストラクタ
    ~DeliveryBox();	// デストラクタ

    static void CreateInstance() { if (!m_Instance) m_Instance = new DeliveryBox; }
    // マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
    static DeliveryBox* GetInstance() { return m_Instance; }
    // 使わなくなったら削除する際の削除関数
    static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }
    
    void Init();
    void Load();
	void Start();
    void Update();
    void Draw();

    void SetUIFlag(bool flag) { Uiflag = flag; } // UI表示フラグ
    bool IsUIFlag() const { return Uiflag; } // UI表示フラグ取得
    CollisionAABB* GetAABB() const { return m_AABB; }
    std::vector<DeliveryBox*> GetDeliveryBoxes() { return m_DeliveryBoxes; }

private:
	static DeliveryBox* m_Instance; // シングルトンインスタンス
    VECTOR m_Pos;
    VECTOR m_Size;
	CollisionAABB* m_AABB;	// AABBの当たり判定
    int m_ModelHandle;
    float m_DeliveryRadius = 5.0f; // 納品できる距離
    bool Uiflag;
    bool CheckDelivery(Player* player); // 納品条件チェック
    std::vector< DeliveryBox*> m_DeliveryBoxes;
};
