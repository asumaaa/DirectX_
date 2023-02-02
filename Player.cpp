#include "Player.h"
#include "math.h"
#include <cmath>

#define PI 3.14159265359
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

ID3D12Device* Player::device = nullptr;
Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model);
	object0->PlayAnimation();

	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model);
	object1->PlayAnimation();

	//Y座標を逆にして座標設定
	position1 = { position0.x,		-position0.y - 10,	position0.z };
	rotation1 = { rotation0.x + (float)PI,	rotation0.y + (float)PI,		rotation0.y };
	scale1 = scale0;

	//ヒットボックスの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject0.reset(newCubeObject);
	cubeObject0->SetModel(cubeModel);
	CubeObject3D* newCubeObject1 = new CubeObject3D();
	newCubeObject1->Initialize();
	cubeObject1.reset(newCubeObject1);
	cubeObject1->SetModel(cubeModel);

	hitboxPosition0.x = position0.x;
	hitboxPosition0.y = position0.y + 6;
	hitboxPosition0.z = position0.z;
	hitboxRotation0 = { 0.0f,0.0f,0.0f };
	hitboxScale0 = { 3.0f,12.0f,3.0f };

	hitboxPosition1.x = position1.x;
	hitboxPosition1.y = position1.y - 6;
	hitboxPosition1.z = position1.z;
	hitboxRotation1 = hitboxRotation0;
	hitboxScale1 = hitboxScale0;
}

void Player::Update()
{
	//移動更新
	UpdateMove();
	//当たり判定更新
	UpdateCollision();

	//hitboxの座標調整
	hitboxPosition0.x = position0.x;
	hitboxPosition0.y = position0.y + 6;
	hitboxPosition0.z = position0.z;

	hitboxPosition1.x = position1.x;
	hitboxPosition1.y = position1.y - 6;
	hitboxPosition1.z = position1.z;

	//オブジェクト更新
	object0->SetPosition(position0);
	object0->SetScale(scale0);
	object0->SetRotation(rotation0);
	object0->Update();

	object1->SetPosition(position1);
	object1->SetScale(scale1);
	object1->SetRotation(rotation1);
	object1->Update();

	cubeObject0->SetPosition(hitboxPosition0);
	cubeObject0->SetScale(hitboxScale0);
	cubeObject0->SetRotation(hitboxRotation0);
	cubeObject0->Update();

	cubeObject1->SetPosition(hitboxPosition1);
	cubeObject1->SetScale(hitboxScale1);
	cubeObject1->SetRotation(hitboxRotation1);
	cubeObject1->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object0->Draw(cmdList);
	object1->Draw(cmdList);
	cubeObject0->Draw(cmdList);
	cubeObject1->Draw(cmdList);
}

void Player::SetTitle()
{
	groundFlag0 = false;
	groundFlag1 = false;

	SetPosition0({ 0,10,0 });
	SetPosition1({ 0,-10,0 });
}

void Player::SetTutorial()
{
	groundFlag0 = false;
	groundFlag1 = false;

	SetPosition0({ -50,10,-50 });
	SetPosition1({ -50,-10,-50 });
}

void Player::UpdateCollision()
{
	/*XMVECTOR player0 = XMLoadFloat3(&position0);
	XMVECTOR player1 = XMLoadFloat3(&position1);
	XMVECTOR direction = XMVector3Normalize(player0 - player1);
	player1 = player1 + direction;
	XMStoreFloat3(&position1, player1);*/

	DirectX::XMFLOAT3 Min = { hitboxPosition0.x - hitboxScale0.x,
		hitboxPosition0.y - hitboxScale0.y, hitboxPosition0.z - hitboxScale0.z };
	DirectX::XMFLOAT3 Max = { hitboxPosition0.x + hitboxScale0.x,
		hitboxPosition0.y + hitboxScale0.y, hitboxPosition0.z + hitboxScale0.z };


	DirectX::XMFLOAT3 Min1 = { hitboxPosition1.x - hitboxScale1.x,
		hitboxPosition1.y - hitboxScale1.y, hitboxPosition1.z - hitboxScale1.z };
	DirectX::XMFLOAT3 Max1 = { hitboxPosition1.x + hitboxScale1.x,
		hitboxPosition1.y + hitboxScale1.y, hitboxPosition1.z + hitboxScale1.z };

	//-----------オブジェクトとの当たり判定---------
	for (std::unique_ptr<Collision>& collision : collisionsObstacle)
	{
		//左からぶつかる場合
		if (-2 + velocity0.x <= Max.x - collision->GetMin().x && Max.x - collision->GetMin().x <= 2 + velocity0.x)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.x -= 0.0002f;
				hitboxPosition0.x -= 0.0002f;
				position1.x -= 0.0002f;
				hitboxPosition1.x -= 0.0002f;

			}
		}
		//右からぶつかる場合
		if (-2 + velocity0.x <= Min.x - collision->GetMax().x && Min.x - collision->GetMax().x <= 2 + velocity0.x)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.x += 0.0002f;
				hitboxPosition0.x += 0.0002f;
				position1.x += 0.0002f;
				hitboxPosition1.x += 0.0002f;
			}
		}
		//下からぶつかる場合
		if (-2 + velocity0.z <= Max.z - collision->GetMin().z && Max.z - collision->GetMin().z <= 2 + velocity0.z)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.z -= 0.0002f;
				hitboxPosition0.z -= 0.0002f;
				position1.z -= 0.0002f;
				hitboxPosition1.z -= 0.0002f;
			}
		}
		//上からぶつかる場合
		if (-2 + velocity0.z <= Min.z - collision->GetMax().z && Min.z - collision->GetMax().z <= 2 + velocity0.z)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.z += 0.0002f;
				hitboxPosition0.z += 0.0002f;
				position1.z += 0.0002f;
				hitboxPosition1.z += 0.0002f;
			}
		}

		//player2
		//左からぶつかる場合
		if (-2 + velocity1.x <= Max1.x - collision->GetMin().x && Max1.x - collision->GetMin().x <= 2 + velocity1.x)
		{
			while (collision->Update(hitboxPosition1, hitboxScale1) == 1)
			{
				position1.x -= 0.0002f;
				hitboxPosition1.x -= 0.0002f;

			}

		}
		//右からぶつかる場合
		else if (-2 + velocity1.x <= Min1.x - collision->GetMax().x && Min1.x - collision->GetMax().x <= 2 + velocity1.x)
		{
			while (collision->Update(hitboxPosition1, hitboxScale1) == 1)
			{
				position1.x += 0.0002f;
				hitboxPosition1.x += 0.0002f;

			}
		}
		//下からぶつかる場合
		else if (-2 + velocity1.z <= Max1.z - collision->GetMin().z && Max1.z - collision->GetMin().z <= 2 + velocity1.z)
		{
			while (collision->Update(hitboxPosition1, hitboxScale1) == 1)
			{
				position1.z -= 0.0002f;
				hitboxPosition1.z -= 0.0002f;

			}
		}
		//上からぶつかる場合
		else if (-2 + velocity1.z <= Min1.z - collision->GetMax().z && Min1.z - collision->GetMax().z <= 2 + velocity1.z)
		{
			while (collision->Update(hitboxPosition1, hitboxScale1) == 1)
			{
				position1.z += 0.0002f;
				hitboxPosition1.z += 0.0002f;

			}
		}
	}

	//-----------床との当たり判定-----------------
	for (std::unique_ptr<Collision>& collision : collisionsFloor)
	{
		//表のオブジェクト
		if (collision->Update(position0, scale0) == 1)
		{
			//接地フラグを立てる
			groundFlag0 = true;
			//自由落下Tの値をリセット
			fallTimer0 = 0;
			//落下ベクトルをリセット
			fallVelocity0.y = 0;

			//接地フラグをfalseに
			fallTimer1 = -1;
			
			groundFlag1 = false;
			
		}
		//めり込まなくなりまで加算
		while (collision->Update(position0, scale0) == 1)
		{
			//めり込んだらプレイヤーの状態を変更
			position0.y += 0.002f;
			if (collision->Update(position0, scale0) == 0)
			{
				playerState = back;
				break;
			}
		}
		//裏のオブジェクト
		if (collision->Update(position1, scale1) == 1)
		{
			//followFlag = false;
			//接地フラグを立てる
			groundFlag1 = true;
			//自由落下Tの値をリセット
			fallTimer1 = 0;
			//落下ベクトルをリセット
			fallVelocity1.y = 0;
		}
		//めり込まなくなりまで加算
		while (collision->Update(position1, scale1) == 1)
		{
			//めり込んだらプレイヤーの状態を変更
			position1.y -= 0.002f;
			if (collision->Update(position1, scale1) == 0)
			{
				break;
			}
		}
	}

	//-----------鍵との当たり判定---------
	for (std::unique_ptr<Collision>& collision : collisionsKey)
	{
		if (collision->Update(hitboxPosition0, hitboxScale0) == 1)
		{
			keyFlag = true;
		}
		if (collision->Update(hitboxPosition1, hitboxScale1) == 1)
		{
			keyFlag = true;
		}
	}
}

void Player::UpdateMove()
{

	//左ステックの変数
	float x = dxInput->GamePad.state.Gamepad.sThumbLY / (32767.0f) * (PI / 90.0f);
	float y = dxInput->GamePad.state.Gamepad.sThumbLX / (32767.0f) * (PI / 90.0f);
	//スティックで得た変数を代入
	velocity0.z = x * 10;
	velocity0.x = y * 10;
	velocity1.x = velocity0.x;
	velocity1.z = velocity0.z;
	while(dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		velocity0.z = x * 20;
		velocity0.x = y * 20;
		velocity1.x = velocity0.x;
		velocity1.z = velocity0.z;
		break;
	}
	if (followFlag == true)
	{
		XMVECTOR player0 = XMLoadFloat3(&position0);
		XMVECTOR player1 = XMLoadFloat3(&position1);
		XMVECTOR direction = XMVector3Normalize(player0 - player1);
		player1 = player1 + direction;
		XMStoreFloat3(&position1, player1);

		//followFlag = false;
	}
	//--------------落下、ジャンプ----------------
	//スペースキーでジャンプ
	if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A && groundFlag0 == true && playerState == front)
	{
		//接地フラグをfalseに
		fallTimer0 = -1;
		groundFlag0 = false;
	}

	//スペースキーでジャンプ
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B /*&& groundFlag1 == true*/ && playerState == back)
	{
		//接地フラグをfalseに
		playerState = front;
		//followFlag = false;
	}


	//地面に接していない場合の落下処理(表のオブジェクト)
	if (groundFlag0 == false)
	{
		//60フレームでタイマーを1進める
		fallTimer0 += 3.0f / 60.0f;
		//落下ベクトル計算
		float v = GAcceleration * fallTimer0;
		fallVelocity0.y = -(GAcceleration * fallTimer0);
		//座標に加算
		position0.x += fallVelocity0.x;
		position0.y += fallVelocity0.y;
		position0.z += fallVelocity0.z;
	}
	//地面に接していない場合の落下処理(裏のオブジェクト)
	if (groundFlag1 == false)
	{
		//60フレームでタイマーを1進める
		fallTimer1 += 2.0f / 60.0f;
		//落下ベクトル計算
		float v = GAcceleration * fallTimer1;
		fallVelocity1.y = +(GAcceleration * fallTimer1);
		//座標に加算
		position1.x += fallVelocity1.x;
		position1.y += fallVelocity1.y;
		position1.z += fallVelocity1.z;
	}

	position0.x += velocity0.x;
	position0.y += velocity0.y;
	position0.z += velocity0.z;
	position1.x += velocity1.x;
	position1.y += velocity1.y;
	position1.z += velocity1.z;


	//プレイヤーの向き
	float c = atan(velocity0.x / velocity0.z);
	if (velocity0.z > 0)
	{
		playerDirection0 = { 0.0f				,atan(c)				,0.0f };
		playerDirection1 = { 0.0f + (float)PI	,atan(c) + (float)PI	,0.0f };
	}
	else if (velocity0.z < 0)
	{
		playerDirection0 = { 0.0f				,atan(c) + (float)PI	,0.0f };
		playerDirection1 = { 0.0f + (float)PI	,atan(c)				,0.0f };
	}

	rotation0 = playerDirection0;
	rotation1 = playerDirection1;
}

void Player::SetCollisionObstacle(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsObstacle.push_back(std::move(newCollision));
}

void Player::SetCollisionKey(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsKey.push_back(std::move(newCollision));
}

void Player::SetCollisionGoal(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsGoal.push_back(std::move(newCollision));
}

void Player::ClearCollision()
{
	collisionsObstacle.clear();
	collisionsFloor.clear();
	collisionsKey.clear();
	collisionsGoal.clear();
}

void Player::SetCollisionFloor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsFloor.push_back(std::move(newCollision));
}
