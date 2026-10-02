#pragma once

//BGM
enum BGMType
{
	BGM_TYPE_TITLE,
	BGM_TYPE_PLAY,
	BGM_TYPE_CLEAR,
	BGM_TYPE_LOSE,
	BGM_TYPE_SOUND_MAX,
	BGM_TYPE_SOUND_NONE = -1,
};

//SE
enum SEType
{
	SE_TYPE_PLAYER_DEAD,
	SE_TYPE_PLAYER_DAMAGE,
	SE_TYPE_PLAYER_SHOT,
	SE_TYPE_ENEMY_DAMAGE,
	SE_TYPE_ENEMY_DEAD,
	SE_TYPE_ENEMY_SHOT,
	SE_TYPE_TOOLENEMY_LADDER_ATTACK,
	SE_TYPE_TOOLENEMY_LADDER_DEAD,
	SE_TYPE_TOOLENEMY_HAMMER_ATTACK,
	SE_TYPE_TOOLENEMY_DRIVER_ATTACK,
	SE_TYPE_TOOLENEMY_DRIVER_DEAD,
	SE_TYPE_TOOLENEMY_SAW_ATTACK,
	SE_TYPE_ITEM_PICKUP,
	SE_TYPE_ITEM_MEDICINE_USE,
	SE_TYPE_ITEM_DELIVERY,
	SE_TYPE_GIMMICK_DOOR_OPEN,
	SE_TYPE_GIMMICK_HAMMER,
	SE_TYPE_GIMMICK_LADDER,
	SE_TYPE_UI_INVENTORY_SELECT,
	SE_TYPE_UI_INVENTORY_OPEN,
	SE_TYPE_SCENE_CANGOAL,
	SE_TYPE_SCENE_GOAL,
	SE_TYPE_NONE,
	SE_TYPE_SOUND_MAX,
};

// サウンドオブジェクト管理クラス
class SoundManager
{
public:
	SoundManager();	// コンストラクタ
	~SoundManager();	// デストラクタ

public:
	// サウンドマネージャーを生成する
	static void CreateInstance() { if (!m_Instance) m_Instance = new SoundManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static SoundManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Load();	// ロード
	void Fin();		// 終了

public:
	// かつての CreateSound は不要（Main.cpp から呼ばれているため互換で残す）
	void CreateSound();

	//BGMの再生
	void PlayBGM(int id);
	//BGMの停止
	void StopBGM(int id);
	//SEの再生(通常)
	void PlaySE(int id);
	//SEの再生(ヒットストップ)
	void PlaySEWait(int id);

private:
	// 生成されたSoundManager自身を格納する変数
	static SoundManager* m_Instance;

	// サウンドハンドル
	int m_BGMHandle[BGM_TYPE_SOUND_MAX];
	int m_SEHandle[SE_TYPE_SOUND_MAX];
};
