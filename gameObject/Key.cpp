#include "Key.h"
ID3D12Device* Key::device = nullptr;
Camera* Key::camera = nullptr;

void Key::Initialize()
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
	//�q�b�g�{�b�N�X�̃X�P�[���ݒ�
	hitboxScale.x = 4.0f;
	hitboxScale.y = 9.0f;
	hitboxScale.z = 4.0f;
}

void Key::Update()
{
	//hitbox�̍��W����
	/*hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;*/

	//�I�u�W�F�N�g�X�V
	object->SetPosition(position);
	object->SetScale(scale);
	object->SetRotation(rotation);
	object->Update();

	cubeObject->SetPosition(hitboxPosition);
	cubeObject->SetScale(hitboxScale);
	cubeObject->SetRotation(hitboxRotation);
	cubeObject->Update();
}

void Key::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
	cubeObject->Draw(cmdList);
}

void Key::SetTutorial()
{
	SetPosition({ -20,-10,-50 });
	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };
}
