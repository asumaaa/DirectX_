#include "Key.h"
ID3D12Device* Key::device = nullptr;
Camera* Key::camera = nullptr;

void Key::Initialize()
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
	hitboxScale.x = 4.0f;
	hitboxScale.y = 9.0f;
	hitboxScale.z = 4.0f;
}

void Key::Update()
{
	//hitboxの座標調整
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
