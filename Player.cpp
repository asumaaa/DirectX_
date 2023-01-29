#include "Player.h"

#define PI 3.14159265359
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

ID3D12Device* Player::device = nullptr;
Camera* Player::camera = nullptr;
Input* Player::input = nullptr; 
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
	Metaball* newMetaball0 = new Metaball();
	newMetaball0->Initialize();
	newMetaball0->SetInput(input);
	newMetaball0->SetImageData({ 1.0, 1.0, 1.0, 1 });
	metaball0.reset(newMetaball0);

	Metaball* newMetaball1 = new Metaball();
	newMetaball1->Initialize();
	newMetaball1->SetInput(input);
	newMetaball1->SetImageData({ 1.0, 1.0, 1.0, 1 });
	metaball1.reset(newMetaball1);

	//Y座標を逆にして座標設定
	position1 = { position0.x,-position0.y - 10,position0.z };
	rotation1 = rotation0;
	scale1 = scale0;
}

void Player::Update()
{
	//当たり判定更新
	UpdateCollision();
	//移動更新
	UpdateMove();

	//変形行列反映
	metaball0->SetPosition(position0);
	metaball0->SetScale(scale0);
	metaball0->SetRotation(rotation0);
	metaball1->SetPosition(position1);
	metaball1->SetScale(scale1);
	metaball1->SetRotation(rotation1);

	//オブジェクト更新
	metaball0->Update();
	metaball1->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	metaball0->Draw(cmdList);
	metaball1->Draw(cmdList);
}

void Player::UpdateCollision()
{
	for (std::unique_ptr<Collision>& collision : collisions)
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
		}
		//めり込まなくなりまで加算
		while (collision->Update(position0, scale0) == 1)
		{
			position0.y += 0.002f;
			if (collision->Update(position0, scale0) == 0)
			{
				break;
			}
		}
		//裏のオブジェクト
		if (collision->Update(position1, scale1) == 1)
		{
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
			position1.y -= 0.002f;
			if (collision->Update(position1, scale1) == 0)
			{
				break;
			}
		}
	}
}

void Player::UpdateMove()
{
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
		fallTimer1 += 3.0f / 60.0f;
		//落下ベクトル計算
		float v = GAcceleration * fallTimer1;
		fallVelocity1.y = +(GAcceleration * fallTimer1);
		//座標に加算
		position1.x += fallVelocity1.x;
		position1.y += fallVelocity1.y;
		position1.z += fallVelocity1.z;
	}
}

void Player::SetCollision(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisions.push_back(std::move(newCollision));
}
