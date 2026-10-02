#pragma once
#include "EnemyParams.h"
#include <Windows.h>
#include <string>

class EnemyParamsLoader 
{
public:
    static void LoadAllEnemyParams(const std::string& path);// JSONファイルから敵のパラメータを読み込む
    static const EnemyParams& Get(const std::string& id);

private:
    static std::unordered_map<std::string, EnemyParams> m_Params;
};

// UTF-8からShift_JISに変換する関数
std::string Utf8ToSjis(const std::string& utf8); 
