#include "Obstacle.h"

ID3D12Device* Obstacle::device = nullptr;
Camera* Obstacle::camera = nullptr;

void Obstacle::Update()
{

	//変形行列セット
	//object->SetPosition(position);
	//object->SetRotation(rotation);
	//object->SetScale(scale);

	////オブジェクト更新
	//object->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	/*object->Draw(cmdList);*/
}
