#pragma once
//カテゴリ
enum class ItemCategory 
{
    MEDICINE,
    WEAPON,
    BLESSING,
    TREASURE,
    TOOL,
};
//薬
enum class MedicineType 
{
    HP,
    HP_UP,
    STAMINA_UP,
    SPEED_UP,
    ALL,
};
//武器
enum class WeaponType 
{
    SWORD,
    BOW,
    STAFF,
};
//祝福
enum class BlessingType 
{
	HP_UP,
	STAMINA_UP,
	SPEED_UP,
};
//宝
enum class TreasureType
{
    CROSS,
    GEM1,
    GEM2,
    GEM3,
	STAR,
	TEAPOT,
};
//道具
enum class ToolType 
{
    NONE = -1,
    LADDER,
    BAR,
	HAMMER,
	SAW,
	DRIVER,
    DOOR,
};
