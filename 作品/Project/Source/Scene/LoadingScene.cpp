#include "LoadingScene.h"
#include "DxLib.h"
#include "SceneManager.h"
#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>
#include "../Color/Color.h"
#include "ScreenSize.h"
#include "../Input/Input.h"
#include "../Camera/CameraManager.h"
#include "../Player/PlayerManager.h"
#include "../Collision/CollisionManager.h"
#include "../Enemy/EnemyManager.h"
#include "../EnemyShot/EnemyShotManager.h"
#include "../PlayerShot/PlayerShotManager.h"
#include "../Goal/GoalManager.h"
#include "../Sound/SoundManager.h"
#include "../Effect/EffectManager.h"
#include "../Animation/AnimationManager.h"
#include "../Button/Button.h"
#include "../Inventory/Inventory.h"
#include "../Item/ItemManager.h"
#include "../ShadowMap/ShadowMap.h"
#include "../Player/Player.h"
#include "../DeliveryBox/DeliveryBox.h"
#include "../Enemy/EnemyParamsLoader.h"
#include "../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../Object/Gimmick/Tool/GimmickManager.h"
#include "../Object/StageObject/StageObjectManager.h"
#include "../Stage/StageManager.h"
#include "../UI/Message/MessageManager.h"

struct LoadTask
{
	const char* label;          // 表示用
	std::function<void()> func; // 実処理
};
static std::vector<LoadTask> s_loadTasks;
static size_t s_taskIndex = 0;
static const char* s_Tips[] =
{
	"TIPS: 薬は拾ったらすぐに使うといい",
	"TIPS: 道具は一個ずつしか持ち運べない",
	"TIPS: インベントリは5個でMAX",
	"TIPS: 道具はギミックの数だけある",
	"TIPS: 時には道を戻ってみては？",
};
static const int TIP_COUNT = sizeof(s_Tips) / sizeof(s_Tips[0]);

int tipFontHandle = -1;
int pressFontHandle = -1;

LoadingScene::LoadingScene() : SceneBase()
	, m_BarAnim(0.0f)  
	, m_PressAnim(0.0f)  
	, m_CurrentTip(0)  
	, m_LoadProgress(0)  
	, m_IsLoaded(false)  
	, m_FrameCounter(0)  
	, m_CurrentLoadingText("")  
{
}

LoadingScene::~LoadingScene() {}

void LoadingScene::Init()
{
	// フォントハンドル取得
	tipFontHandle = MessageManager::GetInstance()->GetFontHandle(FontId::UI_TIP);
	pressFontHandle = MessageManager::GetInstance()->GetFontHandle(FontId::UI_PRESS);

	// メンバ変数初期化
	m_LoadProgress = 0;
	m_IsLoaded = false;
	m_FrameCounter = 0;
	m_BarAnim = 0.0f;
	m_PressAnim = 0.0f;

	// ランダムの初期化
	srand((unsigned int)time(nullptr));
	m_CurrentTip = rand() % TIP_COUNT;
	
	s_loadTasks.clear();
	s_taskIndex = 0;

	// 登録順序は依存関係に注意して決める
	s_loadTasks.push_back({
		"敵データ読み込み中...",
		[]() {
			EnemyParamsLoader::LoadAllEnemyParams("Data/Enemy/Enemies.json");
		}
		});

	s_loadTasks.push_back({
		"ステージデータ読み込み中...",
		[]() {
			GimmickParamsLoader::LoadAllGimmickParams("Data/Object/Gimmick/Tool/ToolGimmick_Stage1.json");
		}
		});

	s_loadTasks.push_back({
		"アニメーション初期化中...",
		[]() {
			AnimationManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"コリジョン初期化中...",
		[]() {
			CollisionManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"プレイヤー初期化中...",
		[]() {
			PlayerManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"プレイヤー生成中...",
		[]() {
			PlayerManager::GetInstance()->CreatePlayer();
		}
		});

	s_loadTasks.push_back({
		"プレイヤー初期化中...",
		[]() {
			PlayerManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"シャドウマップを生成中...",
		[]() {
			ShadowMap::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"シャドウマップを初期化中...",
		[]() {
			ShadowMap::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"カメラ初期化中...",
		[]() {
			CameraManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"カメラ生成中...",
		[]() {
			CameraManager::GetInstance()->CreateCamera(CAMERA);
			CameraManager::GetInstance()->CreateCamera(DEBUG_CAMERA);
		}
		});

	s_loadTasks.push_back({
		"カメラ初期化中...",
		[]() {
			CameraManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"敵初期化中...",
		[]() {
			EnemyManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"敵初期化中...",
		[]() {
			EnemyManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"敵弾初期化中...",
		[]() {
			EnemyShotManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"敵弾初期化中...",
		[]() {
			EnemyShotManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"プレイヤー弾初期化中...",
		[]() {
			PlayerShotManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"プレイヤー弾初期化中...",
		[]() {
			PlayerShotManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"ゴール初期化中...",
		[]() {
			GoalManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"ゴール生成中...",
		[]() {
			GoalManager::GetInstance()->CreateGoal();
			GoalManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"ステージオブジェクト初期化中...",
		[]() {
			StageObjectManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"ステージオブジェクト初期化中...",
		[]() {
			StageObjectManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"インベントリ初期化中...",
		[]() {
			Inventory::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"インベントリ初期化中...",
		[]() {
			Inventory::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"アイテム初期化中...",
		[]() {
			ItemManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"アイテム初期化中...",
		[]() {
			ItemManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"ギミック初期化中...",
		[]() {
			GimmickManager::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"ギミック初期化中...",
		[]() {
			GimmickManager::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"デリバリーボックス初期化中...",
		[]() {
			DeliveryBox::CreateInstance();
		}
		});

	s_loadTasks.push_back({
		"デリバリーボックス初期化中...",
		[]() {
			DeliveryBox::GetInstance()->Init();
		}
		});

	s_loadTasks.push_back({
		"ステージ初期化中...",
		[]() {
			StageManager::CreateInstance();
		}
		});

	// Load 系（リソース読み込み）
	s_loadTasks.push_back({
		"プレイヤー情報読み込み中...",
		[]() {
			PlayerManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"カメラ情報読み込み中...",
		[]() {
			CameraManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"エネミー情報読み込み中...",
		[]() {
			EnemyManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
	"エネミー弾情報読み込み中...",
	[]() {
		EnemyShotManager::GetInstance()->Load();
	}
		});

	s_loadTasks.push_back({
		"プレイヤー弾情報読み込み中...",
		[]() {
			PlayerShotManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"ゴール情報読み込み中...",
		[]() {
			GoalManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"ステージオブジェクト読み込み中...",
		[]() {
			StageObjectManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"インベントリ情報読み込み中...",
		[]() {
			Inventory::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"ギミック情報読み込み中...",
		[]() {
			GimmickManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"デリバリーボックス情報読み込み中...",
		[]() {
			DeliveryBox::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"アイテム情報読み込み中...",
		[]() {
			ItemManager::GetInstance()->Load();
		}
		});

	s_loadTasks.push_back({
		"ステージ情報読み込み中...",
		[]() {
			StageManager::GetInstance()->Load("Data/Stage/Stage1.json");
		}
		});

}

void LoadingScene::Load()
{
}

void LoadingScene::Start()
{

}

void LoadingScene::Step()
{
}

void LoadingScene::Update()
{
	m_BarAnim += 0.1f;

    if (!m_IsLoaded)
    {
        // 毎フレーム（あるいは数フレーム毎）に 1 タスクずつ実行する
        if (s_taskIndex < s_loadTasks.size())
        {
			m_CurrentLoadingText = s_loadTasks[s_taskIndex].label;
			s_loadTasks[s_taskIndex].func();
            ++s_taskIndex;
            // 進捗更新（タスク数比で算出）
            m_LoadProgress = static_cast<int>((s_taskIndex * 100) / s_loadTasks.size());
            
            // 大きな文字を表示
            {
                char buf[64];
                snprintf(buf, sizeof(buf), "Loading... %d%%", m_LoadProgress);
                if (MessageManager::GetInstance())
                {
                    MessageManager::GetInstance()->ShowMessage(690, SCREEN_HEIGHT / 2 - 50, WHITE, UI_NORMAL, buf, 250);
                }
            }
        }
        else
        {
            m_IsLoaded = true;
            m_LoadProgress = 100;
        }

        // デバッグ用にフレームカウンタを更新
        ++m_FrameCounter;
    }
    else
    {
        // ロードが終わってから A でゲーム開始に進める
        if (Input::IsTriggerPadButton(PAD_INPUT_A))
        {
			// BGM 停止（タイトルで鳴っている場合）
			SoundManager::GetInstance()->StopBGM(BGM_TYPE_TITLE);
			SoundManager::GetInstance()->StopBGM(BGM_TYPE_LOSE);
			// 遷移前にメッセージを即時消す
			if (MessageManager::GetInstance())
			{
				MessageManager::GetInstance()->ClearMessage();
			}
			SceneManager::GetInstance()->ChangeScene(PLAY);// PLAY シーンへ移行
        }
    }
	// ロード完了後のアニメーション更新
	if (m_IsLoaded)
	{
		m_PressAnim += 0.1f;
	}

	// チップ更新
	static int tipTimer = 0;
	tipTimer++;

	if (tipTimer > 300) // 5秒（60fps）
	{
		m_CurrentTip = rand() % TIP_COUNT;
		tipTimer = 0;
	}
}

void LoadingScene::Draw()
{
	// 背景
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK, TRUE);

	// アニメーション量
	int glow = (int)((sinf(m_BarAnim) + 1.0f) * 0.5f * 6);
	// 進捗
	int barWidth = (int)(BAR_W * (m_LoadProgress / 100.0f));

	// 中身
	DrawBox(BAR_X - glow, BAR_Y - glow, BAR_X + barWidth + glow, BAR_Y + BAR_H + glow, DARKLIME, TRUE);
	// 枠
	DrawBox(BAR_X - glow, BAR_Y - glow, BAR_X + BAR_W + glow, BAR_Y + BAR_H + glow, WHITE, FALSE);

	// チップ表示
	DrawStringToHandle(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 150, s_Tips[m_CurrentTip], WHITE, tipFontHandle);

	// ロード完了後のメッセージ
	if (m_IsLoaded)
	{
		int alpha = (int)((sinf(m_BarAnim) + 2.0f) * 0.5f * 255);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawStringToHandle(650, 470, "Press A to Start", YELLOW, pressFontHandle);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// ロード中のメッセージ
	if (!m_IsLoaded)
	{
		// 現在の読み込みテキスト表示
		DrawStringToHandle(SCREEN_WIDTH / 2 - 200, BAR_Y - 40, m_CurrentLoadingText, GRAY, tipFontHandle);
	}

}

void LoadingScene::Fin()
{
}
