#include "DxLib.h"
#include "Player/PlayerManager.h"
#include "Camera/CameraManager.h"
#include "Goal/GoalManager.h"
#include "Collision/CollisionManager.h"
#include "Enemy/EnemyManager.h"
#include "Input/Input.h"
#include "FPS/FPS.h"
#include "MyMath/MyMath.h"
#include "Scene/SceneManager.h"
#include "Sound/SoundManager.h"
#include "UI/Message/MessageManager.h"
#include "Color/Color.h"
#include "Effect/EffectManager.h"
#include "Scene/ScreenSize.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// ウィンドウモードON
	ChangeWindowMode(TRUE);

	SetZBufferBitDepth(24); // Zバッファのビット深度を24ビットに設定

	// 画面解像度の設定
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	// コントローラー操作用
	SetUseXInputFlag(TRUE);

	//エフェクトマネージャー生成
	EffectManager::CreateInstence();
	EffectManager* effectManager = EffectManager::GetInstance();
	effectManager->Setup();//セットアップ

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// 描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	// シーンマネージャー生成
	SceneManager::CreateInstance();
	// シーン初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Init();

	//サウンドマネージャー生成
	SoundManager::CreateInstance();
	SoundManager* soundManager = SoundManager::GetInstance();
	soundManager->CreateSound();
	soundManager->Load();

	//Zバッファーを有効にする
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// エフェクトマネージャー初期化
	effectManager->Init(); 
	// エフェクトマネージャーロード
	effectManager->Load();
	// エフェクトマネージャースタート
	effectManager->Start();

	// MessageManager 初期化（DxLib_Init の後）
	MessageManager::CreateInstance();
	MessageManager* messageManager = MessageManager::GetInstance();
	// メッセージマネージャーロード
	messageManager->Load();
	// メッセージマネージャースタート
	messageManager->Start();

	// 入力初期化
	Input::Init();

	// FPS初期化
	FPSSystem::Init();

	// ゲームのメインループ
	while (ProcessMessage() >= 0)
	{
		// 1ミリ秒待機すると処理が安定する
		Sleep(1);

		// 画面をクリア
		ClearDrawScreen();

		// 入力更新
		Input::Update();

		// シーンを更新
		sceneManager->Update();
		
		// 入力描画
		Input::Draw();

		// メッセージ更新
		messageManager->Update();
		// メッセージ描画
		messageManager->Draw();

		// FPS更新
		FPSSystem::Update();
		// FPS描画
		FPSSystem::Draw();

		// エスケープキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) || sceneManager->GetInstance()->GetEndFlag()) break;

		// 1フレームが速すぎた場合の待機
		FPSSystem::WaitFPS();

		// 画面フリップ
		ScreenFlip();
	}

	// メッセージマネージャー削除
    messageManager->Fin();
	// メッセージマネージャー削除
	MessageManager::DeleteInstance();
	// シーンマネージャー削除
	SceneManager::DeleteInstance();
	// サウンドマネージャー削除
	SoundManager::DeleteInstance();
	// エフェクトマネージャー削除
	EffectManager::DeleteInstance();

	// 入力終了
	Input::Fin();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
