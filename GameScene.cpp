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
	goalModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/beast.png");
	keyModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/key.png");
	titleModel = FbxLoader::GetInstance()->LoadModelFromFile("title", "Resources/white1x1.png");
	stage1Model = FbxLoader::GetInstance()->LoadModelFromFile("stage1", "Resources/white1x1.png");

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

	//��
	Floor::SetDevice(dxCommon_->GetDevice());
	Floor::SetCamera(camera_.get());

	//�e�L�X�g�̃I�u�W�F�N�g
	TextObject::SetDevice(dxCommon_->GetDevice());
	TextObject::SetCamera(camera_.get());

#pragma endregion

#pragma region �L���[�u���f���̐ݒ�

	//���f���̐ݒ�
	//���̃��f��
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice()); 
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 0.3f, 0.0f, 0.0f,0.7f });
	//hitbox�̃��f��
	CubeModel* newCubeModel1 = new CubeModel();
	newCubeModel1->CreateBuffers(dxCommon_->GetDevice());
	hitBoxModel.reset(newCubeModel1);
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

#pragma region ���̐ݒ�

	//��������
	for (int i = 0; i < floorVol; i++)
	{
		std::unique_ptr<Floor>newFloor = std::make_unique<Floor>();
		newFloor->SetCubeModel(cubeModel.get());
		newFloor->Initialize();
		newFloor->SetScale({ 120,0.5,120 });
		newFloor->SetPosition({ 0,0,0 });
		floors.push_back(std::move(newFloor));
	}

#pragma endregion

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
		newObstacle->SetPosition({0,0,-10000});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region �e�L�X�g�̃I�u�W�F�N�g
	for (int i = 0; i < textObjectVol; i++)
	{
		std::unique_ptr<TextObject>newTextObject = std::make_unique<TextObject>();
		if (i == 0)	//�^�C�g��
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,30,0 });
		}
		if (i == 1)	//�X�e�[�W1
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		newTextObject->Initialize();
		textObjects.push_back(std::move(newTextObject));
	}

#pragma endregion

#pragma region �S�[��������

	//�f�o�C�X�ƃJ�����Z�b�g
	Goal::SetDevice(dxCommon_->GetDevice());
	Goal::SetCamera(camera_.get());
	//�S�[��������
	Goal* newGoal = new Goal();
	newGoal->SetModel(goalModel);
	newGoal->SetCubeModel(hitBoxModel.get());
	newGoal->Initialize();
	goal.reset(newGoal);

#pragma endregion 

#pragma region ��������

	//�f�o�C�X�ƃJ�����Z�b�g
	Key::SetDevice(dxCommon_->GetDevice());
	Key::SetCamera(camera_.get());
	//�S�[��������
	Key* newKey = new Key();
	newKey->SetModel(keyModel);
	newKey->SetCubeModel(hitBoxModel.get());
	newKey->Initialize();
	key.reset(newKey);

#pragma endregion

#pragma region �X�v���C�g
	//�X�v���C�g����������
	spriteCommon = sprite->SpriteCommonCreate(dxCommon_->GetDevice(), 1280, 720);
	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/title.png", dxCommon_->GetDevice());
	sprite->SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/key.png", dxCommon_->GetDevice());
	titleSprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	titleSprite.SetTexNumber(0);
	titleSprite.SetPosition(XMFLOAT3(200, 100, 0));
	titleSprite.SetScale(XMFLOAT2(414 * 2.2, 54 * 2.2));
	keySprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	keySprite.SetTexNumber(1);
	keySprite.SetPosition(XMFLOAT3(1200, 650, 0));
	keySprite.SetScale(XMFLOAT2(64, 64));
#pragma endregion

	SetTitle();

}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();


	//�V�[�����Ƃ̏���
	(this->*Scene_[scene_])();

	//�f�o�b�O�p �L�[���͂ŃX�e�[�W�ύX
	if (input_->TriggerKey(DIK_0))stage = Stage::Tutorial;
	if (input_->TriggerKey(DIK_1))stage = Stage::Stage1;

	//�O�̃V�[���ƍ��̃V�[�����Ⴉ�����烊�Z�b�g����
	if (scene_ != preScene_)
	{
		//�^�C�g�����Z�b�g
		if(scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//�O�̃X�e�[�W�ƌ��݂̃X�e�[�W���Ⴉ�����烊�Z�b�g����
	if (stage != preStage)
	{
		//�`���[�g���A���Ɉړ������ꍇ
		if (stage == Stage::Tutorial)	SetTutorial();
		if (stage == Stage::Stage1)		SetStage1();
	}

	//�O�̃t���[���̃V�[���擾
	preScene_ = scene_;
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


	//�v���C���[�X�V
	player->Update();
	//���X�V
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//�e�L�X�g�̃I�u�W�F�N�g�X�V
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Update();
	}

	camera_->StageSelect(player->GetPosition0(), player->GetPlayerState());
	//�J�����X�V
	camera_->Update();
}

void GameScene::TitleDraw()
{
	//�v���C���[�`��
	player->Draw(dxCommon_->GetCommandList());
	//���`��
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Draw(dxCommon_->GetCommandList());
	}
	//�e�L�X�g�̃I�u�W�F�N�g�`��
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Draw(dxCommon_->GetCommandList());
	}

	//�f�o�b�O
	/*if (player->GetGroundFlag0())
	{
		keySprite.SpriteTransferVertexBuffer(keySprite);
		keySprite.SpriteUpdate(keySprite, spriteCommon);
		sprite->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon);
		keySprite.SpriteDraw(dxCommon_->GetCommandList(), spriteCommon, dxCommon_->GetDevice(), keySprite.vbView);
	}*/
}

void GameScene::GameUpdate()
{
	camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());
	//�J�����X�V
	camera_->Update();

	//�X�y�[�X����������w�肵�Ă�}�b�v�ɍX�V
	/*DebugLoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);*/

	//�v���C���[�X�V
	player->Update();
	//�S�[���X�V
	goal->Update();
	//���X�V
	key->Update();
	//���X�V
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}
	//��Q���X�V
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}
}

void GameScene::GameDraw()
{
	//�v���C���[�`��
	player->Draw(dxCommon_->GetCommandList());
	//�S�[���`��
	if (player->GetKeyFlag())
	{
		goal->Draw(dxCommon_->GetCommandList());
	}
	//���`��
	key->Draw(dxCommon_->GetCommandList());
	//��Q���`��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i != 6)
		{
			floor->Draw(dxCommon_->GetCommandList());
		}
		i++;
	}

	//�����擾������`��
	/*if (player->GetKeyFlag())
	{
		keySprite.SpriteTransferVertexBuffer(keySprite);
		keySprite.SpriteUpdate(keySprite, spriteCommon);
		sprite->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon);
		keySprite.SpriteDraw(dxCommon_->GetCommandList(), spriteCommon, dxCommon_->GetDevice(), keySprite.vbView);
	}*/

	/*if (player->GetGroundFlag0())
	{
		keySprite.SpriteTransferVertexBuffer(keySprite);
		keySprite.SpriteUpdate(keySprite, spriteCommon);
		sprite->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon);
		keySprite.SpriteDraw(dxCommon_->GetCommandList(), spriteCommon, dxCommon_->GetDevice(), keySprite.vbView);
	}*/
}

void GameScene::SetTitle()
{
	//�v���C���[�Z�b�g
	player->SetTitle();
	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();

	int i = 0;
	for (std::unique_ptr<TextObject>& textObj: textObjects)
	{
		if (i == 0)	//�^�C�g��
		{
			textObj->SetPosition({ 0,30,0 });
		}
		if (i == 1)	//�X�e�[�W1
		{
			textObj->SetPosition({ 0,-30,0 });
			textObj->SetRotation({ PI,0,0 });
		}
		i++;
	}

	//���Z�b�g
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->SetScale({ 360,0.5,30 });
		floor->SetPosition({ 180,0,0 });
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
	}

}

void GameScene::SetTutorial()
{
	//�v���C���[�Z�b�g
	player->SetTutorial();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);
	//�S�[���Z�b�g
	goal->SetTutorial();
	//���Z�b�g
	key->SetTutorial();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	//���Z�b�g
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}
		if (i == 1)
		{
			floor->SetScale({ 0.5f,80.0,120 });
			floor->SetPosition({ -60,0,0 });
		}
		if (i == 2)
		{
			floor->SetScale({ 0.5f,80.0,120 });
			floor->SetPosition({ 60,0,0 });
		}
		if (i == 3)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,40,0 });
		}
		if (i == 4)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,-40,0 });
		}
		if (i == 5)
		{
			floor->SetScale({ 120,80.0f,0.5 });
			floor->SetPosition({ 0,0,60 });
		}
		if (i == 6)
		{
			floor->SetScale({ 120,80.0f,0.5 });
			floor->SetPosition({ 0,0,-60 });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}

	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage1()
{
	//�v���C���[�Z�b�g
	player->SetTutorial();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleStage1.csv", stage1ObstacleVal);
	//�S�[���Z�b�g
	goal->SetTutorial();
	//���Z�b�g
	key->SetTutorial();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	//���Z�b�g
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->SetScale({ 120,0.5,120 });
		floor->SetPosition({ 0,0,0 });
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(key->GetPosition(), key->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::LoadCsv(const wchar_t* fileName, int obstacleVal)
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
			if (i <= obstacleVal)
			{
				obstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
				obstacle->SetHitbox();
			}
			else
			{
				obstacle->SetPosition({ 0,0,-10000});
			}
			i++;
		}
}

void GameScene::DebugLoadCsv(const wchar_t* fileName, int obstacleVal)
{
	if (input_->TriggerKey(DIK_SPACE))
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
			if (i <= obstacleVal)
			{
				obstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
			}
			else
			{
				obstacle->SetPosition({ 0,0,-10000 });
			}
			i++;
		}
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
