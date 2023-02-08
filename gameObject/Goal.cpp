#include "Goal.h"

ID3D12Device* Goal::device = nullptr;
Camera* Goal::camera = nullptr;

void Goal::Initialize()
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
	hitboxScale.x = 6.0f;
	hitboxScale.y = 6.0f;
	hitboxScale.z = 6.0f;

}

void Goal::Update()
{
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

void Goal::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
	/*cubeObject->Draw(cmdList);*/
}

void Goal::SetTutorial()
{
	SetPosition({ 50,5,50 });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}
void Goal::SetStage2()
{
	SetPosition({ 10.0f	,5.0f	,10.0f });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}
void Goal::SetStage4()
{
	SetPosition({ 50.0,   -31.0,  54.0 });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}
