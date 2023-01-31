#pragma once
#include "Collision.h"
#include "Camera.h"
#include "Input.h"
#include "DXInput.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"

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

	//���f���Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox�p�̃��f��
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//�V�[�����Ƃ̃Z�b�g
	void SetTutorial();

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
	void SetCollisionFloor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	void SetCollisionObstacle(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);

	//�Q�b�^�[
	//�ό`�s��
	DirectX::XMFLOAT3 GetPosition0() { return position0; }
	DirectX::XMFLOAT3 GetRotation0() { return rotation0; }
	DirectX::XMFLOAT3 GetScale0() { return scale0; }
	DirectX::XMFLOAT3 GetPosition1() { return position1; }
	DirectX::XMFLOAT3 GetRotation1() { return rotation1; }
	DirectX::XMFLOAT3 GetScale1() { return scale1; }

	//hitbox
	DirectX::XMFLOAT3 GetHitboxPosition0() { return hitboxPosition0; }
	DirectX::XMFLOAT3 GetHitboxRotation0() { return hitboxRotation0; }
	DirectX::XMFLOAT3 GetHitboxScale0() { return hitboxScale0; }
	DirectX::XMFLOAT3 GetHitboxPosition1() { return hitboxPosition1; }
	DirectX::XMFLOAT3 GetHitboxRotation1() { return hitboxRotation1; }
	DirectX::XMFLOAT3 GetHitboxScale1() { return hitboxScale1; }

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
	/*std::unique_ptr<Metaball> metaball0;
	std::unique_ptr<Metaball> metaball1;*/

	//���f��
	FbxModel* model;
	CubeModel* cubeModel;
	FbxObject3D* object0 = nullptr;
	FbxObject3D* object1 = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject0;
	std::unique_ptr<CubeObject3D> cubeObject1;
	
	//�����蔻��
	std::list<std::unique_ptr<Collision>> collisionsFloor;
	std::list<std::unique_ptr<Collision>> collisionsObstacle;

	//�ό`�s��
	DirectX::XMFLOAT3 position0 = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 rotation0 = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 scale0 = {0.01f,0.01f,0.01f};
	DirectX::XMFLOAT3 position1;
	DirectX::XMFLOAT3 rotation1;
	DirectX::XMFLOAT3 scale1;

	//hitbox�̕ό`�s��
	DirectX::XMFLOAT3 hitboxPosition0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale0 = { 0.01f,0.01f,0.01f };
	DirectX::XMFLOAT3 hitboxPosition1;
	DirectX::XMFLOAT3 hitboxRotation1;
	DirectX::XMFLOAT3 hitboxScale1;

	//�ړ��x�N�g��
	XMFLOAT3 velocity0 = { 0,0,0 };
	XMFLOAT3 velocity1 = { 0,0,0 };

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

