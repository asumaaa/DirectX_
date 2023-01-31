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
	//メンバ関数
public:
	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { Player::device = device; }
	static void SetCamera(Camera* camera) { Player::camera = camera; }
	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }
	
	//コンストラクタ
	Player() {};
	//デストラクタ
	~Player() {};
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルセット
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox用のモデル
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//シーンごとのセット
	void SetTutorial();

	//当たり判定更新
	void UpdateCollision();
	//動き更新
	void UpdateMove();
	
	//セッター
	//変形行列セット
	void SetPosition0(DirectX::XMFLOAT3 pos) { position0 = pos; }
	void SetRotation0(DirectX::XMFLOAT3 rot) { rotation0 = rot; }
	void SetScale0(DirectX::XMFLOAT3 sca) { scale0 = sca; }
	void SetPosition1(DirectX::XMFLOAT3 pos) { position1 = pos; }
	void SetRotation1(DirectX::XMFLOAT3 rot) { rotation1 = rot; }
	void SetScale1(DirectX::XMFLOAT3 sca) { scale1 = sca; }
	//当たり判定セット
	void SetCollisionFloor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);
	void SetCollisionObstacle(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale);

	//ゲッター
	//変形行列
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

	//メンバ変数
private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//モデル兼オブジェクト(プレイヤーが2つあるので2つ生成)
	/*std::unique_ptr<Metaball> metaball0;
	std::unique_ptr<Metaball> metaball1;*/

	//モデル
	FbxModel* model;
	CubeModel* cubeModel;
	FbxObject3D* object0 = nullptr;
	FbxObject3D* object1 = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject0;
	std::unique_ptr<CubeObject3D> cubeObject1;
	
	//当たり判定
	std::list<std::unique_ptr<Collision>> collisionsFloor;
	std::list<std::unique_ptr<Collision>> collisionsObstacle;

	//変形行列
	DirectX::XMFLOAT3 position0 = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 rotation0 = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 scale0 = {0.01f,0.01f,0.01f};
	DirectX::XMFLOAT3 position1;
	DirectX::XMFLOAT3 rotation1;
	DirectX::XMFLOAT3 scale1;

	//hitboxの変形行列
	DirectX::XMFLOAT3 hitboxPosition0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale0 = { 0.01f,0.01f,0.01f };
	DirectX::XMFLOAT3 hitboxPosition1;
	DirectX::XMFLOAT3 hitboxRotation1;
	DirectX::XMFLOAT3 hitboxScale1;

	//移動ベクトル
	XMFLOAT3 velocity0 = { 0,0,0 };
	XMFLOAT3 velocity1 = { 0,0,0 };

	//移動、落下用変数
	//落下ベクトル
	XMFLOAT3 fallVelocity0 = { 0,0,0 };
	XMFLOAT3 fallVelocity1 = { 0,0,0 };
	//落下タイマー
	float fallTimer0 = 0.0f;
	float fallTimer1 = 0.0f;
	//接地フラグ
	bool groundFlag0 = false;
	bool groundFlag1 = false;

	//表裏の列挙型
public:
	enum State
	{
		front,	//表状態のとき
		back,	//裏状態のとき
	};
	//ゲッター
	State GetPlayerState() { return playerState; }
private:
	//プレイヤーの状態を表すフラグ
	State playerState = front;
};

