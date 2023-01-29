#pragma once
#include "Metaball.h"
#include "Collision.h"
#include "Camera.h"
#include "Input.h"
#include "DXInput.h"
#include "Collision.h"

class Player
{
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Player::device = device; }
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }
	
	//�R���X�g���N�^
	Player() {};
	//�f�X�g���N�^
	~Player() {};
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�����蔻��X�V
	void UpdateCollision();
	//�����X�V
	void UpdateMove();
	
	//�Z�b�^�[
	//�ό`�s��Z�b�g
	void SetPosition0(DirectX::XMFLOAT3 pos) { position0 = pos; }
	void SetRotation0(DirectX::XMFLOAT3 rot) { rotation0 = rot; }
	void SetScale0(DirectX::XMFLOAT3 sca) { scale0 = sca; }
	void SetPosition1(DirectX::XMFLOAT3 pos) { position1 = pos; }
	void SetRotation1(DirectX::XMFLOAT3 rot) { rotation1 = rot; }
	void SetScale1(DirectX::XMFLOAT3 sca) { scale1 = sca; }
	//�����蔻��Z�b�g
	void SetCollision(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);

	//�Q�b�^�[
	//�ό`�s��
	DirectX::XMFLOAT3 GetPosition0() { return position0; }
	DirectX::XMFLOAT3 GetRotation0() { return rotation0; }
	DirectX::XMFLOAT3 GetScale0() { return scale0; }
	DirectX::XMFLOAT3 GetPosition1() { return position1; }
	DirectX::XMFLOAT3 GetRotation1() { return rotation1; }
	DirectX::XMFLOAT3 GetScale1() { return scale1; }

	//�����o�ϐ�
private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//���f�����I�u�W�F�N�g(�v���C���[��2����̂�2����)
	std::unique_ptr<Metaball> metaball0;
	std::unique_ptr<Metaball> metaball1;
	
	//�����蔻��
	std::list<std::unique_ptr<Collision>> collisions;

	//�ό`�s��
	DirectX::XMFLOAT3 position0 = {0.0f,10.0f,0.0f};
	DirectX::XMFLOAT3 rotation0 = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 scale0 = {3.0f,3.0f,3.0f};
	DirectX::XMFLOAT3 position1;
	DirectX::XMFLOAT3 rotation1;
	DirectX::XMFLOAT3 scale1;

	//�ړ��A�����p�ϐ�
	//�����x�N�g��
	XMFLOAT3 fallVelocity0 = { 0,0,0 };
	XMFLOAT3 fallVelocity1 = { 0,0,0 };
	//�����^�C�}�[
	float fallTimer0 = 0.0f;
	float fallTimer1 = 0.0f;
	//�ڒn�t���O
	bool groundFlag0 = false;
	bool groundFlag1 = false;

	//�\���̗񋓌^
public:
	enum State
	{
		front,	//�\��Ԃ̂Ƃ�
		back,	//����Ԃ̂Ƃ�
	};
	//�Q�b�^�[
	State GetPlayerState() { return playerState; }
private:
	//�v���C���[�̏�Ԃ�\���t���O
	State playerState = front;
};

