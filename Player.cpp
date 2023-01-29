#include "Player.h"

#define PI 3.14159265359
#define G 6.674	//���L���͒萔
#define GAcceleration 9.80665 * 1/10	//�d�͉����x

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

	//Y���W���t�ɂ��č��W�ݒ�
	position1 = { position0.x,-position0.y - 10,position0.z };
	rotation1 = rotation0;
	scale1 = scale0;
}

void Player::Update()
{
	//�����蔻��X�V
	UpdateCollision();
	//�ړ��X�V
	UpdateMove();

	//�ό`�s�񔽉f
	metaball0->SetPosition(position0);
	metaball0->SetScale(scale0);
	metaball0->SetRotation(rotation0);
	metaball1->SetPosition(position1);
	metaball1->SetScale(scale1);
	metaball1->SetRotation(rotation1);

	//�I�u�W�F�N�g�X�V
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
		//�\�̃I�u�W�F�N�g
		if (collision->Update(position0, scale0) == 1)
		{
			//�ڒn�t���O�𗧂Ă�
			groundFlag0 = true;
			//���R����T�̒l�����Z�b�g
			fallTimer0 = 0;
			//�����x�N�g�������Z�b�g
			fallVelocity0.y = 0;
		}
		//�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
		while (collision->Update(position0, scale0) == 1)
		{
			position0.y += 0.002f;
			if (collision->Update(position0, scale0) == 0)
			{
				break;
			}
		}
		//���̃I�u�W�F�N�g
		if (collision->Update(position1, scale1) == 1)
		{
			//�ڒn�t���O�𗧂Ă�
			groundFlag1 = true;
			//���R����T�̒l�����Z�b�g
			fallTimer1 = 0;
			//�����x�N�g�������Z�b�g
			fallVelocity1.y = 0;
		}
		//�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
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
	//�n�ʂɐڂ��Ă��Ȃ��ꍇ�̗�������(�\�̃I�u�W�F�N�g)
	if (groundFlag0 == false)
	{
		//60�t���[���Ń^�C�}�[��1�i�߂�
		fallTimer0 += 3.0f / 60.0f;
		//�����x�N�g���v�Z
		float v = GAcceleration * fallTimer0;
		fallVelocity0.y = -(GAcceleration * fallTimer0);
		//���W�ɉ��Z
		position0.x += fallVelocity0.x;
		position0.y += fallVelocity0.y;
		position0.z += fallVelocity0.z;
	}
	//�n�ʂɐڂ��Ă��Ȃ��ꍇ�̗�������(���̃I�u�W�F�N�g)
	if (groundFlag1 == false)
	{
		//60�t���[���Ń^�C�}�[��1�i�߂�
		fallTimer1 += 3.0f / 60.0f;
		//�����x�N�g���v�Z
		float v = GAcceleration * fallTimer1;
		fallVelocity1.y = +(GAcceleration * fallTimer1);
		//���W�ɉ��Z
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
