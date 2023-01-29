#pragma once
#include "CubeObject3D.h"
#include "CubeModel.h"
#include "DirectXMath.h"

class Obstacle
{
public:
	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { Obstacle::device = device; }
	static void SetCamera(Camera* camera) { Obstacle::camera = camera; }
	//モデルセット
	/*void SetModel(CubeModel* model) { cubeModel = model; }*/

	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//ゲッター
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	//セッター
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//オブジェクト
	std::unique_ptr<CubeModel> cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.0f,0.0f,0.0f };
};

