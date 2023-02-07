#include "Obstacle.h"

ID3D12Device* Obstacle::device = nullptr;
Camera* Obstacle::camera = nullptr;

void Obstacle::Initialize()
{
	//�I�u�W�F�N�g������
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);

	//�q�b�g�{�b�N�X�̐ݒ�
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel);

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };
}

void Obstacle::Update()
{
	//hitbox����
	/*hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };*/

	//�ό`�s��Z�b�g
	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);
	//�I�u�W�F�N�g�X�V
	object->Update();

	cubeObject->SetPosition(hitboxPosition);
	cubeObject->SetScale(hitboxScale);
	cubeObject->SetRotation(hitboxRotation);
	cubeObject->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
	cubeObject->Draw(cmdList);
}

void Obstacle::SetHitbox()
{
	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };
}
