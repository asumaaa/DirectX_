#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"

using namespace DirectX;

class Camera
{
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
	
	//�v���C���[��ǔ����鏈��(�v���C���[�̍��W,�v���C���[�̏��)
	void PlayerAim(DirectX::XMFLOAT3 pos, int playerState);

	//�Q�b�^�[�Z�b�^�[
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };
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
};
