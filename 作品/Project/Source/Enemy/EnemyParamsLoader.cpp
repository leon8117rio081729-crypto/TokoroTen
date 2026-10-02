#include "EnemyParamsLoader.h"
#include <fstream>
#include <json.hpp>
#include <DxLib.h>
std::unordered_map<std::string, EnemyParams> EnemyParamsLoader::m_Params;

void EnemyParamsLoader::LoadAllEnemyParams(const std::string& path)
{
    using json = nlohmann::json;
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Enemies.json ファイルが開けません");
    }
    json data;
    try {
        file >> data;
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSONパースエラー: ") + e.what());
    }

	// JSONデータからEnemyParamsを読み込む
    for (auto& enemyData : data["enemies"])
    {
        EnemyParams params;
        params.id = Utf8ToSjis(enemyData["id"]);
        params.name = Utf8ToSjis(enemyData["name"]);
        params.hp = enemyData["hp"];
        params.speed = enemyData["speed"];
		params.melee = enemyData["melee"];
        auto& pos = enemyData["SphereCollisionPosition"];
        float sx = pos.value("x", 0.0f);
        float sy = pos.value("y", 0.0f);
        float sz = pos.value("z", 0.0f);
        params.SphereCollisionPosition = VGet(sx, sy, sz);
		params.SphereCollisionRadius = enemyData["SphereCollisionRadius"];
        params.detectionRange = enemyData["detectionRange"];
        params.attackInterval = enemyData["attackInterval"];
        params.attackRange = enemyData["attackRange"];
		params.attackHitStart = enemyData["attackHitStart"];
        params.attackHitEnd = enemyData["attackHitEnd"];
		params.attackOffsetX = enemyData["attackOffsetX"];
		params.attackOffsetY = enemyData["attackOffsetY"];
		params.attackOffsetZ = enemyData["attackOffsetZ"];
		params.attackSphereCount = enemyData["attackSphereCount"];
		params.attackSphereSpacing = enemyData["attackSphereSpacing"];
		params.attackSphereRadius = enemyData["attackSphereRadius"];
		// ドロップ設定
        params.dropType = enemyData["dropItem"]["type"];
        params.dropName = Utf8ToSjis(enemyData["dropItem"]["name"]);
        params.dropDescription = Utf8ToSjis(enemyData["dropItem"]["description"]);
        params.dropEffect = enemyData["dropItem"]["effect"];
        params.value = enemyData["dropItem"]["value"];

        std::string id = enemyData["id"];
        m_Params[id] = params;
    }
}

const EnemyParams& EnemyParamsLoader::Get(const std::string& id)
{
    return m_Params.at(id);
}

// DxlibはShift_JISにしか対応していないため
// UTF-8からShift_JISへ変換する関数
std::string Utf8ToSjis(const std::string& utf8)
{
    if (utf8.empty()) return {};

    // UTF-8 → UTF-16
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::wstring wideStr(wideLen - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wideStr[0], wideLen);

    // UTF-16 → Shift-JIS
    int sjisLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string sjisStr(sjisLen - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &sjisStr[0], sjisLen, NULL, NULL);

    return sjisStr;
}


