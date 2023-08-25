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
	model2 = FbxLoader::GetInstance()->LoadModelFromFile("Idle", "Resources/white1x1.png");
	stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");
	goalModel = FbxLoader::GetInstance()->LoadModelFromFile("goal", "Resources/portal.png");
	keyModel = FbxLoader::GetInstance()->LoadModelFromFile("key", "Resources/key.png");
	titleModel = FbxLoader::GetInstance()->LoadModelFromFile("title", "Resources/red.png");
	//stage
	stageTutoModel = FbxLoader::GetInstance()->LoadModelFromFile("tutorial", "Resources/red.png");
	stage1Model = FbxLoader::GetInstance()->LoadModelFromFile("stage1", "Resources/red.png");
	stage2Model = FbxLoader::GetInstance()->LoadModelFromFile("stage2", "Resources/red.png");
	stage3Model = FbxLoader::GetInstance()->LoadModelFromFile("stage3", "Resources/red.png");
	stage4Model = FbxLoader::GetInstance()->LoadModelFromFile("stage4", "Resources/red.png");
	stage5Model = FbxLoader::GetInstance()->LoadModelFromFile("stage5", "Resources/red.png");
	//clear
	clear1Model = FbxLoader::GetInstance()->LoadModelFromFile("clear", "Resources/white1x1.png");
	//������@
	moveTextModel = FbxLoader::GetInstance()->LoadModelFromFile("moveText", "Resources/white1x1.png");
	startTextModel = FbxLoader::GetInstance()->LoadModelFromFile("startText", "Resources/white1x1.png");
	jumpTextModel = FbxLoader::GetInstance()->LoadModelFromFile("jumpText", "Resources/white1x1.png");
	cameraTextModel = FbxLoader::GetInstance()->LoadModelFromFile("cameraText", "Resources/white1x1.png");
	zoomTextModel = FbxLoader::GetInstance()->LoadModelFromFile("zoomText", "Resources/white1x1.png");
	returnTextModel = FbxLoader::GetInstance()->LoadModelFromFile("returnText", "Resources/white1x1.png");

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
	cubeModel->SetImageData({ 0.2f, 0.0f, 0.0f,0.5f });
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

	Player* newPlayer2 = new Player();
	newPlayer2->SetModel(model2);
	newPlayer2->SetCubeModel(hitBoxModel.get());
	newPlayer2->SetPosition0(player->GetPosition0());
	newPlayer2->Initialize();
	player2.reset(newPlayer2);

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
		newObstacle->SetPosition({ 0,0,-10000 });
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region �e�L�X�g�̃I�u�W�F�N�g
	for (int i = 0; i < textObjectVol; i++)
	{
		std::unique_ptr<TextObject>newTextObject = std::make_unique<TextObject>();
		if (i == 0)    //�^�C�g��
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,30,0 });
		}
		if (i == 1)    //�^�C�g��(�t)
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 2)    //�`���[�g���A��
		{
			newTextObject->SetModel(stageTutoModel);
			newTextObject->SetPosition({ 150,30,0 });
		}
		if (i == 3)    //�X�e�[�W1
		{
			newTextObject->SetModel(stage1Model);
			newTextObject->SetPosition({ 160,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 4)    //�X�e�[�W2
		{
			newTextObject->SetModel(stage2Model);
			newTextObject->SetPosition({ 300,30,0 });
		}
		if (i == 5)    //�X�e�[�W3
		{
			newTextObject->SetModel(stage3Model);
			newTextObject->SetPosition({ 300,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 6)    //�X�e�[�W4
		{
			newTextObject->SetModel(stage4Model);
			newTextObject->SetPosition({ 450,30,0 });
		}
		if (i == 7)    //�X�e�[�W�T
		{
			newTextObject->SetModel(stage5Model);
			newTextObject->SetPosition({ 450,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 8)    //moveText
		{
			newTextObject->SetModel(moveTextModel);
			newTextObject->SetPosition({ 10,10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 9)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 180,10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 10)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 180,-10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
			newTextObject->SetRotation({ PI,0,0 });
		}	if (i == 11)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 320,10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 12)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 320,-10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 13)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 470,10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 14)    //startText
		{
			newTextObject->SetModel(startTextModel);
			newTextObject->SetPosition({ 470,-10,0 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 15)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 200,10,0 });
		}
		if (i == 16)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 200,-10,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 17)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 340,10,0 });
		}
		if (i == 18)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 340,-10,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 19)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 490,10,0 });
		}
		if (i == 20)    //jumpText
		{
			newTextObject->SetModel(clear1Model);
			newTextObject->SetPosition({ 490,-10,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}

		if (i == 21)    //startText
		{
			newTextObject->SetModel(cameraTextModel);
			newTextObject->SetPosition({ -10,10,-30 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 22)    //startText
		{
			newTextObject->SetModel(zoomTextModel);
			newTextObject->SetPosition({ -10,20,-30 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 23)    //startText
		{
			newTextObject->SetModel(jumpTextModel);
			newTextObject->SetPosition({ -50,10,-30 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
		}
		if (i == 24)    //startText
		{
			newTextObject->SetModel(returnTextModel);
			newTextObject->SetPosition({ -50,20,-30 });
			newTextObject->SetScale({ 5.0f,5.0f,5.0f });
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
	newGoal->SetScale({5.0f,5.0f ,5.0f });
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
	if (input_->TriggerKey(DIK_2))stage = Stage::Stage2;
	if (input_->TriggerKey(DIK_3))stage = Stage::Stage3;
	if (input_->TriggerKey(DIK_4))stage = Stage::Stage4;
	if (input_->TriggerKey(DIK_5))stage = Stage::Stage5;

	//�O�̃V�[���ƍ��̃V�[�����Ⴉ�����烊�Z�b�g����
	if (scene_ != preScene_)
	{
		//�^�C�g�����Z�b�g
		if (scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//�O�̃X�e�[�W�ƌ��݂̃X�e�[�W���Ⴉ�����烊�Z�b�g����
	if (stage != preStage)
	{
		//�`���[�g���A���Ɉړ������ꍇ
		if (stage == Stage::Tutorial)	SetTutorial();
		if (stage == Stage::Stage1)		SetStage1();
		if (stage == Stage::Stage2)		SetStage2();
		if (stage == Stage::Stage3)		SetStage3();
		if (stage == Stage::Stage4)		SetStage4();
		if (stage == Stage::Stage5)		SetStage5();
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
	if (player->GetSceneFlag() == 0)
	{

		if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		{
			scene_ = static_cast<size_t>(Scene::Game);
			sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
			stage = Stage::Tutorial;
		}
	}
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X && player->GetSceneFlag() == 1)
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Stage1;
	}
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X && player->GetSceneFlag() == 2)
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Stage2;
	}
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X && player->GetSceneFlag() == 3)
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Stage3;
	}
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X && player->GetSceneFlag() == 4)
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Stage4;
	}
	else if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X && player->GetSceneFlag() == 5)
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Stage5;
	}


	//�v���C���[�X�V
	player->Update();
	player2->Update();

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
	if (input_->TriggerKey(DIK_R)) {
		clearTutoFlag = false;
		clear1Flag = false;
		clear2Flag = false;
		clear3Flag = false;
		clear4Flag = false;
		clear5Flag = false;
	}
	//�v���C���[�`��
	if (player->GetMoveFlag() == true) {
		player->Draw(dxCommon_->GetCommandList());
	}
	else {
		player2->Draw(dxCommon_->GetCommandList());
	}
	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}
	//�e�L�X�g�̃I�u�W�F�N�g�`��
	int j = 0;
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		if (j == 0)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 1)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 2)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 3)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 4)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 5)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 6)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 7)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 8)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 9 && player->GetSceneFlag() == 0)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 10 && player->GetSceneFlag() == 1)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 11 && player->GetSceneFlag() == 2)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 12 && player->GetSceneFlag() == 3)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 13 && player->GetSceneFlag() == 4)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 14 && player->GetSceneFlag() == 5)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 15 && clearTutoFlag == true)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 16 && clear1Flag == true)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 17 && clear2Flag == true)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 18 && clear3Flag == true)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 19 && clear4Flag == true)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 20 && clear5Flag == true)textObject->Draw(dxCommon_->GetCommandList());
		j++;
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
	camera_->PlayerAim(player->GetPosition0(), player->GetPosition1(), player->GetPlayerState());
	//�J�����X�V
	camera_->Update();

	//�X�y�[�X����������w�肵�Ă�}�b�v�ɍX�V
	/*DebugLoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);*/

	//�v���C���[�X�V
	player->Update();
	player2->Update();
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
	if (player->GetGoalFlag() == true && stage == Stage::Tutorial)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clearTutoFlag = true;

	}

	else if (player->GetGoalFlag() == true && stage == Stage::Stage1)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clear1Flag = true;

	}
	else if (player->GetGoalFlag() == true && stage == Stage::Stage2)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clear2Flag = true;

	}
	else if (player->GetGoalFlag() == true && stage == Stage::Stage3)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clear3Flag = true;

	}
	else if (player->GetGoalFlag() == true && stage == Stage::Stage4)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clear4Flag = true;
	}
	else if (player->GetGoalFlag() == true && stage == Stage::Stage5)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Title;

		clear5Flag = true;
	}
}

void GameScene::GameDraw()
{
	//�v���C���[�`��
	if (player->GetMoveFlag() == true) {
		player->Draw(dxCommon_->GetCommandList());
	}
	else {
		player2->Draw(dxCommon_->GetCommandList());
	}
	//�S�[���`��
	if (player->GetKeyFlag())
	{
		goal->Draw(dxCommon_->GetCommandList());
	}
	//���`��
	if (player->GetKeyFlag() == false)
	{
		key->Draw(dxCommon_->GetCommandList());
	}
	//��Q���`��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		//�J�����O�̎�
		if (camera_->GetMode() == Camera::forward)
		{
			if (i != 6)floor->Draw(dxCommon_->GetCommandList());
		}
		//�J�����E�̎�
		if (camera_->GetMode() == Camera::right)
		{
			if (i != 2)floor->Draw(dxCommon_->GetCommandList());
		}
		//�J�������̎�
		if (camera_->GetMode() == Camera::backward)
		{
			if (i != 5)floor->Draw(dxCommon_->GetCommandList());
		}
		//�J�������̎�
		if (camera_->GetMode() == Camera::left)
		{
			if (i != 1)floor->Draw(dxCommon_->GetCommandList());
		}

		i++;
	}

	//�e�L�X�g�̃I�u�W�F�N�g�X�V
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Update();
	}

	//�e�L�X�g�̃I�u�W�F�N�g�`��
	int j = 0;
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		if (j == 21 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 22 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 23 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 24 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		j++;
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
	camera_->SetCamera();
	//�v���C���[�Z�b�g
	player->SetTitle();
	player2->SetTitle();
	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();

	int i = 0;
	for (std::unique_ptr<TextObject>& textObj : textObjects)
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
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,0,0 });
		}
		if (j == 1)
		{
			floor->SetScale({ 0.5f,80.0,60 });
			floor->SetPosition({ -320,0,0 });
		}
		if (j == 2)
		{
			floor->SetScale({ 0.5f,80.0,60 });
			floor->SetPosition({ 520,0,0 });
		}
		if (j == 3)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,40,0 });
		}
		if (j == 4)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,-40,0 });
		}
		if (j == 5)
		{
			floor->SetScale({ 1100,80.0f,0.5 });
			floor->SetPosition({ 0,0,30 });
		}
		if (j == 6)
		{
			floor->SetScale({ 1100,80.0f,0.5 });
			floor->SetPosition({ 0,0,-30 });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		j++;
	}

}

void GameScene::SetTutorial()
{	
	//�v���C���[�Z�b�g
	player->SetTutorial();
	player2->SetTutorial();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);
	//�S�[���Z�b�g
	goal->SetTutorial();
	//���Z�b�g
	key->SetTutorial();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();
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
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}

	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage1()
{
	//�v���C���[�Z�b�g
	player->SetStage();
	player2->SetStage();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial1.csv", tutorialObstacleVal1);
	//�S�[���Z�b�g
	goal->SetTutorial();
	//���Z�b�g
	key->SetStage1();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();
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
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage2()
{
	//�v���C���[�Z�b�g
	player->SetStage2();
	player2->SetStage2();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial2.csv", tutorialObstacleVal2);
	//�S�[���Z�b�g
	goal->SetStage2();
	//���Z�b�g
	key->SetStage2();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();
	//���Z�b�g
	float divider = 1.5f;
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,0,0 });
		}
		if (i == 1)
		{
			floor->SetScale({ 0.5f / divider,200.0f / divider,120.0f / divider });
			floor->SetPosition({ -60 / divider,0,0 });
		}
		if (i == 2)
		{
			floor->SetScale({ 0.5f / divider,200.0f / divider,120.0f / divider });
			floor->SetPosition({ 60 / divider,0,0 });
		}
		if (i == 3)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,100.0f / divider,0 });
		}
		if (i == 4)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,-100.0f / divider,0 });
		}
		if (i == 5)
		{
			floor->SetScale({ 120.0f / divider,200.0f / divider,0.5f / divider });
			floor->SetPosition({ 0,0,60.0f / divider });
		}
		if (i == 6)
		{
			floor->SetScale({ 120.0f / divider,200.0f / divider,0.5f / divider });
			floor->SetPosition({ 0,0,-60.0f / divider });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage3()
{

	//�v���C���[�Z�b�g
	player->SetStage();
	player2->SetStage();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial3.csv", tutorialObstacleVal3);
	//�S�[���Z�b�g
	goal->SetTutorial();
	//���Z�b�g
	key->SetStage3();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();

	//���Z�b�g
	float divider = 1.0f;
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,0,0 });
		}
		if (i == 1)
		{
			floor->SetScale({ 0.5f / divider,200.0f / divider,120.0f / divider });
			floor->SetPosition({ -60 / divider,0,0 });
		}
		if (i == 2)
		{
			floor->SetScale({ 0.5f / divider,200.0f / divider,120.0f / divider });
			floor->SetPosition({ 60 / divider,0,0 });
		}
		if (i == 3)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,100.0f / divider,0 });
		}
		if (i == 4)
		{
			floor->SetScale({ 120.0f / divider,0.5f / divider,120.0f / divider });
			floor->SetPosition({ 0,-100.0f / divider,0 });
		}
		if (i == 5)
		{
			floor->SetScale({ 120.0f / divider,200.0f / divider,0.5f / divider });
			floor->SetPosition({ 0,0,60.0f / divider });
		}
		if (i == 6)
		{
			floor->SetScale({ 120.0f / divider,200.0f / divider,0.5f / divider });
			floor->SetPosition({ 0,0,-60.0f / divider });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage4()
{

	//�v���C���[�Z�b�g
	player->SetStage();
	player2->SetStage();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial4.csv", tutorialObstacleVal4);
	//�S�[���Z�b�g
	goal->SetStage4();
	//���Z�b�g
	key->SetStage4();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();

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
			floor->SetScale({ 0.5f,200.0,120 });
			floor->SetPosition({ -60,0,0 });
		}
		if (i == 2)
		{
			floor->SetScale({ 0.5f,200.0,120 });
			floor->SetPosition({ 60,0,0 });
		}
		if (i == 3)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,100,0 });
		}
		if (i == 4)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,-100,0 });
		}
		if (i == 5)
		{
			floor->SetScale({ 120,200.0f,0.5 });
			floor->SetPosition({ 0,0,60 });
		}
		if (i == 6)
		{
			floor->SetScale({ 120,200.0f,0.5 });
			floor->SetPosition({ 0,0,-60 });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
}

void GameScene::SetStage5()
{
	
	//�v���C���[�Z�b�g
	player->SetStage();
	player2->SetStage();
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial5.csv", 246);
	//�S�[���Z�b�g
	goal->SetTutorial();
	goal->SetPosition({ -50.0f,3.0f,-50.0f });
	//���Z�b�g
	key->SetStage5();

	//�v���C���[�̓����蔻������Z�b�g
	player->ClearCollision();
	player2->ClearCollision();
	player->SetPosition0({ -50, 3,-50 });
	player2->SetPosition0({ -50, 3,-50 });

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
			floor->SetScale({ 0.5f,200.0,120 });
			floor->SetPosition({ -60,0,0 });
		}
		if (i == 2)
		{
			floor->SetScale({ 0.5f,200.0,120 });
			floor->SetPosition({ 60,0,0 });
		}
		if (i == 3)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,100,0 });
		}
		if (i == 4)
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,-100,0 });
		}
		if (i == 5)
		{
			floor->SetScale({ 120,200.0f,0.5 });
			floor->SetPosition({ 0,0,60 });
		}
		if (i == 6)
		{
			floor->SetScale({ 120,200.0f,0.5 });
			floor->SetPosition({ 0,0,-60 });
		}
		/*else
		{
			floor->SetScale({ 120,0.5,120 });
			floor->SetPosition({ 0,0,0 });
		}*/

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//��
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
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
			obstacle->SetPosition({ 1,1,1 });
			obstacle->SetHitbox();
		}
		else
		{
			obstacle->SetPosition({ 0,0,-10000 });
			obstacle->SetHitbox();
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
