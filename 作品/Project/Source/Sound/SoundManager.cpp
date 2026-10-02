#include "SoundManager.h"
#include "DxLib.h"

// 静的変数の初期化
SoundManager* SoundManager::m_Instance = nullptr;

// コンストラクタ
SoundManager::SoundManager()
	:m_BGMHandle{}
	,m_SEHandle{}
{
}

// デストラクタ
SoundManager::~SoundManager()
{
	Fin();
}

void SoundManager::CreateSound()
{
}

void SoundManager::PlayBGM(int id)
{
	if (id >= 0 && id < BGM_TYPE_SOUND_MAX && m_BGMHandle[id] >= 0)
	{
		PlaySoundMem(m_BGMHandle[id], DX_PLAYTYPE_LOOP, true);
	}
}

void SoundManager::StopBGM(int id)
{
	if (id >= 0 && id < BGM_TYPE_SOUND_MAX && m_BGMHandle[id] >= 0)
	{
		StopSoundMem(m_BGMHandle[id],false);
	}
}

void SoundManager::PlaySE(int id)
{
	if (id >= 0 && id < SE_TYPE_SOUND_MAX && m_SEHandle[id] >= 0)
	{
		PlaySoundMem(m_SEHandle[id], DX_PLAYTYPE_BACK, true);
	}
}

void SoundManager::PlaySEWait(int id)
{
	if (id >= 0 && id < SE_TYPE_SOUND_MAX && m_SEHandle[id] >= 0)
	{
		PlaySoundMem(m_SEHandle[id], DX_PLAYTYPE_NORMAL, true);
	}
}

void SoundManager::Load()
{
	// SEをロードする
	m_SEHandle[SE_TYPE_PLAYER_DEAD] = LoadSoundMem("Data/Sound/SE/Player/Player_Dead.mp3");
	m_SEHandle[SE_TYPE_PLAYER_DAMAGE] = LoadSoundMem("Data/Sound/SE/Player/Player_Damage.mp3");
	m_SEHandle[SE_TYPE_PLAYER_SHOT] = LoadSoundMem("Data/Sound/SE/Player/Player_Shot.mp3");
	m_SEHandle[SE_TYPE_ENEMY_DAMAGE] = LoadSoundMem("Data/Sound/SE/Enemy/Enemy_Damage.mp3");
	m_SEHandle[SE_TYPE_ENEMY_DEAD] = LoadSoundMem("Data/Sound/SE/Enemy/Enemy_Dead.mp3");
	m_SEHandle[SE_TYPE_ENEMY_SHOT] = LoadSoundMem("Data/Sound/SE/Enemy/Enemy_Shot.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_LADDER_ATTACK] = LoadSoundMem("Data/Sound/SE/Enemy/梯子/梯子攻撃.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_LADDER_DEAD] = LoadSoundMem("Data/Sound/SE/Enemy/梯子/梯子死亡.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_HAMMER_ATTACK] = LoadSoundMem("Data/Sound/SE/Enemy/ハンマー/衝撃波.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_DRIVER_ATTACK] = LoadSoundMem("Data/Sound/SE/Enemy/ドライバー/ドライバー攻撃.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_DRIVER_DEAD] = LoadSoundMem("Data/Sound/SE/Enemy/ドライバー/ドライバー死亡.mp3");
	m_SEHandle[SE_TYPE_TOOLENEMY_SAW_ATTACK] = LoadSoundMem("Data/Sound/SE/Enemy/のこぎり/のこぎり攻撃.mp3");
	m_SEHandle[SE_TYPE_ITEM_PICKUP] = LoadSoundMem("Data/Sound/SE/Item/Item_Get.mp3");
	m_SEHandle[SE_TYPE_ITEM_MEDICINE_USE] = LoadSoundMem("Data/Sound/SE/Item/Medicine/Use_Medicine.mp3");
	m_SEHandle[SE_TYPE_ITEM_DELIVERY] = LoadSoundMem("Data/Sound/SE/Item/Delivery/Delivery.mp3");
	m_SEHandle[SE_TYPE_GIMMICK_DOOR_OPEN] = LoadSoundMem("Data/Sound/SE/Object/ToolGimmick/Door_Open.ogg");
	m_SEHandle[SE_TYPE_GIMMICK_HAMMER] = LoadSoundMem("Data/Sound/SE/Object/ToolGimmick/Hammer_Used.mp3");
	m_SEHandle[SE_TYPE_GIMMICK_LADDER] = LoadSoundMem("Data/Sound/SE/Object/ToolGimmick/Ladder_Used.mp3");
	m_SEHandle[SE_TYPE_UI_INVENTORY_SELECT] = LoadSoundMem("Data/Sound/SE/UI/Inventory_Select.mp3");
	m_SEHandle[SE_TYPE_UI_INVENTORY_OPEN] = LoadSoundMem("Data/Sound/SE/UI/Inventory_Open.mp3");
	m_SEHandle[SE_TYPE_SCENE_GOAL] = LoadSoundMem("Data/Sound/SE/Scene/Goal.mp3");
	m_SEHandle[SE_TYPE_SCENE_CANGOAL] = LoadSoundMem("Data/Sound/SE/Scene/Cangoal.mp3");
	// BGMをロードする
	m_BGMHandle[BGM_TYPE_TITLE] = LoadSoundMem("Data/Sound/BGM/Title.mp3");
	m_BGMHandle[BGM_TYPE_PLAY] = LoadSoundMem("Data/Sound/BGM/Stage1.mp3");
	m_BGMHandle[BGM_TYPE_LOSE] = LoadSoundMem("Data/Sound/BGM/Lose.mp3");
	m_BGMHandle[BGM_TYPE_CLEAR] = LoadSoundMem("Data/Sound/BGM/Clear.mp3");

	//音量調整
	ChangeVolumeSoundMem(255 * 50 / 100, m_BGMHandle[BGM_TYPE_TITLE]);
	ChangeVolumeSoundMem(255 * 50 / 100, m_BGMHandle[BGM_TYPE_CLEAR]);
	ChangeVolumeSoundMem(255 * 70 / 100, m_BGMHandle[BGM_TYPE_LOSE]);
	ChangeVolumeSoundMem(255 * 30 / 100, m_BGMHandle[BGM_TYPE_PLAY]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_PLAYER_DEAD]);
	ChangeVolumeSoundMem(255 * 80 / 100, m_SEHandle[SE_TYPE_PLAYER_DAMAGE]);
	ChangeVolumeSoundMem(255 * 70 / 100, m_SEHandle[SE_TYPE_PLAYER_SHOT]);
	ChangeVolumeSoundMem(255 * 50 / 100, m_SEHandle[SE_TYPE_ENEMY_DAMAGE]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_ENEMY_DEAD]);
	ChangeVolumeSoundMem(255 * 70 / 100, m_SEHandle[SE_TYPE_ENEMY_SHOT]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_LADDER_ATTACK]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_LADDER_DEAD]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_HAMMER_ATTACK]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_DRIVER_ATTACK]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_DRIVER_DEAD]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_TOOLENEMY_SAW_ATTACK]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_ITEM_PICKUP]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_ITEM_MEDICINE_USE]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_ITEM_DELIVERY]);
	ChangeVolumeSoundMem(255 * 50 / 100, m_SEHandle[SE_TYPE_GIMMICK_DOOR_OPEN]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_GIMMICK_HAMMER]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_GIMMICK_LADDER]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_UI_INVENTORY_SELECT]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_UI_INVENTORY_OPEN]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_SCENE_GOAL]);
	ChangeVolumeSoundMem(255 * 60 / 100, m_SEHandle[SE_TYPE_SCENE_CANGOAL]);
}

void SoundManager::Fin()
{
	// BGM ハンドルを解放
	for (int i = 0; i < BGM_TYPE_SOUND_MAX; ++i)
	{
		if (m_BGMHandle[i] >= 0)
		{
			StopSoundMem(m_BGMHandle[i]);
			DeleteSoundMem(m_BGMHandle[i]);
			m_BGMHandle[i] = -1;
		}
	}
	// SE ハンドルを解放
	for (int i = 0; i < SE_TYPE_SOUND_MAX; ++i)
	{
		if (m_SEHandle[i] >= 0)
		{
			StopSoundMem(m_SEHandle[i]);
			DeleteSoundMem(m_SEHandle[i]);
			m_SEHandle[i] = -1;
		}
	}
}
