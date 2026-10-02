#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <DxLib.h>
#include "../../../Item/ItemType.h"


enum GimmickType : int;

struct GimmickParam
{
    GimmickType type;
    ToolType tool = ToolType::NONE;
    ToolType tool2 = ToolType::NONE;
    VECTOR pos = VGet(0.0f,0.0f,0.0f);
    // --- コリジョン設定 ---
    float aabbHalfX = 0.0f;   // 半幅（X軸方向の半サイズ）
    float aabbHalfY = 0.0f;   // 半高さ（Y軸方向の半サイズ）
    float aabbHalfZ = 0.0f;   // 半奥行き（Z軸方向の半サイズ）

    float aabbOffsetX = 0.0f; // AABB中心のオフセット（X）
    float aabbOffsetY = 0.0f; // AABB中心のオフセット（Y）
    float aabbOffsetZ = 0.0f; // AABB中心のオフセット（Z）

	// --- 複数スフィアコリジョン設定(仮) ---
    int sphereCountX = 1;        // X方向に並べる数
    int sphereCountY = 1;        // Y方向に並べる数
    int sphereCountZ = 1;        // Z方向に並べる数

    float sphereSpacingX = 0.0f; // X方向の間隔
    float sphereSpacingY = 0.0f; // Y方向の間隔
    float sphereSpacingZ = 0.0f; // Z方向の間隔

    float sphereOffsetX = 0.0f;  // 全体の基準からのXオフセット
    float sphereOffsetY = 0.0f;  // 全体の基準からのYオフセット
    float sphereOffsetZ = 0.0f;  // 全体の基準からのZオフセット

    float sphereRadius = 1.0f;  // 各スフィアの半径
};

class GimmickParamsLoader
{
public:
    static bool LoadAllGimmickParams(const std::string& filePath);
    static const std::vector<GimmickParam>& GetAll();

private:
    static std::vector<GimmickParam> m_GimmickParams;
};

inline std::string GetToolName(ToolType type)
{
    switch (type)
    {
    case ToolType::NONE: return "";
    case ToolType::LADDER: return "はしご";
    case ToolType::HAMMER: return "ハンマー";
    case ToolType::BAR: return "バール";
    case ToolType::SAW: return "のこぎり";
    case ToolType::DRIVER: return "ドライバー";
	case ToolType::DOOR: return "ドア";
    default: return "道具";
    }
}