#include "PlayScene.h"
#include <vector>
#include "DxLib.h"
#include "../Color/Color.h"
#include "ScreenSize.h"
#include "SceneManager.h"
#include "../Input/Input.h"
#include "../Effect/EffectManager.h"
#include "../Camera/CameraManager.h"
#include "../Player/PlayerManager.h"
#include "../Collision/CollisionManager.h"
#include "../Enemy/EnemyManager.h"
#include "../EnemyShot/EnemyShotManager.h"
#include "../PlayerShot/PlayerShotManager.h"
#include "../Goal/GoalManager.h"
#include "../Sound/SoundManager.h"
#include "../Animation/AnimationManager.h"
#include "../Button/Button.h"
#include "../Inventory/Inventory.h"
#include "../Item/ItemManager.h"
#include "../ShadowMap/ShadowMap.h"
#include "../Player/Player.h"
#include "../DeliveryBox/DeliveryBox.h"
#include "../Object/Gimmick/Tool/GimmickParamsLoader.h"
#include "../Object/Gimmick/Tool/GimmickManager.h"
#include "../Object/StageObject/StageObjectManager.h"
#include "../Stage/StageManager.h"
#include "../UI/Message/MessageManager.h"

// ポーズ画面のボタンを定義
std::vector<Button> Pausebuttons =
{
		Button(SCREEN_WIDTH / 2 - 150, 250, 180, 50, "ゲーム再開"),
		Button(SCREEN_WIDTH / 2 - 150, 600, 180, 50, "ゲーム終了"),
};

PlayScene::PlayScene() : SceneBase()
	, m_IgnoreInputTimer(0)
	, m_StatsFont(-1)
{
}

PlayScene::~PlayScene()
{
	Fin();
}

void PlayScene::Init()
{
	SetUseZBuffer3D(TRUE); // Zバッファ有効化
	SetWriteZBuffer3D(TRUE); // Zバッファ書き込み
}

void PlayScene::Load()
{
	m_StatsFont = MessageManager::GetInstance()->GetFontHandle(FontId::UI_PLAYER_STATS);
}

void PlayScene::Start()
{
	//それぞれを開始
	SetMouseDispFlag(FALSE); // マウスカーソルを非表示にする
	PlayerManager::GetInstance()->Start();
	CameraManager::GetInstance()->Start();
	EnemyManager::GetInstance()->Start();
	EnemyShotManager::GetInstance()->Start();
	PlayerShotManager::GetInstance()->Start();
	GoalManager::GetInstance()->Start();
	Inventory::GetInstance()->Start();
	GimmickManager::GetInstance()->Start();
	DeliveryBox::GetInstance()->Start();
	ItemManager::GetInstance()->Start();
	StageManager::GetInstance()->Start();
	StageObjectManager::GetInstance()->Start();
	SoundManager::GetInstance()->PlayBGM(BGM_TYPE_PLAY);
}

void PlayScene::Step()
{
	// 入力無視期間中
	if (m_IgnoreInputTimer > 0)
	{
		m_IgnoreInputTimer--;
		return;  // プレイヤー操作などはしない
	}

	// ポーズ画面のON/OFFを切り替える
	if (Input::IsTriggerPadButton(PAD_INPUT_R))//STARTボタンでポーズ画面を開く
	{
		if (SceneManager::GetInstance()->m_IsPaused == false)
		{
			SceneManager::GetInstance()->SetPauseFlag(true); // ポーズフラグを切り替える
			Inventory::GetInstance()->CloseInventory(); // インベントリを閉じる
			SoundManager::GetInstance()->PlaySE(SE_TYPE_UI_INVENTORY_OPEN); // ポーズ音
		}
		else
		{
			EndPause(); // ポーズ終了
		}
	}

	if (SceneManager::GetInstance()->m_IsPaused)
	{
		static int selectWait = 0;
		if (selectWait > 0)
		{
			selectWait--;
		}
		// 十字キー or スティック上下入力で選択移動
		int pad = GetJoypadInputState(DX_INPUT_PAD1);

		if (selectWait == 0)
		{
			if (pad & PAD_INPUT_DOWN)
			{
				SceneManager::GetInstance()->m_SelectButton++;
				if (SceneManager::GetInstance()->m_SelectButton >= Pausebuttons.size())
				{
					SceneManager::GetInstance()->m_SelectButton = 0;
				}	
				selectWait = 10;
			}
			else if (pad & PAD_INPUT_UP)
			{
				SceneManager::GetInstance()->m_SelectButton--;
				if (SceneManager::GetInstance()->m_SelectButton < 0)
				{
					SceneManager::GetInstance()->m_SelectButton = (int)Pausebuttons.size() - 1;
				}
				selectWait = 10;
			}
		}

		// 決定ボタン（Aボタン）押下時
		if (Input::IsTriggerPadButton(PAD_INPUT_A))
		{
			int select = SceneManager::GetInstance()->m_SelectButton;
			if (select == 0)
			{
				EndPause(); // 「ゲーム再開」
			}
			else if (select == 1)
			{
				SceneManager::GetInstance()->SetEndFlag(true); // 「ゲーム終了」
			}
		}

		// キャンセルボタン（Bボタン）押下時 → ポーズ解除
		if (Input::IsTriggerPadButton(PAD_INPUT_B))
		{
			EndPause();
		}
		//終了ボタンでゲーム終了
		if (Pausebuttons[1].IsClicked())
		{
			//※DxLib_End()はコンセントを引っこ抜くのと同じようなものなので呼ばない
			SceneManager::GetInstance()->SetEndFlag(true);
			return;
		}
	}
	
	// ポーズ処理中はカメラ操作しない
	if (!SceneManager::GetInstance()->m_IsPaused)
	{
		// カメラステップ
		CameraManager::GetInstance()->Step();
		//インベントリステップ
		Inventory::GetInstance()->Step();
		//エフェクトステップ
		EffectManager::GetInstance()->Step();
	}
	// プレイヤーステップ
	PlayerManager::GetInstance()->Step();
	//エネミーステップ
	EnemyManager::GetInstance()->Step();
	//エネミーショットステップ
	EnemyShotManager::GetInstance()->Step();
	// プレイヤーショットステップ
	PlayerShotManager::GetInstance()->Step();
	// ギミックステップ
	GimmickManager::GetInstance()->Step();
	// ゴールステップ
	GoalManager::GetInstance()->Step();
}

void PlayScene::Update()
{
	// ステージオブジェクト更新
	StageObjectManager::GetInstance()->Update();
	// プレイヤー更新
	PlayerManager::GetInstance()->Update();
	// カメラアップデート
	CameraManager::GetInstance()->Update();
	// エフェクト更新
	EffectManager::GetInstance()->Update();
	// エネミーステップ
	EnemyManager::GetInstance()->Update();
	// エネミーショットアップデート
	EnemyShotManager::GetInstance()->Update();
	// プレイヤーショットアップデート
	PlayerShotManager::GetInstance()->Update();
	// アイテム更新
	ItemManager::GetInstance()->Update();
	// ギミック更新
	GimmickManager::GetInstance()->Update();
	// 納品ボックス更新
	DeliveryBox::GetInstance()->Update();
	// ゴール更新
	GoalManager::GetInstance()->Update();
	// 当たり判定更新
	CollisionManager::GetInstance()->CheckCollision();
	// ポーズ中の処理
	if (SceneManager::GetInstance()->m_IsPaused)
	{
		for (auto& btn : Pausebuttons)
		{
			btn.Update();
		}
	}
}

void PlayScene::Draw()
{
	// シャドウマップに描画された影を床に映しこむ
	ShadowMap::GetInstance()->StartAppearsShadowMap();
	// ステージオブジェクト描画
	StageObjectManager::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndAppearsShadowMap();

	// プレイヤーの影をシャドウマップに描画する
	ShadowMap::GetInstance()->StartDrawShadowMap();
	PlayerManager::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndDrawShadowMap();
	// プレイヤー描画
	PlayerManager::GetInstance()->Draw();
	// シャドウマップに描画された影を床に映しこむ
	ShadowMap::GetInstance()->StartAppearsShadowMap();
	//納品ボックス描画
	DeliveryBox::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndAppearsShadowMap();
	// カメラ描画
	CameraManager::GetInstance()->Draw();
	// エフェクト描画
	EffectManager::GetInstance()->Draw();
	//エネミー描画
	EnemyManager::GetInstance()->Draw();
	//エネミーショット描画
	EnemyShotManager::GetInstance()->Draw();
	// プレイヤーショット描画
	PlayerShotManager::GetInstance()->Draw();
	// シャドウマップに描画された影を床に映しこむ
	ShadowMap::GetInstance()->StartAppearsShadowMap();
	// ギミック描画
	GimmickManager::GetInstance()->Draw();
	// アイテム描画
	ItemManager::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndAppearsShadowMap();
	// ゴール描画
	GoalManager::GetInstance()->Draw();
	// 入力描画
	Input::Draw();
	// 当たり判定描画
	CollisionManager::GetInstance()->Draw();
	// インベントリ描画
	Inventory::GetInstance()->Draw();
	// ポーズ中の処理
	if (SceneManager::GetInstance()->m_IsPaused)
	{
		// 半透明の黒い背景 (アルファ値128 = 半透明)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ブレンドを戻す
		DrawBox(SCREEN_WIDTH/2 - 300, 0, SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT, BLACK, TRUE);

		for (size_t i = 0; i < Pausebuttons.size(); ++i)
		{
			bool isSelected = (i == SceneManager::GetInstance()->m_SelectButton);
			Pausebuttons[i].Draw(isSelected);
		}

		DrawFormatString(SCREEN_WIDTH - 750, 865, WHITE, "A:決定  B:戻る");

		// 操作説明を描画する
		DrawFormatStringToHandle(1170, 500, RED, m_StatsFont, "!ゲームルール!");
		DrawFormatStringToHandle(1170, 530, WHITE, m_StatsFont, "1.探索しながらお宝を集める");
		DrawFormatStringToHandle(1170, 560, WHITE, m_StatsFont, "2.スタート地点の\n  納品ボックスに納品");
		DrawFormatStringToHandle(1170, 620, WHITE, m_StatsFont, "3.ノルマまで稼ぐ");
		DrawFormatStringToHandle(1170, 650, WHITE, m_StatsFont, "4.ゴールを探す");
		//DebugDrawJoypadButtons(); // デバッグ用のジョイパッドボタン表示
	}
	// プレイヤーUI描画
	if (PlayerManager::GetInstance())
	{
		if (Player* p = PlayerManager::GetInstance()->GetPlayer())
		{
			p->DrawUI();
		}
	}
}

void PlayScene::Fin()
{
	// プレイヤーマネージャー削除
	PlayerManager::DeleteInstance();

	// カメラマネージャー削除
	CameraManager::DeleteInstance();

	//エネミーマネージャー削除
	EnemyManager::DeleteInstance();

	//エネミーショットマネージャー削除
	EnemyShotManager::DeleteInstance();

	// プレイヤーショットマネージャー削除
	PlayerShotManager::DeleteInstance();

	// ゴールマネージャー削除
	GoalManager::DeleteInstance();

	// アニメーションマネージャー削除
	AnimationManager::DeleteInstance();

	// ステージオブジェクトマネージャー削除
	StageObjectManager::DeleteInstance();

	// インベントリ削除
	Inventory::DeleteInstance();

	// アイテムマネージャー削除
	ItemManager::DeleteInstance();

	// ギミックマネージャー削除
	GimmickManager::DeleteInstance();

	// デリバリーボックス削除
	DeliveryBox::DeleteInstance();

	// コリジョンマネージャー削除
	CollisionManager::DeleteInstance();

	// インベントリ削除
	Inventory::DeleteInstance();

	SetUseZBuffer3D(FALSE); // Zバッファ無効化
	SetWriteZBuffer3D(FALSE);// Zバッファ書き込み無効化

	// シャドウマップ削除
	ShadowMap::DeleteInstance();
}

void PlayScene::EndPause()
{
	SceneManager::GetInstance()->SetPauseFlag(false); // ポーズフラグを切り替える
	// ポーズ中の選択位置をリセット
	SceneManager::GetInstance()->m_SelectButton = 0;
	// マウスカーソルを非表示にする
	SetMouseDispFlag(FALSE);
}

void PlayScene::DebugDrawJoypadButtons()
{
	int padState = GetJoypadInputState(DX_INPUT_PAD1);
	int y = 0;

	if (padState & PAD_INPUT_A)      DrawString(0, y += 20, "A ボタン", WHITE);
	if (padState & PAD_INPUT_B)      DrawString(0, y += 20, "B ボタン", WHITE);
	if (padState & PAD_INPUT_X)      DrawString(0, y += 20, "X ボタン", WHITE);
	if (padState & PAD_INPUT_Y)      DrawString(0, y += 20, "Y ボタン", WHITE);
	if (padState & PAD_INPUT_L)      DrawString(0, y += 20, "L ボタン (LB)", WHITE);
	if (padState & PAD_INPUT_R)      DrawString(0, y += 20, "R ボタン (RB)", WHITE);
	if (padState & PAD_INPUT_START)  DrawString(0, y += 20, "START ボタン", WHITE);
	if (padState & PAD_INPUT_UP)     DrawString(0, y += 20, "↑", WHITE);
	if (padState & PAD_INPUT_DOWN)   DrawString(0, y += 20, "↓", WHITE);
	if (padState & PAD_INPUT_LEFT)   DrawString(0, y += 20, "←", WHITE);
	if (padState & PAD_INPUT_RIGHT)  DrawString(0, y += 20, "→", WHITE);	
}				   
