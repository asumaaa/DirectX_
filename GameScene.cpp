#include "GameScene.h"
#include "FbxLoader.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//�J����������
	Camera* newCamera = new Camera();
	newCamera->Initialize(input_);
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,20,0 });
	camera_->SetEye({ 0, 0, -10 });

	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube", "Resources/cube/Crate.jpg");

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);
}

void GameScene::Update()
{
	camera_->Update();
	object1->Update();

	//�R���g���[���[�X�V
	dxInput->InputProcess();
}

void GameScene::Draw()
{
	object1->Draw(dxCommon_->GetCommandList());
}
