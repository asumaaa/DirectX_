#pragma once
#include "CubeObject3D.h"
#include "CubeModel.h"
#include "DirectXMath.h"

class Obstacle
{
public:
	//�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { Obstacle::device = device; }
	static void SetCamera(Camera* camera) { Obstacle::camera = camera; }
	//���f���Z�b�g
	/*void SetModel(CubeModel* model) { cubeModel = model; }*/

	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//�Q�b�^�[
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	//�Z�b�^�[
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�J����
	static Camera* camera;
	//�I�u�W�F�N�g
	std::unique_ptr<CubeModel> cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//�ό`�s��
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.0f,0.0f,0.0f };
};

