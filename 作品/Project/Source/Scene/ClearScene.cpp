#include "ClearScene.h"
#include "DxLib.h"
#include "SceneManager.h"
#include "../Input/Input.h"
#include "../Sound/SoundManager.h"
#include "../Button/Button.h"
#include <vector>
#include "ScreenSize.h"
#include "../Button/IconButton.h"
#include "../UI/Message/MessageManager.h"
#include "../UI/Scroll/ScrollManager.h"
#include "../Color/Color.h"

ClearScene::ClearScene() : SceneBase()
	, m_ClearHandle(-1)
	, titleBtm(nullptr)
	, exitBtm(nullptr)
{
}

ClearScene::~ClearScene()
{
	// nullptrチェック
	if (titleBtm)
	{
		delete titleBtm;
		titleBtm = nullptr;
	}
	if (exitBtm)
	{
		delete exitBtm;
		exitBtm = nullptr;
	}
}

void ClearScene::Init()
{
	// ScrollManager を生成して背景をロード、速度設定
	ScrollManager::CreateInstance();
	ScrollManager::GetInstance()->LoadBackground("Data/Background/TitleBack.png");
	ScrollManager::GetInstance()->SetSpeed(-0.5f, 0.0f);
	ScrollManager::GetInstance()->SetTile(true, false);
	ScrollManager::GetInstance()->SetActive(true);

	// 画像を読み込む（LoadImgButton 内で LoadGraph を呼ぶ）
	img = IconButton::LoadImgButton();

	// ボタン位置・サイズを適宜調整
	titleBtm = new IconButton(250, 750, 480, 122, img.sNormal2, img.sHover2, img.sPush2);
	exitBtm = new IconButton(900, 750, 480, 122, img.sNormal, img.sHover, img.sPush);

	// 初期選択
	SceneManager::GetInstance()->m_SelectButton = 0;

	// ボタンに選択状態を反映して即時描画できるようにする
	if (titleBtm) titleBtm->SetSelected(true);
	if (exitBtm)  exitBtm->SetSelected(false);
}

void ClearScene::Load()
{
	m_ClearHandle = LoadGraph("Data/Background/ClearMessage.png");
}

void ClearScene::Start()
{
	// マウスカーソルを表示する
	SetMouseDispFlag(TRUE);
	// BGM再生
	SoundManager::GetInstance()->PlayBGM(BGM_TYPE_CLEAR);

	// 選択初期化（念のため）
	SceneManager::GetInstance()->m_SelectButton = 0;

	// Start 時にも selected を確実に反映
	if (titleBtm) titleBtm->SetSelected(true);
	if (exitBtm)  exitBtm->SetSelected(false);

	// ボタンメッセージ表示
	MessageManager::GetInstance()->SetMessage(250, 780, BLACK, UI_LARGE, " タイトルへ        ゲーム終了", -1);
}

void ClearScene::Step()
{
	// シーンマネージャーのインスタンスを取得
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

	// 決定ボタン（Aボタン）で選択中の処理実行
	if (Input::IsTriggerPadButton(PAD_INPUT_A))
	{
		if (sceneMgr->m_SelectButton == 0)
		{
			SoundManager::GetInstance()->StopBGM(BGM_TYPE_CLEAR);
			SceneManager::GetInstance()->ChangeScene(TITLE);
		}
		else 
		{
			SceneManager::GetInstance()->SetEndFlag(true);
			return;
		}
	}
}

void ClearScene::Update()
{
	// ScrollManager の更新
	if (ScrollManager::GetInstance()) ScrollManager::GetInstance()->Update();
	// IconButton の更新
	if (titleBtm) titleBtm->Update();
	if (exitBtm)  exitBtm->Update();

	// コントローラ選択を反映して視覚状態を切り替える
	SceneManager* sceneMgr = SceneManager::GetInstance();
	if (titleBtm) titleBtm->SetSelected(sceneMgr->m_SelectButton == 0);
	if (exitBtm)  exitBtm->SetSelected(sceneMgr->m_SelectButton == 1);

	// マウスでクリックされたら即座に反応させる
	if (titleBtm && titleBtm->IsClicked())
	{
		SceneManager::GetInstance()->ChangeScene(TITLE);
	}
	if (exitBtm && exitBtm->IsClicked())
	{
		SceneManager::GetInstance()->SetEndFlag(true);
	}
}

void ClearScene::Draw()
{
	// 背景のスクロール描画を先に呼ぶ
	if (ScrollManager::GetInstance()) ScrollManager::GetInstance()->Draw();

	// 背景（失敗時は塗りつぶしして警告を出す）
	if (m_ClearHandle > 0)
	{
		DrawGraph(0, -90, m_ClearHandle, TRUE);
	}
	else
	{
		// 背景画像がなければ黒で塗る
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
	}

	// IconButton の描画
	if (titleBtm) titleBtm->Draw();
	if (exitBtm)  exitBtm->Draw();
}

void ClearScene::Fin()
{
	// 画像リソースを解放
	if (m_ClearHandle != 0)
	{
		DeleteGraph(m_ClearHandle);
		m_ClearHandle = 0;
	}

	// IconButton と画像ハンドルを解放
	if (titleBtm)
	{
		delete titleBtm;
		titleBtm = nullptr;
	}
	if (exitBtm)
	{
		delete exitBtm;
		exitBtm = nullptr;
	}

	// ScrollManager を解放
	ScrollManager::DeleteInstance();

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
