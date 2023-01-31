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

#pragma endregion

#pragma region キューブモデルの設定

	//モデルの設定
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	hitBoxModel.reset(newCubeModel);
	//床のモデル
	cubeModel->SetImageData({ 1.0f, 0.0f, 0.0f,1.0f });
	//hitboxのモデル
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

	//キューブオブジェクトの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel.get());
	cubeObject->SetScale({ 120.0f,0.5f,120.0f });
	cubeObject->SetPosition({ 0.0f,0.0f,0.0f });

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
		newObstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion


	//プレイヤーに当たり判定をセット
	player->SetCollisionFloor(cubeObject->GetPosition(), cubeObject->GetScale());	//床
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


	//スプライト初期化処理
	spriteCommon = sprite->SpriteCommonCreate(dxCommon_->GetDevice(), 1280, 720);
	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/title.png", dxCommon_->GetDevice());
	titleSprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	titleSprite.SetTexNumber(0);
	titleSprite.SetPosition(XMFLOAT3(200, 100, 0));
	titleSprite.SetScale(XMFLOAT2(414 * 2.2, 54 * 2.2));
}

void GameScene::Update()
{
	//コントローラー更新
	dxInput->InputProcess();
	//シーンごとの処理
	(this->*Scene_[scene_])();

	//前のステージと現在のステージが違かったらリセット処理
	if (stage != preStage)
	{
		if (stage == Stage::Tutorial)
		{
			player->SetTutorial();
			LoadCsv(L"Resources/obstacleTutorial.csv");
		}
	}

	//前のフレームのシーン取得
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

	//タイトルのスプライト更新
	titleSprite.SpriteTransferVertexBuffer(titleSprite);
	titleSprite.SpriteUpdate(titleSprite, spriteCommon);
}

void GameScene::TitleDraw()
{
	//スプライト共通コマンド
	sprite->SpriteCommonBeginDraw(dxCommon_->GetCommandList(), spriteCommon);

	titleSprite.SpriteDraw(dxCommon_->GetCommandList(), spriteCommon, dxCommon_->GetDevice(), titleSprite.vbView);
}

void GameScene::GameUpdate()
{
	camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());
	//カメラ更新
	camera_->Update();

	//キューブオブジェクト更新
	cubeObject->Update();
	//プレイヤー更新
	player->Update();
	//障害物更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}
}

void GameScene::GameDraw()
{
	//キューブ描画
	cubeObject->Draw(dxCommon_->GetCommandList());
	//プレイヤー描画
	player->Draw(dxCommon_->GetCommandList());
	//障害物描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::LoadCsv(const wchar_t* fileName)
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
