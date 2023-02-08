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
	stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");
	goalModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/beast.png");
	keyModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/key.png");
	titleModel = FbxLoader::GetInstance()->LoadModelFromFile("title", "Resources/white1x1.png");
	stage1Model = FbxLoader::GetInstance()->LoadModelFromFile("stage1", "Resources/white1x1.png");

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
	cubeModel->SetImageData({ 0.3f, 0.0f, 0.0f,0.7f });
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
		newObstacle->SetPosition({0,0,-10000});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region テキストのオブジェクト
	for (int i = 0; i < textObjectVol; i++)
	{
		std::unique_ptr<TextObject>newTextObject = std::make_unique<TextObject>();
		if (i == 0)	//タイトル
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,30,0 });
		}
		if (i == 1)	//ステージ1
		{
			newTextObject->SetModel(titleModel);
			newTextObject->SetPosition({ 0,-30,0 });
			newTextObject->SetRotation({ PI,0,0 });
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

	//前のシーンと今のシーンが違かったらリセット処理
	if (scene_ != preScene_)
	{
		//タイトルをセット
		if(scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//前のステージと現在のステージが違かったらリセット処理
	if (stage != preStage)
	{
		//チュートリアルに移動した場合
		if (stage == Stage::Tutorial)	SetTutorial();
		if (stage == Stage::Stage1)		SetStage1();
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
	if (input_->PushKey(DIK_SPACE))
	{
		scene_ = static_cast<size_t>(Scene::Game);
		sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
		stage = Stage::Tutorial;
	}


	//プレイヤー更新
	player->Update();
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
	//プレイヤー描画
	player->Draw(dxCommon_->GetCommandList());
	//床描画
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Draw(dxCommon_->GetCommandList());
	}
	//テキストのオブジェクト描画
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Draw(dxCommon_->GetCommandList());
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
	camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());
	//カメラ更新
	camera_->Update();

	//スペースを押したら指定してるマップに更新
	/*DebugLoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);*/

	//プレイヤー更新
	player->Update();
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
}

void GameScene::GameDraw()
{
	//プレイヤー描画
	player->Draw(dxCommon_->GetCommandList());
	//ゴール描画
	if (player->GetKeyFlag())
	{
		goal->Draw(dxCommon_->GetCommandList());
	}
	//鍵描画
	key->Draw(dxCommon_->GetCommandList());
	//障害物描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	//床描画
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i != 6)
		{
			floor->Draw(dxCommon_->GetCommandList());
		}
		i++;
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
	//プレイヤーセット
	player->SetTitle();
	//プレイヤーの当たり判定をリセット
	player->ClearCollision();

	int i = 0;
	for (std::unique_ptr<TextObject>& textObj: textObjects)
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
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->SetScale({ 360,0.5,30 });
		floor->SetPosition({ 180,0,0 });
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
	}

}

void GameScene::SetTutorial()
{
	//プレイヤーセット
	player->SetTutorial();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial.csv", tutorialObstacleVal);
	//ゴールセット
	goal->SetTutorial();
	//鍵セット
	key->SetTutorial();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
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

		i++;
	}

	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}
}

void GameScene::SetStage1()
{
	//プレイヤーセット
	player->SetTutorial();
	//障害物読み込み
	LoadCsv(L"Resources/obstacleStage1.csv", stage1ObstacleVal);
	//ゴールセット
	goal->SetTutorial();
	//鍵セット
	key->SetTutorial();

	//プレイヤーの当たり判定をリセット
	player->ClearCollision();
	//床セット
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->SetScale({ 120,0.5,120 });
		floor->SetPosition({ 0,0,0 });
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
	}
	player->SetCollisionKey(key->GetPosition(), key->GetScale());	//鍵
	player->SetCollisionGoal(key->GetPosition(), key->GetScale());	//ゴール
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
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
