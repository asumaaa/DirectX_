#include "Camera.h"
#include "Math.h"
#include "Math2.h"
//#define PI 3.14159265359

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

	//���݂̃t���[���̃L�[��ۑ�
	preKeyR = keyR;
	preKeyL = keyL;
	preBKeyR = bKeyR;
	preBKeyL = bKeyL;
}
void Camera::StageSelect(DirectX::XMFLOAT3 pos, int playerState)
{
	//�O�̃t���[���ƌ��݂̃t���[���ŏ�Ԃ��������t���O�𗧂Ă�
	if (playerState != prePlayerState)
	{
		changeMode = true;
	}

	//changeMode==true�̂Ƃ�
	if (changeMode == true)
	{
		//�^�C�}�[�J�n
		changeModeTimer += 1.0f;

		/*easeInOutQuart(changeModeTimer / 60.0f);*/
		//�\���痠�̏ꍇ
		if (playerState == 1)
		{
			eye_.y = eyeTitleY - ((eyeTitleY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			target_.y = targetTitlePosY - ((targetTitlePosY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			eye_.x = pos.x;
			target_.x = pos.x;
		}
		//������\�̏ꍇ
		if (playerState == 0)
		{
			eye_.y = -eyeTitleY + ((eyeTitleY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			target_.y = -targetTitlePosY + ((targetTitlePosY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			eye_.x = pos.x;
			target_.x = pos.x;
		}

		//�ݒ肵�Ă������Ԃ܂œ��B������changeMode����
		if (changeModeTimer >= changeModeTime)
		{
			changeMode = false;
			changeModeTimer = 0;
		}

	}

	//�v���C���[�̏�Ԃɂ���Ď��_��ύX
	if (playerState == 0 && changeMode == false)
	{
		eye_ = { pos.x,eyeTitleY,pos.z - 100 };
		target_ = { pos.x,targetTitlePosY,pos.z + 20 };
	}
	if (playerState == 1 && changeMode == false)
	{
		eye_ = { pos.x,-eyeTitleY,pos.z - 100 };
		target_ = { pos.x,-targetTitlePosY,pos.z + 20 };
	}

	//�O�̃t���[���̏�Ԃ��擾
	prePlayerState = playerState;
}

void Camera::PlayerAim(DirectX::XMFLOAT3 pos0, DirectX::XMFLOAT3 pos1, int playerState)
{
	//�R���g���[���[���͂�ϐ��ɑ��
	if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)keyR = 1;
	else { keyR = 0; }
	if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)keyL = 1;
	else { keyL = 0; }
	if (dxInput->GamePad.state.Gamepad.bRightTrigger)bKeyR = 1;
	else { bKeyR = 0; }
	if (dxInput->GamePad.state.Gamepad.bLeftTrigger)bKeyL = 1;
	else { bKeyL = 0; }

	//�����_���v���C���[��x���W�ɍ��킹��
	if (playerState == 0)
	{
		target_ = { pos0.x,pos0.y,pos0.z };
	}
	if (playerState == 1)
	{
		target_ = { pos1.x,pos1.y,pos1.z };
	}

#pragma region Y���W�֘A
	//�O�̃t���[���ƌ��݂̃t���[���ŏ�Ԃ��������t���O�𗧂Ă�
	if (playerState != prePlayerState)
	{
		changeMode = true;
	}

	//changeMode==true�̂Ƃ�
	if (changeMode == true)
	{
		//�^�C�}�[�J�n
		changeModeTimer += 1.0f;

		/*easeInOutQuart(changeModeTimer / 60.0f);*/
		//�\���痠�̏ꍇ
		if (playerState == 1)
		{
			eye_.y = changePosY - ((changePosY * easeOutQuint(changeModeTimer / 60.0f))* 2);
		}
		//������\�̏ꍇ
		if (playerState == 0)
		{
			eye_.y = -changePosY + ((changePosY * easeOutQuint(changeModeTimer / 60.0f))*2);
		}

		//�ݒ肵�Ă������Ԃ܂œ��B������changeMode����
		if (changeModeTimer >= changeModeTime)
		{
			changeMode = false;
			changeModeTimer = 0;
		}
		
	}

#pragma endregion

#pragma region �v���C���[�Ƃ̋���
	//����R�{�^��
	if (bKeyR == 1 && preBKeyR == 0 && approachFlag == false && approachMode == Max)
	{
		approachFlag = true;
		approachMin = true;
		approachMode = Min;
	}
	//����L�{�^��
	else if (bKeyL == 1 && preBKeyL == 0 && approachFlag == false && approachMode == Min)
	{
		approachFlag = true;
		approachMax = true;
		approachMode = Max;
	}

	//1�t���[��������̐ڋߗ�(60/distanceTime)
	float ap = -(maxPlayerDistance - minPlayerDistance) / distanceTime;
	if (approachFlag)
	{
		//�^�C�}�[�X�V
		distanceTimer++;
		if (approachMax)
		{
			playerDistance -= ap;
		}
		if (approachMin) 
		{
			playerDistance += ap;
		}

		if (distanceTimer >= distanceTime)
		{
			distanceTimer = 0;
			approachFlag = false;
			approachMax = false;
			approachMin = false;
		}
	}

#pragma endregoin 

#pragma region XZ�֘A

	//90�x
	float rot = (float)PI * 1 / 2;
	//�L�[���͂Ő؂�ւ�
	//R�̃g���K�[�L�[
	if (keyR == 1 && preKeyR == 0 && changeModeR == false && changeModeL == false)
	{
		changeModeR = true;
	}
	//L�̃g���K�[�L�[
	if (keyL == 1 && preKeyL == 0 && changeModeR == false && changeModeL == false)
	{
		changeModeL = true;
		//���[�h��ύX
		/*if (mode == forward)	mode = left;
		else if (mode == left)		mode = backward;
		else if (mode == backward)	mode = right;
		else if (mode == right)		mode = forward;*/
	}

	//1�t���[��������ɑ����p�x
	float rot2 = rot / changeTime;
	//�p�x�����Z���鏈��
	if (changeModeR || changeModeL)
	{
		//�^�C�}�[�X�V
		changeTimer++;

		//R�̏ꍇ
		if (changeModeR)
		{
			changeRot += rot / changeTime;
			//�^�C�}�[�������܂Ői�񂾂烂�[�h�ύX
			if (changeTimer == changeTime / 2)
			{
				//���[�h��ύX
				if (mode == forward)	mode = right;
				else if (mode == right)		mode = backward;
				else if (mode == backward)	mode = left;
				else if (mode == left)		mode = forward;
			}
		}
		//L�̏ꍇ
		if (changeModeL)
		{
			changeRot -= rot / changeTime;
			//�^�C�}�[�������܂Ői�񂾂烂�[�h�ύX
			if (changeTimer == changeTime / 2)
			{
				//���[�h��ύX
				if (mode == forward)	mode = left;
				else if (mode == left)		mode = backward;
				else if (mode == backward)	mode = right;
				else if (mode == right)		mode = forward;
			}
		}

		if (changeTimer >= changeTime)
		{
			changeTimer = 0;
			changeModeR = false;
			changeModeL = false;
		}
	}

	//XZ���W�ݒ�
	if (playerState == 0)
	{
		changeRotX = cos(changeRot) * playerDistance + pos0.x;
		changeRotZ = sin(changeRot) * playerDistance + pos0.z;
	}
	if (playerState == 1)
	{
		changeRotX = cos(changeRot) * playerDistance + pos1.x;
		changeRotZ = sin(changeRot) * playerDistance + pos1.z;
	}


#pragma endregion

	//�v���C���[�̏�Ԃɂ���Ď��_��ύX
	if (playerState == 0 && changeMode == false)
	{
		eye_ = { changeRotX,changePosY,changeRotZ };
	}
	if (playerState == 1 && changeMode == false)
	{
		eye_ = { changeRotX,-changePosY,changeRotZ };
	}

	//�O�̃t���[���̏�Ԃ��擾
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

void Camera::SetCamera()
{
	//�J�����̈ʒu
	mode = forward;
	approachMode = Max;
	approachFlag = true;
	approachMax = true;
	approachMode = Max;
}