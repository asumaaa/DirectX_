#pragma once
#include "Collision.h"
#include "Camera.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"

class Floor
{
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Floor::device = device; }
	static void SetCamera(Camera* camera) { Floor::camera = camera; }
	//�R���X�g���N�^
	Floor() {};
	//�f�X�g���N�^
	~Floor() {};
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���Z�b�g
	/*void SetModel(FbxModel* model) { this->model = model; }*/
	//hitbox�p�̃��f��
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//�V�[�����Ƃ̃Z�b�g
	void SetTitle();
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

	//���f��
	CubeModel* cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f,6.0f,1.0f };
};
