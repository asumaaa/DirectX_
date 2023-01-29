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
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 0, -50 });

	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest", "Resources/white1x1.png");
	/*stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");*/

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//fbx�̃I�u�W�F�N�g
	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();

	//�L���[�u�̐ݒ�
	//�f�o�C�X���Z�b�g
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();
	//���f���̐ݒ�
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 0.7f, 0.3f, 0.3f,0.1f });

	//�L���[�u�I�u�W�F�N�g�̐ݒ�
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel.get());
	cubeObject->SetScale({ 30.0f,0.5f,30.0f });
	cubeObject->SetPosition({ 0.0f,0.0f,0.0f });

	//���^�{�[��
	//�f�o�C�X���Z�b�g
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//�v���C���[�Z�b�g
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//�v���C���[������
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	player.reset(newPlayer);

	//��Q��
	/*Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());*/
	/*obstacle->SetModel(model1);*/
	/*obstacle->SetPosition({10.0f,0.0f,0.0f});*/

	//�v���C���[�ɓ����蔻����Z�b�g
	player->SetCollision(cubeObject->GetPosition(), cubeObject->GetScale());
}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();
	//�V�[�����Ƃ̏���
	(this->*Scene_[scene_])();
}

void GameScene::Draw()
{
	(this->*SceneDraw_[sceneDraw_])();
}

void GameScene::TitleUpdate()
{
}

void GameScene::TitleDraw()
{
}

void GameScene::GameUpdate()
{
	camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());
	//�J�����X�V
	camera_->Update();

	//fBX�I�u�W�F�N�g�X�V
	object1->Update();
	//�L���[�u�I�u�W�F�N�g�X�V
	cubeObject->Update();
	//�v���C���[�X�V
	player->Update();
	//��Q���X�V
	/*obstacle->Update();*/
}

void GameScene::GameDraw()
{
	/*object1->Draw(dxCommon_->GetCommandList());*/
	//�L���[�u�`��
	cubeObject->Draw(dxCommon_->GetCommandList());
	//�v���C���[�`��
	player->Draw(dxCommon_->GetCommandList());
	//��Q���`��
	/*obstacle->Draw(dxCommon_->GetCommandList());*/
}

void (GameScene::* GameScene::Scene_[])() =
{
	&GameScene::TitleUpdate,
	&GameScene::GameUpdate,
};

void (GameScene::* GameScene::SceneDraw_[])() =
{
	&GameScene::TitleDraw,
	&GameScene::GameDraw,
};
