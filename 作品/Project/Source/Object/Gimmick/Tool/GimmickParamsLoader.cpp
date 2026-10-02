#include "GimmickParamsLoader.h"
#include <fstream>
#include "json.hpp"
#include "GimmickManager.h"

using json = nlohmann::json;

std::vector<GimmickParam> GimmickParamsLoader::m_GimmickParams;

bool GimmickParamsLoader::LoadAllGimmickParams(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Gimmicks JSON ファイルが開けません: ") + path);
    }

    json data;
    try {
        file >> data;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSONパースエラー: ") + e.what());
    }

    m_GimmickParams.clear();

    if (!data.contains("Gimmicks") || !data["Gimmicks"].is_array()) {
        // 空でも成功扱いするか false を返すかは仕様次第
        return true;
    }

    for (auto& elem : data["Gimmicks"])
    {
        GimmickParam param;
        // type
        if (elem.contains("type") && elem["type"].is_string()) {
            std::string typeStr = elem["type"].get<std::string>();
            if (typeStr == "LADDER_GIMMICK") param.type = LADDER_GIMMICK;
            else if (typeStr == "HAMMER_GIMMICK") param.type = HAMMER_GIMMICK;
            else if (typeStr == "HAMMER_GIMMICK_SIDE") param.type = HAMMER_GIMMICK_SIDE;
            else if (typeStr == "BAR_SAW_GIMMICK") param.type = BAR_SAW_GIMMICK;
			else if (typeStr == "BAR_SAW_GIMMICK_SIDE") param.type = BAR_SAW_GIMMICK_SIDE;
            else if (typeStr == "DRIVER_GIMMICK") param.type = DRIVER_GIMMICK;
			else if (typeStr == "DRIVER_GIMMICK_SIDE") param.type = DRIVER_GIMMICK_SIDE;
			else if (typeStr == "DOOR_GIMMICK") param.type = DOOR_GIMMICK;
			else if (typeStr == "DOOR_GIMMICK_SIDE") param.type = DOOR_GIMMICK_SIDE;
            else param.type = GIMMICK_TYPE_NONE;
        }
        else {
            param.type = GIMMICK_TYPE_NONE;
        }

        // tool (primary)
        param.tool = ToolType::LADDER;
        if (elem.contains("tool") && elem["tool"].is_string()) {
            std::string toolStr = elem["tool"].get<std::string>();
            if (toolStr == "LADDER") param.tool = ToolType::LADDER;
            else if (toolStr == "HAMMER") param.tool = ToolType::HAMMER;
            else if (toolStr == "BAR") param.tool = ToolType::BAR;
            else if (toolStr == "SAW") param.tool = ToolType::SAW;
            else if (toolStr == "DRIVER") param.tool = ToolType::DRIVER;
			else if (toolStr == "NONE") param.tool = ToolType::NONE;
			else param.tool = ToolType::NONE; // デフォルト
        }

        // tool2 （GimmickParam に tool2 がある前提での処理。無ければこのブロックを削除）
        if (elem.contains("tool2") && elem["tool2"].is_string()) {
            std::string toolStr2 = elem["tool2"].get<std::string>();
            // ここで param.tool2 に設定する例（GimmickParam に tool2 フィールドが必要）
            if (toolStr2 == "LADDER") param.tool2 = ToolType::LADDER;
            else if (toolStr2 == "HAMMER") param.tool2 = ToolType::HAMMER;
            else if (toolStr2 == "BAR") param.tool2 = ToolType::BAR;
            else if (toolStr2 == "SAW") param.tool2 = ToolType::SAW;
            else if (toolStr2 == "DRIVER") param.tool2 = ToolType::DRIVER;
			else if (toolStr2 == "NONE") param.tool2 = ToolType::NONE;
            else param.tool2 = ToolType::NONE; // デフォルト
        }

        // position
        if (elem.contains("position") && elem["position"].is_array() && elem["position"].size() >= 3) {
            try {
                float x = elem["position"][0].get<float>();
                float y = elem["position"][1].get<float>();
                float z = elem["position"][2].get<float>();
                param.pos = VGet(x, y, z);
            }
            catch (...) {
                param.pos = VGet(0.0f, 0.0f, 0.0f);
                printfDx("Warning: invalid position value for a gimmick, using (0,0,0)\n");
            }
        }
        else {
            param.pos = VGet(0.0f, 0.0f, 0.0f);
            printfDx("Warning: missing or invalid 'position' for a gimmick, using (0,0,0)\n");
        }

        // コリジョン設定
		param.aabbHalfX = elem.value("aabbHalfX", 0.0f);
		param.aabbHalfY = elem.value("aabbHalfY", 0.0f);
		param.aabbHalfZ = elem.value("aabbHalfZ", 0.0f);

        param.aabbOffsetX = elem.value("aabbOffsetX", 0.0f);
        param.aabbOffsetY = elem.value("aabbOffsetY", 0.0f);
        param.aabbOffsetZ = elem.value("aabbOffsetZ", 0.0f);
        m_GimmickParams.push_back(param);
    }

    return true;
}

const std::vector<GimmickParam>& GimmickParamsLoader::GetAll()
{
    return m_GimmickParams;
}
