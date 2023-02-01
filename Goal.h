#pragma once
#include "Collision.h"
#include "Camera.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"

class Goal
{
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Goal::device = device; }
	static void SetCamera(Camera* camera) { Goal::camera = camera; }
	//�R���X�g���N�^
	Goal() {};
	//�f�X�g���N�^
	~Goal() {};
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

	//�Z�b�^�[
	//�ό`�s��Z�b�g
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

	//�Q�b�^�[
	//�ό`�s��
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }

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
	FbxObject3D* object = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.06f,0.06f,0.06f };

	//hitbox�̕ό`�s��
	DirectX::XMFLOAT3 hitboxPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale = { 0.01f,0.01f,0.01f };
};
