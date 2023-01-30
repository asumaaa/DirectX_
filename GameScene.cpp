#include "GameScene.h"
#include "FbxLoader.h"

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

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 0, -50 });

	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/white1x1.png");
	/*stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");*/

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//fbxのオブジェクト
	object1 = new FbxObject3D;
	object1->Initialize();
	object1->SetModel(model1);
	object1->SetScale({ 0.01,0.01,0.01 });
	object1->SetRotation({ 0.0,0.0,0.0 });
	object1->PlayAnimation();

	//キューブの設定
	//デバイスをセット
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();
	//モデルの設定
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 0.7f, 0.3f, 0.3f,0.1f });

	//キューブオブジェクトの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel.get());
	cubeObject->SetScale({ 30.0f,0.5f,30.0f });
	cubeObject->SetPosition({ 0.0f,0.0f,0.0f });

	//メタボール
	//デバイスをセット
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//プレイヤーセット
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//プレイヤー初期化
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	player.reset(newPlayer);

	//障害物
	/*Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());*/
	/*obstacle->SetModel(model1);*/
	/*obstacle->SetPosition({10.0f,0.0f,0.0f});*/

	//プレイヤーに当たり判定をセット
	player->SetCollision(cubeObject->GetPosition(), cubeObject->GetScale());


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
	/*camera_->PlayerAim(player->GetPosition0(), player->GetPlayerState());*/
	//カメラ更新
	camera_->Update();

	//fBXオブジェクト更新
	/*object1->PlayAnimation();*/
	object1->Update();
	//キューブオブジェクト更新
	cubeObject->Update();
	//プレイヤー更新
	player->Update();
	//障害物更新
	/*obstacle->Update();*/
}

void GameScene::GameDraw()
{
	object1->Draw(dxCommon_->GetCommandList());
	//キューブ描画
	cubeObject->Draw(dxCommon_->GetCommandList());
	//プレイヤー描画
	player->Draw(dxCommon_->GetCommandList());
	//障害物描画
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
