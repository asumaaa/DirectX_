#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:
	static enum Mode
	{
		forward,
		right,
		backward,
		left,
	};
	static enum ApproachMode
	{
		Max,
		Min,
	};

public:
	//�V���O���g���C���X�^���X
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//�C���X�g���N�^ �f�X�g���N�^
	Camera();
	~Camera();
	//������
	void Initialize();
	//�X�V
	void Update();
	

	//�^�C�g���A�X�e�[�W�Z���N�g�̋���
	void StageSelect(DirectX::XMFLOAT3 pos, int playerState);
	//�v���C���[��ǔ����鏈��(�v���C���[�̍��W,�v���C���[�̏��)
	void PlayerAim(DirectX::XMFLOAT3 pos0, DirectX::XMFLOAT3 pos1, int playerState);

	//�Q�b�^�[�Z�b�^�[
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };

	Mode GetMode() { return mode; }
private:
	//����
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//�ˉe�ϊ�
	XMMATRIX matProjection_;
	//�r���[�ϊ��s��
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };

	//�v���C���[�ǔ������p�̕ϐ�
	//�O�̃t���[���̃v���C���[�̏�Ԃ�ۑ�����ϐ�
	int prePlayerState;
	//���\�؂�ւ���Ă���u�Ԃɗ��Ă�t���O
	bool changeMode = false;
	//���\��؂�ւ���^�C�}�[
	float changeModeTimer = 0.0f;
	//���\��؂�ւ���̂ɂ����鎞��
	float changeModeTime = 60.0f;
	//�v���C���[�ǔ���Ԃ̍ۂ�Y���W
	float changePosY = 10.0f;

	//�v���C���[�ǔ���Ԃ̍ۂ�Y���W
	float eyeTitleY = 30.0f;
	float targetTitlePosY = 20.0f;

	//���_��ς���p�̕ϐ�
	//�R���g���[���[�ۑ��p
	float keyR = 0;
	float preKeyR = 0;
	float keyL = 0;
	float preKeyL = 0;
	//���
	float bKeyR = 0;
	float preBKeyR = 0;
	float bKeyL = 0;
	float preBKeyL = 0;
	//�p�x
	float changeRotX;
	float changeRotZ;
	float changeRot = (float)PI * 1/2;
	//�J�����̓���ւ��ɂ�����t���[��
	float changeTime = 30.0f;
	float changeTimer = 0.0f;
	bool changeModeR = false;
	bool changeModeL = false;
	//�J�����̐ڋ߂Ɏg���t���O
	bool approachFlag = false;
	bool approachMin = false;
	bool approachMax = false;
	//�v���C���[�ƃJ�����̋����̍ő�l
	float maxPlayerDistance = -100.0f;
	//�v���C���[�ƃJ�����̋����̍ŏ��l
	float minPlayerDistance = -40.0f;
	//�v���C���[�ƃJ�����̋���
	float playerDistance = maxPlayerDistance;
	//�v���C���[�̐ڋ߂ɂ�����t���[��
	float distanceTime = 30.0f;
	//�ڋ߂Ɏg���^�C�}�[
	float distanceTimer = 0.0f;

	//�J�����̈ʒu
	Mode mode = forward;
	ApproachMode approachMode = Max;
};
