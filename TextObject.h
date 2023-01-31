#pragma once
#include "Collision.h"
#include "Camera.h"
#include "FbxModel.h"
#include "FbxObject3D.h"

class TextObject
{
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { TextObject::device = device; }
	static void SetCamera(Camera* camera) { TextObject::camera = camera; }
	//�R���X�g���N�^
	TextObject() {};
	//�f�X�g���N�^
	~TextObject() {};
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//���f���Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }

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

	//���f��
	FbxModel* model;
	FbxObject3D* object = nullptr;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,30.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 20.0f,20.0f,20.0f };
};


