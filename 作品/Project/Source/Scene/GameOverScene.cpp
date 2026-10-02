#include "GameOverScene.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "../Input/Input.h"
#include "../Sound/SoundManager.h"
#include "../UI/Message/MessageManager.h"
#include <vector>
#include "ScreenSize.h"
#include "../Color/Color.h"

GameOverScene::GameOverScene() : SceneBase()
	, m_GameOverHandle(-1) 
	, retryBtm(nullptr)
	, exitBtm(nullptr)
{
}

GameOverScene::~GameOverScene()
{
	// nullptrチェック
	if (retryBtm)
	{
		delete retryBtm;
		retryBtm = nullptr;
	}
	if (exitBtm)
	{
		delete exitBtm;
		exitBtm = nullptr;
	}
}

void GameOverScene::Init()
{
	// 画像を読み込む（LoadImgButton 内で LoadGraph を呼ぶ）
	img = IconButton::LoadImgButton();

	// ボタン位置・サイズを適宜調整
	retryBtm = new IconButton(250, 750, 480, 122, img.sNormal2, img.sHover2, img.sPush2);
	exitBtm = new IconButton(900, 750, 480, 122, img.sNormal, img.sHover, img.sPush);

	// 初期選択
	SceneManager::GetInstance()->m_SelectButton = 0;

	// ボタンに選択状態を反映して即時描画できるようにする
	if (retryBtm) retryBtm->SetSelected(true);
	if (exitBtm)  exitBtm->SetSelected(false);
}

void GameOverScene::Load()
{
	m_GameOverHandle = LoadGraph("Data/Background/GameOver.png");
}

void GameOverScene::Start()
{
	// マウスカーソルを表示する
	SetMouseDispFlag(TRUE);
	// BGM再生
	SoundManager::GetInstance()->PlayBGM(BGM_TYPE_LOSE);

	// 選択初期化（念のため）
	SceneManager::GetInstance()->m_SelectButton = 0;

	// Start 時にも selected を確実に反映
	if (retryBtm) retryBtm->SetSelected(true);
	if (exitBtm)  exitBtm->SetSelected(false);

	// ボタンメッセージ表示
	MessageManager::GetInstance()->SetMessage(250, 780, BLACK, UI_LARGE, "  リトライ         ゲーム終了", -1);

}

void GameOverScene::Step()
{
	SceneManager* sceneMgr = SceneManager::GetInstance();

	// 左右で選択移動
	if (Input::IsTriggerPadButton(PAD_INPUT_LEFT))
	{
		sceneMgr->m_SelectButton--;
		if (sceneMgr->m_SelectButton < 0) sceneMgr->m_SelectButton = 1;
	}
	if (Input::IsTriggerPadButton(PAD_INPUT_RIGHT))
	{
		sceneMgr->m_SelectButton++;
		if (sceneMgr->m_SelectButton > 1) sceneMgr->m_SelectButton = 0;
	}

	// A ボタンで決定（コントローラ）
	if (Input::IsTriggerPadButton(PAD_INPUT_A))
	{
		if (sceneMgr->m_SelectButton == 0)
		{
			MessageManager::GetInstance()->ClearMessage();
			SceneManager::GetInstance()->ChangeScene(LOADING);
		}
		else // ==1
		{
			SceneManager::GetInstance()->SetEndFlag(true);
			return;
		}
	}
}

void GameOverScene::Update()
{
	// IconButton の更新
	if (retryBtm) retryBtm->Update();
	if (exitBtm)  exitBtm->Update();

	// コントローラ選択を反映して視覚状態を切り替える
	SceneManager* sceneMgr = SceneManager::GetInstance();
	if (retryBtm) retryBtm->SetSelected(sceneMgr->m_SelectButton == 0);
	if (exitBtm)  exitBtm->SetSelected(sceneMgr->m_SelectButton == 1);

	// マウスでクリックされたら即座に反応させる
	if (retryBtm && retryBtm->IsClicked())
	{
		SceneManager::GetInstance()->ChangeScene(LOADING);
	}
	if (exitBtm && exitBtm->IsClicked())
	{
		SceneManager::GetInstance()->SetEndFlag(true);
	}
}

void GameOverScene::Draw()
{
	// 背景（失敗時は塗りつぶしして警告を出す）
	if (m_GameOverHandle > 0)
	{
		DrawGraph(-200, -80, m_GameOverHandle, TRUE);
	}
	else
	{
		// 背景画像がなければ黒で塗る
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		DrawFormatString(10, 10, GetColor(255, 0, 0), "Warning: Title background not loaded");
	}

	// IconButton の描画
	if (retryBtm) retryBtm->Draw();
	if (exitBtm)  exitBtm->Draw();
}

void GameOverScene::Fin()
{
	// 画像リソースを解放
	if (m_GameOverHandle != 0)
	{
		DeleteGraph(m_GameOverHandle);
		m_GameOverHandle = 0;
	}

	// IconButton と画像ハンドルを解放
	if (retryBtm)
	{
		delete retryBtm;
		retryBtm = nullptr;
	}
	if (exitBtm)
	{
		delete exitBtm;
		exitBtm = nullptr;
	}
	// LoadImgButton で読み込んだグラフィックハンドルを解放
	if (img.sNormal >= 0)  DeleteGraph(img.sNormal);
	if (img.sHover >= 0)  DeleteGraph(img.sHover);
	if (img.sPush >= 0)  DeleteGraph(img.sPush);

	if (img.sNormal2 >= 0) DeleteGraph(img.sNormal2);
	if (img.sHover2 >= 0) DeleteGraph(img.sHover2);
	if (img.sPush2 >= 0) DeleteGraph(img.sPush2);

	// リセット
	img = IconButton::ButtonImg();
}
