#include "Goal.h"

ID3D12Device* Goal::device = nullptr;
Camera* Goal::camera = nullptr;

void Goal::Initialize()
{
	//オブジェクト初期化
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);

	//ヒットボックスの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel);
	//ヒットボックスのスケール設定
	hitboxScale.x = 6.0f;
	hitboxScale.y = 6.0f;
	hitboxScale.z = 6.0f;

}

void Goal::Update()
{
	/*hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;*/

	//オブジェクト更新
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
	cubeObject->Draw(cmdList);
}

void Goal::SetTutorial()
{
	SetPosition({ -65.0	,12.0	,-5.0 });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };
}
