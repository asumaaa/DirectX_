#include "TextObject.h"

ID3D12Device* TextObject::device = nullptr;
Camera* TextObject::camera = nullptr;

void TextObject::Initialize()
{
	//�I�u�W�F�N�g������
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);
}

void TextObject::Update()
{
	//�I�u�W�F�N�g�X�V
	object->SetPosition(position);
	object->SetScale(scale);
	object->SetRotation(rotation);
	object->Update();
}

void TextObject::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
}

void TextObject::SetTutorial()
{
	/*SetPosition({ 50,10,50 });
	SetScale({ 50,10,50 });*/
}
