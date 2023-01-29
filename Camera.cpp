#include "Camera.h"
#include "Math.h"
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
	//�ˉe�ϊ�
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)window_width / window_height,//�A�X�y�N�g��(��ʉ���/��ʗ���)
		0.1f, 1000.0f						//�O�[�A���[
	);

	//�s��v�Z
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	//�f�o�b�O�p
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
	//�����_���v���C���[��x���W�ɍ��킹��
	target_ = { pos.x,0,pos.z };

	if (playerState == 0)
	{
		eye_ = { pos.x,10,pos.z - 30 };
	}
	if (playerState == 1)
	{
		eye_ = { pos.x,-10,pos.z - 30 };
	}
	prePlayerState = playerState;
}
//void Camera::PlayerAim(DirectX::XMFLOAT3 pos, Player::State playerState)
//{
//	//�����_���v���C���[��x���W�ɍ��킹��
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