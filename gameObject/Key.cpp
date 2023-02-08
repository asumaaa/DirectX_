#include "Key.h"
#include "Math2.h"

ID3D12Device* Key::device = nullptr;
Camera* Key::camera = nullptr;

#define PI 3.1415

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

	//��]
	addRot += 0.02;
	SetRotation({PI,addRot,0.0f});

	//�㉺
	float add = 0.0f;
	if (posY < 0)
	{
		if (easeInFlag == false)
		{
			addPos += 0.01;
		}
		if (easeInFlag == true)
		{
			addPos -= 0.01;
		}
		add = -easeInOutQuart(addPos);
	}
	if (posY > 0)
	{
		if (easeInFlag == false)
		{
			addPos += 0.01;
		}
		if (easeInFlag == true)
		{
			addPos -= 0.01;
		}
		add = easeInOutQuart(addPos);
	}

	//addPos�����Z�b�g �t���O����ւ�
	if (addPos >= 1 && easeInFlag == false)
	{
		easeInFlag = true;
	}
	else if (addPos <= 0 && easeInFlag == true)
	{
		easeInFlag = false;
	}

	position.y = posY + add;

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
	/*cubeObject->Draw(cmdList);*/
}

void Key::SetTutorial()
{
	SetPosition({ -20,-5,-50 });
	posY = position.y;
	SetScale({ 0.24f ,0.24f , 0.48f});
	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y - 3;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };

}
