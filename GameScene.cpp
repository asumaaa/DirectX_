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
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10, -50 });

	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest", "Resources/white1x1.png");

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
	cubeModel->SetImageData({ 0.7f, 0.3f, 0.3f,1.0f });

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

	Metaball* newMetaball = new Metaball();
	newMetaball->Initialize();
	newMetaball->SetInput(input_);
	newMetaball->SetImageData({ 1.0, 1.0, 1.0, 1 });
	newMetaball->SetPosition({ 0,10,0 });
	newMetaball->SetScale({ 3,3,3 });
	metaball.reset(newMetaball);

	//�����蔻�菉����
	for (int i = 0; i < stageObjNum; i++)
	{
		std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
		newCollision->SetObject(cubeObject->GetPosition(), cubeObject->GetScale());
		collisions.push_back(std::move(newCollision));
	}

}

void GameScene::Update()
{
	camera_->Update();

	object1->Update();
	cubeObject->Update();

	//�����蔻��X�V
	for (std::unique_ptr<Collision>& collision : collisions)
	{
		metaball->UpdateCollision(collision.get());
	}

	metaball->Update();

	//�R���g���[���[�X�V
	dxInput->InputProcess();
}

void GameScene::Draw()
{
	/*object1->Draw(dxCommon_->GetCommandList());*/
	cubeObject->Draw(dxCommon_->GetCommandList());
	metaball->Draw(dxCommon_->GetCommandList());
}
