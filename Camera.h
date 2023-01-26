#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "WinApp.h"

using namespace DirectX;

class Camera
{
public:
	//シングルトンインスタンス
	static Camera* GetInstance();
	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize(Input* input);
	//更新
	void Update();
	//ゲッターセッター
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };
private:
	//入力
	Input* input_;
	//射影変換
	XMMATRIX matProjection_;
	//ビュー変換行列
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };
};
