#include "Camera.h"
#include "Math.h"
#include "Math2.h"
#define PI 3.14159265359

Input* Camera::input = nullptr;
DXInput* Camera::dxInput = nullptr;

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	//射影変換
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	//デバッグ用
	/*if (input->PushKey(DIK_D))
	{
		position.x += 0.55;
	}
	if (input->PushKey(DIK_A))
	{
		position.x -= 0.55;
	}
	if (input->PushKey(DIK_W))
	{
		eye_.y += 0.55;
	}
	if (input->PushKey(DIK_S))
	{
		eye_.y -= 0.55;
	}
	if (input->PushKey(DIK_W))
	{
		position.z += 0.55;
	}
	if (input->PushKey(DIK_S))
	{
		position.z -= 0.55;
	}*/


	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}
void Camera::PlayerAim(DirectX::XMFLOAT3 pos, int playerState)
{
	//注視点をプレイヤーのx座標に合わせる
	target_ = { pos.x,0,pos.z };

	//前のフレームと現在のフレームで状態が違ったらフラグを立てる
	if (playerState != prePlayerState)
	{
		changeMode = true;
	}

	//changeMode==trueのとき
	if (changeMode == true)
	{
		//タイマー開始
		changeModeTimer += 1.0f;

		/*easeInOutQuart(changeModeTimer / 60.0f);*/
		//表から裏の場合
		if (playerState == 1)
		{
			eye_.y = changePosY - ((changePosY * easeOutQuint(changeModeTimer / 60.0f))* 2);
		}
		//裏から表の場合
		if (playerState == 0)
		{
			eye_.y = -changePosY + ((changePosY * easeOutQuint(changeModeTimer / 60.0f))*2);
		}

		//設定していた時間まで到達したらchangeMode解除
		if (changeModeTimer >= changeModeTime)
		{
			changeMode = false;
			changeModeTimer = 0;
		}
		
	}

	//プレイヤーの状態によって視点を変更
	if (playerState == 0 && changeMode == false)
	{
		eye_ = { pos.x,changePosY,pos.z - 50 };
	}
	if (playerState == 1 && changeMode == false)
	{
		eye_ = { pos.x,-changePosY,pos.z - 50 };
	}

	//前のフレームの状態を取得
	prePlayerState = playerState;
}
//void Camera::PlayerAim(DirectX::XMFLOAT3 pos, Player::State playerState)
//{
//	//注視点をプレイヤーのx座標に合わせる
//	target_ = { pos.x,0,pos.z };
//
//	if (playerState == Player::State::front)
//	{
//		eye_ = { pos.x,10,pos.z - 30 };
//	}
//	if (playerState == Player::State::back)
//	{
//		eye_ = { pos.x,-10,pos.z - 30 };
//	}
//}
void Camera::SetTarget(XMFLOAT3 pos)
{
	target_ = pos;
}

void Camera::SetEye(XMFLOAT3 pos)
{
	eye_ = pos;
}