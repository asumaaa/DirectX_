#pragma once
#include "FbxObject3D.h"
#include "FbxModel.h"
#include "CubeModel.h"
#include "CubeObject3D.h"
#include "DirectXMath.h"

class Obstacle
{
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Obstacle::device = device; }
	static void SetCamera(Camera* camera) { Obstacle::camera = camera; }
	//���f���Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox�p�̃��f��
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//�X�V
	void Initialize();
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�Q�b�^�[
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	//hitbox
	DirectX::XMFLOAT3 GetHitboxPosition() { return hitboxPosition; }
	DirectX::XMFLOAT3 GetHitboxRotation() { return hitboxRotation; }
	DirectX::XMFLOAT3 GetHitboxScale() { return hitboxScale; }
	//�Z�b�^�[
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

	//�q�b�g�{�b�N�X�Z�b�g(�t�@�C����ǂݍ��񂾌�ɂ���)
	void SetHitbox();

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//�I�u�W�F�N�g
	FbxModel *model;
	FbxObject3D* object = nullptr;

	//hitbox
	CubeModel* cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 10.0f,5.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.06f,0.06f,0.06f };

	//hitbox�̕ό`�s��
	DirectX::XMFLOAT3 hitboxPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale = { 0.01f,0.01f,0.01f };
};

