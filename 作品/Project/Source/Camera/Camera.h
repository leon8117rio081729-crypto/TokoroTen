#pragma once
#include "DxLib.h"
#include "CameraBase.h"

class Player;

// カメラクラス
class Camera : public CameraBase
{
public:
	// 定数
	// ニア、ファークリップの設定
	static constexpr float CAMERA_NEAR_CLIP = 5.0f;
	static constexpr float CAMERA_FAR_CLIP = 50.0f;
	// プレイヤーからの距離
	static constexpr float PLAYER_DISTANCE = -20.0f;
	static constexpr float ROTATION_SPEED = 0.025f;
	// X軸回転の制限値
	static constexpr float X_ROTATION_MAX = (DX_PI_F * 0.49f);
	static constexpr float X_ROTATION_MIN = (DX_PI_F * 0.000001f - 0.15f);
    // 視点操作感度
    static constexpr float CONTR_SENSITIVITY = 0.03f;
    // 補間率（小さいほど滑らか）
    static constexpr float SMOOTH = 0.15f;

	// カメラ衝突時の手前パディング（m単位）
	static constexpr float CAMERA_COLLISION_PADDING = 0.2f;
	// カメラ位置補間係数（0..1）
	static constexpr float CAMERA_POSITION_LERP = 0.2f;

	Camera();	// コンストラクタ
	~Camera();	// デストラクタ

public:
	// 初期化～終了関数はCameraBaseからoverrideする
	// これでCameraBaseポインタ変数からCameraクラスの関数が呼べる
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

	// Cameraクラス専用で必要な関数があれば追加で書く
	void SetMove(VECTOR move) { move = m_Move; }
private:

	float m_BaseTargetY;
	float m_CameraBaseY;   // カメラ位置用の基準Y
	float m_TargetBaseY;   // 注視点用の基準Y
 
	// Cameraクラス専用で使用する変数を書く
	Player* m_TargetPlayer;
	VECTOR m_Move;
	VECTOR m_TargetRot;
};


