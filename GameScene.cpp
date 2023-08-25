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

#pragma region カメラ初期化

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 0, -50 });

#pragma endregion

#pragma region FBX読み込み
	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
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
	//操作方法
	moveTextModel = FbxLoader::GetInstance()->LoadModelFromFile("moveText", "Resources/white1x1.png");
	startTextModel = FbxLoader::GetInstance()->LoadModelFromFile("startText", "Resources/white1x1.png");
	jumpTextModel = FbxLoader::GetInstance()->LoadModelFromFile("jumpText", "Resources/white1x1.png");
	cameraTextModel = FbxLoader::GetInstance()->LoadModelFromFile("cameraText", "Resources/white1x1.png");
	zoomTextModel = FbxLoader::GetInstance()->LoadModelFromFile("zoomText", "Resources/white1x1.png");
	returnTextModel = FbxLoader::GetInstance()->LoadModelFromFile("returnText", "Resources/white1x1.png");

#pragma endregion

#pragma region オブジェクトにデバイスをセット

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//キューブの設定
	//デバイスをセット
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();

	//メタボール
	//デバイスをセット
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//障害物
	Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());

	//床
	Floor::SetDevice(dxCommon_->GetDevice());
	Floor::SetCamera(camera_.get());

	//テキストのオブジェクト
	TextObject::SetDevice(dxCommon_->GetDevice());
	TextObject::SetCamera(camera_.get());

#pragma endregion

#pragma region キューブモデルの設定

	//モデルの設定
	//床のモデル
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 0.2f, 0.0f, 0.0f,0.5f });
	//hitboxのモデル
	CubeModel* newCubeModel1 = new CubeModel();
	newCubeModel1->CreateBuffers(dxCommon_->GetDevice());
	hitBoxModel.reset(newCubeModel1);
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

#pragma region 床の設定

	//床初期化
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

#pragma region プレイヤー初期化

	//プレイヤーセット
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//プレイヤー初期化
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

#pragma region 障害物初期化

	//ファイル読み込み
	std::stringstream obstaclePosList;	//文字列
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/obstacleTutorial.csv");
	//ファイルの内容をコピー
	obstaclePosList << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	//ファイルから障害物の場所を読み込み
	while (getline(obstaclePosList, line, '{'))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//カンマ区切りで先頭文字列を取得
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		obstaclePos.push_back(pos);
	}
	//障害物初期化
	for (int i = 1; i <= obstacleVal; i++)
	{
		std::unique_ptr<Obstacle>newObstacle = std::make_unique<Obstacle>();
		newObstacle->SetModel(stoneModel);
		newObstacle->SetCubeModel(hitBoxModel.get());
		//ファイル読み込みで得た座標を代入
		newObstacle->SetPosition({ 0,0,-10000 });
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region テキストのオブジェクト
	for (int i = 0; i < textObjectVol; i++)
	{
		std::unique_ptr<TextObject>newTextObject = std::make_unique<TextObject>();
		if (i == 0)    //タイトル
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,30,0 });
		}
		if (i == 1)    //タイトル(逆)
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 2)    //チュートリアル
		{
			newTextObject->SetModel(stageTutoModel);
			newTextObject->SetPosition({ 150,30,0 });
		}
		if (i == 3)    //ステージ1
		{
			newTextObject->SetModel(stage1Model);
			newTextObject->SetPosition({ 160,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 4)    //ステージ2
		{
			newTextObject->SetModel(stage2Model);
			newTextObject->SetPosition({ 300,30,0 });
		}
		if (i == 5)    //ステージ3
		{
			newTextObject->SetModel(stage3Model);
			newTextObject->SetPosition({ 300,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
		}
		if (i == 6)    //ステージ4
		{
			newTextObject->SetModel(stage4Model);
			newTextObject->SetPosition({ 450,30,0 });
		}
		if (i == 7)    //ステージ５
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

#pragma region ゴール初期化

	//デバイスとカメラセット
	Goal::SetDevice(dxCommon_->GetDevice());
	Goal::SetCamera(camera_.get());
	//ゴール初期化
	Goal* newGoal = new Goal();
	newGoal->SetModel(goalModel);
	newGoal->SetCubeModel(hitBoxModel.get());
	newGoal->Initialize();
	newGoal->SetScale({5.0f,5.0f ,5.0f });
	goal.reset(newGoal);

#pragma endregion 

#pragma region 鍵初期化

	//デバイスとカメラセット
	Key::SetDevice(dxCommon_->GetDevice());
	Key::SetCamera(camera_.get());
	//ゴール初期化
	Key* newKey = new Key();
	newKey->SetModel(keyModel);
	newKey->SetCubeModel(hitBoxModel.get());
	newKey->Initialize();
	key.reset(newKey);

#pragma endregion

#pragma region スプライト
	//スプライト初期化処理
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
	//コントローラー更新
	dxInput->InputProcess();


	//シーンごとの処理
	(this->*Scene_[scene_])();

	//デバッグ用 キー入力でステージ変更
	if (input_->TriggerKey(DIK_0))stage = Stage::Tutorial;
	if (input_->TriggerKey(DIK_1))stage = Stage::Stage1;
	if (input_->TriggerKey(DIK_2))stage = Stage::Stage2;
	if (input_->TriggerKey(DIK_3))stage = Stage::Stage3;
	if (input_->TriggerKey(DIK_4))stage = Stage::Stage4;
	if (input_->TriggerKey(DIK_5))stage = Stage::Stage5;

	//前のシーンと今のシーンが違かったらリセット処理
	if (scene_ != preScene_)
	{
		//タイトルをセット
		if (scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//前のステージと現在のステージが違かったらリセット処理
	if (stage != preStage)
	{
		//チュートリアルに移動した場合
		if (stage == Stage::Tutorial)	SetTutorial();
		if (stage == Stage::Stage1)		SetStage1();
		if (stage == Stage::Stage2)		SetStage2();
		if (stage == Stage::Stage3)		SetStage3();
		if (stage == Stage::Stage4)		SetStage4();
		if (stage == Stage::Stage5)		SetStage5();
	}

	//前のフレームのシーン取得
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


	//プレイヤー更新
	player->Update();
	player2->Update();

	//床更新
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//テキストのオブジェクト更新
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Update();
	}

	camera_->StageSelect(player->GetPosition0(), player->GetPlayerState());
	//カメラ更新
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
	//プレイヤー描画
	if (player->GetMoveFlag() == true) {
		player->Draw(dxCommon_->GetCommandList());
	}
	else {
		player2->Draw(dxCommon_->GetCommandList());
	}
	//床描画
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}
	//テキストのオブジェクト描画
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

	//デバッグ
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
	//カメラ更新
	camera_->Update();

	//スペースを押したら指定してるマップに更新
	/*DebugLoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);*/

	//プレイヤー更新
	player->Update();
	player2->Update();
	//ゴール更新
	goal->Update();
	//鍵更新
	key->Update();
	//床更新
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}
	//障害物更新
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
	//プレイヤー描画
	if (player->GetMoveFlag() == true) {
		player->Draw(dxCommon_->GetCommandList());
	}
	else {
		player2->Draw(dxCommon_->GetCommandList());
	}
	//ゴール描画
	if (player->GetKeyFlag())
	{
		goal->Draw(dxCommon_->GetCommandList());
	}
	//鍵描画
	if (player->GetKeyFlag() == false)
	{
		key->Draw(dxCommon_->GetCommandList());
	}
	//障害物描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	//床描画
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		//カメラ前の時
		if (camera_->GetMode() == Camera::forward)
		{
			if (i != 6)floor->Draw(dxCommon_->GetCommandList());
		}
		//カメラ右の時
		if (camera_->GetMode() == Camera::right)
		{
			if (i != 2)floor->Draw(dxCommon_->GetCommandList());
		}
		//カメラ後ろの時
		if (camera_->GetMode() == Camera::backward)
		{
			if (i != 5)floor->Draw(dxCommon_->GetCommandList());
		}
		//カメラ左の時
		if (camera_->GetMode() == Camera::left)
		{
			if (i != 1)floor->Draw(dxCommon_->GetCommandList());
		}

		i++;
	}

	//テキストのオブジェクト更新
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Update();
	}

	//テキストのオブジェクト描画
	int j = 0;
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		if (j == 21 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 22 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 23 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 24 && stage == Stage::Tutorial)textObject->Draw(dxCommon_->GetCommandList());
		j++;
	}
	//鍵を取得したら描画
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
	//プレイヤーセット
	player->SetTitle();
	player2->SetTitle();
	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();

	int i = 0;
	for (std::unique_ptr<TextObject>& textObj : textObjects)
	{
		if (i == 0)	//タイトル
		{
			textObj->SetPosition({ 0,30,0 });
		}
		if (i == 1)	//ステージ1
		{
			textObj->SetPosition({ 0,-30,0 });
			textObj->SetRotation({ PI,0,0 });
		}
		i++;
	}

	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		j++;
	}

}

void GameScene::SetTutorial()
{	
	//プレイヤーセット
	player->SetTutorial();
	player2->SetTutorial();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);
	//ゴールセット
	goal->SetTutorial();
	//鍵セット
	key->SetTutorial();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();
	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}

	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage1()
{
	//プレイヤーセット
	player->SetStage();
	player2->SetStage();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial1.csv", tutorialObstacleVal1);
	//ゴールセット
	goal->SetTutorial();
	//鍵セット
	key->SetStage1();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();
	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage2()
{
	//プレイヤーセット
	player->SetStage2();
	player2->SetStage2();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial2.csv", tutorialObstacleVal2);
	//ゴールセット
	goal->SetStage2();
	//鍵セット
	key->SetStage2();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();
	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage3()
{

	//プレイヤーセット
	player->SetStage();
	player2->SetStage();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial3.csv", tutorialObstacleVal3);
	//ゴールセット
	goal->SetTutorial();
	//鍵セット
	key->SetStage3();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();

	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage4()
{

	//プレイヤーセット
	player->SetStage();
	player2->SetStage();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial4.csv", tutorialObstacleVal4);
	//ゴールセット
	goal->SetStage4();
	//鍵セット
	key->SetStage4();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();

	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage5()
{
	
	//プレイヤーセット
	player->SetStage();
	player2->SetStage();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial5.csv", 246);
	//ゴールセット
	goal->SetTutorial();
	goal->SetPosition({ -50.0f,3.0f,-50.0f });
	//鍵セット
	key->SetStage5();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	player2->ClearCollision();
	player->SetPosition0({ -50, 3,-50 });
	player2->SetPosition0({ -50, 3,-50 });

	//床セット
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

		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player2->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床

		i++;
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
		player2->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::LoadCsv(const wchar_t* fileName, int obstacleVal)
{
	//ファイル読み込み
	std::stringstream obstaclePosList;	//文字列
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//ファイルを開く
	std::ifstream file;
	file.open(fileName);
	//ファイルの内容をコピー
	obstaclePosList << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	//ファイルから障害物の場所を読み込み
	while (getline(obstaclePosList, line, '{'))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//カンマ区切りで先頭文字列を取得
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
		//ファイル読み込み
		std::stringstream obstaclePosList;	//文字列
		std::vector<DirectX::XMFLOAT3>obstaclePos;
		//ファイルを開く
		std::ifstream file;
		file.open(fileName);
		//ファイルの内容をコピー
		obstaclePosList << file.rdbuf();
		//ファイルを閉じる
		file.close();

		std::string line;

		//ファイルから障害物の場所を読み込み
		while (getline(obstaclePosList, line, '{'))
		{
			//1行分の文字列をストリームに変換して解析しやすくする
			std::istringstream line_stream(line);
			std::string word1;
			std::string word2;
			std::string word3;
			//カンマ区切りで先頭文字列を取得
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
