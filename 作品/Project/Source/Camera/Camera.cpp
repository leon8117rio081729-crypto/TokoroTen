#include <math.h>
#include "Camera.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "CameraManager.h"
#include "../Scene/ScreenSize.h"
#include "../Scene/SceneManager.h"
#include "../Collision/CollisionManager.h"
#include "../Color/Color.h"

// 基底クラスのコンストラクタ呼ぶ際は追加で書く
Camera::Camera() : CameraBase()
	, m_TargetPlayer(nullptr)
	, m_Move(VGet(0.0f, 0.0f, 0.0f))
	, m_TargetRot(VGet(0.0f, 0.0f, 0.0f))
	, m_BaseTargetY(0.0f)
	, m_CameraBaseY(0.0f)
	, m_TargetBaseY(0.0f)
{
}

Camera::~Camera()
{
	Fin();
}

void Camera::Init()
{
}

void Camera::Load()
{
}

void Camera::Start()
{
	//マウスを中央に固定と非表示
	SetMouseDispFlag(FALSE);                 // カーソル非表示
	SetMousePoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2); // 画面中央に固定

	// ニア、ファークリップの設定
	SetCameraNearFar(CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);

	// アップベクトル設定
	m_UpVec = VGet(0.0f, 1.0f, 0.0f);

	// 追従するプレイヤー
	m_TargetPlayer = PlayerManager::GetInstance()->GetPlayer();

	// 初期値設定
	m_Rot = VGet(0, 0, 0);
	m_TargetRot = m_Rot;
}

void Camera::Step()
{
	if (SceneManager::GetInstance()->m_IsPaused == false)
	{
		VECTOR rightStick = Input::GetRightStick(); // 視点変更用

		// 回転角変更（マウス差分の代わりにスティック入力を使う）
		m_TargetRot.y += rightStick.x * CONTR_SENSITIVITY;
		m_TargetRot.x -= rightStick.y * CONTR_SENSITIVITY;

		// ピッチ角制限（上下視点）
		if (m_TargetRot.x > X_ROTATION_MAX)
		{
			m_TargetRot.x = X_ROTATION_MAX;
		}
		if (m_TargetRot.x < X_ROTATION_MIN)
		{
			m_TargetRot.x = X_ROTATION_MIN;
		}

		// Y回転ラップ処理（0～2π）
		if (m_TargetRot.y < 0.0f)
		{
			m_TargetRot.y += DX_TWO_PI_F;
			m_Rot.y += DX_TWO_PI_F;
		}
		if (m_TargetRot.y > DX_TWO_PI_F)
		{
			m_TargetRot.y -= DX_TWO_PI_F;
			m_Rot.y -= DX_TWO_PI_F;
		}

		// スムーズに追従		
		m_Rot.x += (m_TargetRot.x - m_Rot.x) * SMOOTH;
		m_Rot.y += (m_TargetRot.y - m_Rot.y) * SMOOTH;
	
	}
	
}

void Camera::Update()
{
	//3人称カメラ
	// プレイヤーの座標
	VECTOR playerPos = m_TargetPlayer->GetPos();
	VECTOR playerNowPos = m_TargetPlayer->GetPos();

	// 高さ調整
	if (playerNowPos.y >= 13.5f)
	{
		playerPos.x = 33.0f;
		playerPos.y = 16.0f;
		playerPos.z = 45.0f;
	}
	else
	{
		playerPos.y = 0.5f;
	}
	

	// プレイヤー背後オフセット
	VECTOR offset = VGet(0.0f, 0.0f, PLAYER_DISTANCE);

	// ★ カメラ回転反映
	MATRIX rotY = MyMath::MatRotationYaw(m_Rot.y);
	MATRIX rotX = MyMath::MatRotationPitch(m_Rot.x);
	MATRIX rot = MyMath::MatMult(rotY, rotX);

	// 理想カメラ位置（ターゲット候補）
	VECTOR idealPos = MyMath::MatTransform(rot, offset);
	idealPos = MyMath::VecAdd(playerPos, idealPos);

	// 目標位置（補間でここへ近づける）
	VECTOR targetPos = idealPos;

	// ★ 衝突回避
	CollisionSphere* camSphere = CollisionManager::GetInstance()->CreateTempSphere(idealPos, 0.5f);

	VECTOR correctedPos;
	bool hit = CollisionManager::GetInstance()->CheckCameraCollision(camSphere, &correctedPos);

	if (hit)
	{
		// プレイヤー方向に少し戻してカメラ目標位置を決定（パディングは定数）
		VECTOR dir = MyMath::VecNormalize(MyMath::VecSub(playerPos, idealPos));
		VECTOR adjusted = MyMath::VecAdd(correctedPos, MyMath::VecScale(dir, -CAMERA_COLLISION_PADDING));
		targetPos = adjusted;
	}

	// 補間で滑らかに目標へ近づける
	m_Pos = MyMath::VecAdd(m_Pos, MyMath::VecScale(MyMath::VecSub(targetPos, m_Pos), CAMERA_POSITION_LERP));

	// 注視点はプレイヤーの座標
	m_Target = playerPos;
	m_Target.y += 8.0f;

	// カメラセット
	SetCameraPositionAndTargetAndUpVec(m_Pos, m_Target, m_UpVec);
}

void Camera::Draw()
{
	// デバッグ表示
#ifdef _DEBUG
	DrawFormatString(400, 40, WHITE, "Cam pos: %.2f, %.2f, %.2f", m_Pos.x, m_Pos.y, m_Pos.z);
	DrawFormatString(400, 60, WHITE, "Cam tgt: %.2f, %.2f, %.2f", m_Target.x, m_Target.y, m_Target.z);
	DrawFormatString(400, 80, WHITE, "Rot: %.3f, %.3f", m_Rot.x, m_Rot.y);
	DrawFormatString(400, 100, WHITE, "Near/Far: %.3f / %.1f", CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);
	DrawFormatString(400, 120, WHITE, "カメラの座標[%f, %f, %f]", m_Pos.x, m_Pos.y, m_Pos.z);
#endif // _DEBUG
}

void Camera::Fin()
{

}
