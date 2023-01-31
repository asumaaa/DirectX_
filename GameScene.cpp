#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"

#define PI 3.1415

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

#pragma region �J����������

	//�J����������
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 0, -50 });

#pragma endregion

#pragma region FBX�ǂݍ���
	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");
	stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");

#pragma endregion

#pragma region �I�u�W�F�N�g�Ƀf�o�C�X���Z�b�g
	
	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//�L���[�u�̐ݒ�
	//�f�o�C�X���Z�b�g
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();

	//���^�{�[��
	//�f�o�C�X���Z�b�g
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//��Q��
	Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());

#pragma endregion

#pragma region �L���[�u���f���̐ݒ�

	//���f���̐ݒ�
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	hitBoxModel.reset(newCubeModel);
	//���̃��f��
	cubeModel->SetImageData({ 1.0f, 0.0f, 0.0f,1.0f });
	//hitbox�̃��f��
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

	//�L���[�u�I�u�W�F�N�g�̐ݒ�
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel.get());
	cubeObject->SetScale({ 120.0f,0.5f,120.0f });
	cubeObject->SetPosition({ 0.0f,0.0f,0.0f });

#pragma region �v���C���[������

	//�v���C���[�Z�b�g
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//�v���C���[������
	Player* newPlayer = new Player();
	newPlayer->SetModel(model1);
	newPlayer->SetCubeModel(hitBoxModel.get());
	newPlayer->Initialize();
	player.reset(newPlayer);

#pragma endregion

#pragma region ��Q��������

	//�t�@�C���ǂݍ���
	std::stringstream obstaclePosList;	//������
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/obstacleTutorial.csv");
	//�t�@�C���̓��e���R�s�[
	obstaclePosList << file.rdbuf();
	//�t�@�C�������
	file.close();

	std::string line;

	//�t�@�C�������Q���̏ꏊ��ǂݍ���
	while (getline(obstaclePosList, line, '{'))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//�J���}��؂�Ő擪��������擾
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		obstaclePos.push_back(pos);
	}
	//��Q��������
	for (int i = 1; i <= obstacleVal; i++)
	{
		std::unique_ptr<Obstacle>newObstacle = std::make_unique<Obstacle>();
		newObstacle->SetModel(stoneModel);
		newObstacle->SetCubeModel(hitBoxModel.get());
		//�t�@�C���ǂݍ��݂œ������W����
		newObstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion


	//�v���C���[�ɓ����蔻����Z�b�g
	player->SetCollisionFloor(cubeObject->GetPosition(), cubeObject->GetScale());	//��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}


	//�X�v���C�g����������
	spriteCommon = sprite->SpriteCommonCreate(dxCommon_->GetDevice(), 1280, 720);
	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/title.png", dxCommon_->GetDevice());
	titleSprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	titleSprite.SetTexNumber(0);
	titleSprite.SetPosition(XMFLOAT3(200, 100, 0));
	titleSprite.SetScale(XMFLOAT2(414 * 2.2, 54 * 2.2));
}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();
	//�V�[�����Ƃ̏���
	(this->*Scene_[scene_])();

	//�O�̃X�e�[�W�ƌ��݂̃X�e�[�W���Ⴉ�����烊�Z�b�g����
	if (stage != preStage)
	{
		if (stage == Stage::Tutorial)
		{
			player->SetTutorial();
			LoadCsv(L"Resources/obstacleTutorial.csv");
		}
	}

	//�O�̃t���[���̃V�[���擾
	preStage = stage;
}

void GameScene::Draw()
{
	(this->*SceneDraw_[sceneDraw_])();
}

void GameScene::TitleUpdate()
{
	if (input_->PushKey(DIK_SPACE))
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Tutorial;
	}

	//�^�C�g���̃X�v���C�g�X�V
	titleSprite.SpriteTransferVertexBuffer(titleSprite);
	titleSprite.SpriteUpdate(titleSprite, spriteCommon);
}

void GameScene::TitleDraw()
{
	//�X�v���C�g���ʃR�}���h
	sprite->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon);

	titleSprite.SpriteDraw(dxCommon_->GetCommandList(), spriteCommon, dxCommon_->GetDevice(), titleSprite.vbView);
}

void GameScene::GameUpdate()
{
	camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());
	//�J�����X�V
	camera_->Update();

	//�L���[�u�I�u�W�F�N�g�X�V
	cubeObject->Update();
	//�v���C���[�X�V
	player->Update();
	//��Q���X�V
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}
}

void GameScene::GameDraw()
{
	//�L���[�u�`��
	cubeObject->Draw(dxCommon_->GetCommandList());
	//�v���C���[�`��
	player->Draw(dxCommon_->GetCommandList());
	//��Q���`��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::LoadCsv(const wchar_t* fileName)
{
	//�t�@�C���ǂݍ���
		std::stringstream obstaclePosList;	//������
		std::vector<DirectX::XMFLOAT3>obstaclePos;
		//�t�@�C�����J��
		std::ifstream file;
		file.open(fileName);
		//�t�@�C���̓��e���R�s�[
		obstaclePosList << file.rdbuf();
		//�t�@�C�������
		file.close();

		std::string line;

		//�t�@�C�������Q���̏ꏊ��ǂݍ���
		while (getline(obstaclePosList, line, '{'))
		{
			//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
			std::istringstream line_stream(line);
			std::string word1;
			std::string word2;
			std::string word3;
			//�J���}��؂�Ő擪��������擾
			getline(line_stream, word1, ',');
			getline(line_stream, word2, ',');
			getline(line_stream, word3, ',');
			DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
			obstaclePos.push_back(pos);
		}

		int i = 1;
		for (std::unique_ptr<Obstacle>& obstacle : obstacles)
		{
			obstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
			i++;
		}
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
