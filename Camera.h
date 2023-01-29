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
	//シングルトンインスタンス
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize();
	//更新
	void Update();
	
	//プレイヤーを追尾する処理(プレイヤーの座標,プレイヤーの状態)
	void PlayerAim(DirectX::XMFLOAT3 pos, int playerState);

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
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//射影変換
	XMMATRIX matProjection_;
	//ビュー変換行列
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };

	//プレイヤー追尾処理用の変数
	//前のフレームのプレイヤーの状態を保存する変数
	int prePlayerState;
	//裏表切り替わっている瞬間に立てるフラグ
	bool changeMode = false;
	//裏表を切り替えるタイマー
	float changeModeTimer = 0.0f;
	//裏表を切り替えるのにかかる時間
	float changeModeTime = 60.0f;
	//プレイヤー追尾状態の際のY座標
	float changePosY = 10.0f;
};
