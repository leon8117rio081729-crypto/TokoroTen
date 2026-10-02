#pragma once
#include <vector>
#include <memory>
#include "DxLib.h"
#include "../../../Item/ItemType.h"
#include "../../../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../../../Collision/CollisionSphere.h"
#include "../../../Collision/CollisionAABB.h"

class Player;

// ★ 各ギミックに複数の球を持たせるための構造体
struct GimmickCollisionData
{
	std::unique_ptr<CollisionAABB> aabb = nullptr; // boxのコリジョン
	VECTOR localOffset = VGet(0.0f, 0.0f, 0.0f);   // ローカル座標からのオフセット
	VECTOR halfSize = VGet(0.0f, 0.0f, 0.0f);      // AABBの半サイズ
};

class GimmickBase
{
public:

	// プレイヤーがギミックを認識する範囲と、実際にインタラクトできる範囲の定数
	static constexpr float DETECT_RANGE   = 6.5f; // ← ギミックを認識する範囲
	static constexpr float INTERACT_RANGE = 6.0f; // ← 実際に使える距離

	GimmickBase();
	virtual ~GimmickBase();
public:
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Start() = 0;
	virtual void Step();
	virtual void Update();
	virtual void Draw();
	virtual void Fin();
public:
	void SetPos(const VECTOR& pos) { m_Pos = pos; }
	VECTOR GetPos() { return m_Pos; }
	void SetActive(bool active) { m_Active = active; }
	bool GetActive() { return m_Active; }
	void SetRequiredTool(ToolType tool) { m_RequiredTool = tool; }
	void SetRequiredTool2(ToolType tool2) { m_RequiredTool2 = tool2; }
	ToolType GetRequiredTool() const { return m_RequiredTool; }
	ToolType GetRequiredTool2() const { return m_RequiredTool2; }
	virtual GimmickBase* Clone() = 0; // クローン関数を純粋仮想関数として宣言
	virtual void OnPlayerInteract(Player* player) = 0;
	void SetParams(const GimmickParam& params) { m_Params = params; }
	const GimmickParam& GetParams() const { return m_Params; }
protected:
	bool CheckToolEffect(Player* player, ToolType requiredTool,ToolType requiredTool2);
	void CopyBaseAttributes(GimmickBase* clone) const; // 基本属性をコピーするヘルパー関数

	// ★ コリジョン生成（JSONから）
	void SetupCollisionAABB(const GimmickParam& param);
public:
	// プレイヤー側で判定を行うために AABB データを取得する getter
	const std::vector<GimmickCollisionData>& GetCollisionAABBData() const { return m_CollisionAABBs; }
	int m_Handle;
	int m_UsedHandle;
	bool m_Active;
	bool m_IsUsed; // ギミックが使用されたかどうかのフラグ
	float m_UseRadius; // 反応距離
	bool m_Uiflag;
	bool m_CanLadder;
	bool m_UiCanLadder;
	bool m_DoorGmimickFlag; // ドアギミックかどうかのフラグ
	bool m_CollisionDisabled;
	VECTOR m_Pos;
	ToolType m_RequiredTool;  // ★必要ツールタイプを保持
	ToolType m_RequiredTool2;  // ★必要ツールタイプ2を保持
	GimmickParam m_Params;
	std::vector<GimmickCollisionData> m_CollisionSpheres;
	std::vector<GimmickCollisionData> m_CollisionAABBs;

};