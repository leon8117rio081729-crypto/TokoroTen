#include "StageManager.h"
#include "StageParameter.h"
#include "../Object/StageObject/StageObjectManager.h"
#include "../Player/PlayerManager.h"
#include "../Enemy/EnemyManager.h"
#include <fstream>
#include "../Object/StageObject/Floor/Floor.h"
#include "../Object/StageObject/Wall/Wall.h"
#include "../Object/StageObject/Door/Door.h"
#include "../Object/StageObject/Ceiling/Ceiling.h"
#include "../Collision/CollisionOBB.h"

StageManager* StageManager::m_Instance = nullptr;

// usingして使いやすくする
using json = nlohmann::json;
// データがまとまっている階層のキー名
constexpr const char* KEY_ITEMS = "items";

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
    Fin();
}

void StageManager::Load(const char* fileName)
{
	// jsonファイルを開く
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		// ファイルが開けなかった場合のエラーハンドリング
		return;
	}

	// 開いたjsonファイルをjsonクラスに取り込み
	json stageJson;
	file >> stageJson;

	// from_json関数を元にjsonをvectorに格納
	m_Objects = stageJson[KEY_ITEMS].get<std::vector<GameObject>>();

	// ファイルを閉じる
	file.close();
}

/// ステージ開始処理
/// 主に各オブジェクトを配置する
void StageManager::Start()
{
    for (const GameObject& obj : m_Objects)
    {
        // 天井
        if (obj.id >= static_cast<int>(LocateObjectID::CEILING_00))
        {
            int id = obj.id - static_cast<int>(LocateObjectID::CEILING_00);
            StageObjectManager::GetInstance()->CreateCeiling(id, obj.pos, obj.rot, obj.scale);
        }
        // ドア
        else if (obj.id >= static_cast<int>(LocateObjectID::DOOR_00))
        {
            int id = obj.id - static_cast<int>(LocateObjectID::DOOR_00);
            StageObjectManager::GetInstance()->CreateDoor(id, obj.pos, obj.rot, obj.scale);
        }    
        // 壁
        else if (obj.id >= static_cast<int>(LocateObjectID::WALL_00))
        {
            int id = obj.id - static_cast<int>(LocateObjectID::WALL_00);
            StageObjectManager::GetInstance()->CreateWall(id, obj.pos, obj.rot, obj.scale);
        }
        // 床
        else if (obj.id >= static_cast<int>(LocateObjectID::FLOOR_00))
        {
            int id = obj.id - static_cast<int>(LocateObjectID::FLOOR_00);
            StageObjectManager::GetInstance()->CreateFloor(id, obj.pos, obj.rot, obj.scale);
        }
    }
}

void StageManager::Draw()
{
}

void StageManager::Fin()
{

}
