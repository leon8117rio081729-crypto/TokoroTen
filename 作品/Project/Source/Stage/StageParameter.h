#pragma once
#include "DxLib.h"
#include "json.hpp"
#include <string>

// Unityが出力したjsonデータ
// 必ずUnityに合わせること

struct GameObject
{
	int id = 0;
	VECTOR pos = {};
	VECTOR rot = {};
	VECTOR scale = {};
	std::string name = "";
};

enum class LocateObjectID
{
	// Floor
	FLOOR_00,
	// Player
	PLAYER,
	// Wall
	WALL_00,
	WALL_01,
	WALL_02,
	WALL_03,
	// Door
	DOOR_00,
	// Ceiling
	CEILING_00,
	// Enemy
	ENEMY_00,
	ENEMY_01,
};

// jsonにあるtransformをDxlibのVECTORに変換する
inline VECTOR JsonConvXYZ(const nlohmann::json& j,VECTOR& v)
{
	v.x = j.value("x", 0.0f);
	v.y = j.value("y", 0.0f);
	v.z = j.value("z", 0.0f);
	return v;
}

// jsonデータをGameObject構造体に変換する
inline void from_json(const nlohmann::json& j, GameObject& obj)
{
	obj.id = j.value("id", 0);
	JsonConvXYZ(j["position"], obj.pos);
	JsonConvXYZ(j["rotation"], obj.rot);
	JsonConvXYZ(j["scale"], obj.scale);
	obj.name = j.value("name", "");
}