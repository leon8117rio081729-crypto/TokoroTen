#pragma once
#include <string>
#include <unordered_map>
#include <DxLib.h>

struct EnemyParams {
    std::string id = "";
    std::string name = "";
    int hp = 0;
    float speed = 0.0f;
    int melee = 0;
    int attackSE = 0;
	int deathSE = 0;
    VECTOR SphereCollisionPosition = VGet(0.0f, 0.0f, 0.0f);
    float SphereCollisionRadius = 0.0f;
    float detectionRange = 0.0f;
    int attackInterval = 0;
    float attackRange = 0.0f;
    float attackHitStart = 0.0f;
    float attackHitEnd = 0.0f;
    int attackSphereCount = 0;        // 使用するSphereの数
    float attackSphereSpacing = 0.0f; // Sphere間の距離(Z方向)
	float attackOffsetX = 0.0f;       // 横方向のオフセット
    float attackOffsetY = 0.0f;       // 高さ
    float attackOffsetZ = 0.0f;       // 開始Z位置（根元）
    float attackSphereRadius = 0.0f;  // 各Sphereの半径

    std::unordered_map<std::string, int> animations;

    // ドロップ設定
    std::string dropType = "";
    std::string dropName = "";
    std::string dropDescription = "";
    std::string dropEffect = "";
	int value = 0;
 };
