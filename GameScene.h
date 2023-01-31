#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "FbxObject3D.h"
#include "CubeObject3D.h"
#include "CubeModel.h"
#include "Metaball.h"
#include "Collision.h"
#include "Camera.h"
#include "Player.h"
#include "Obstacle.h"
#include "Sprite.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//更新
	void Update();
	//描画
	void Draw();
	//タイトルシーン
	void TitleUpdate();
	void TitleDraw();
	//ゲームシーン
	void GameUpdate();
	void GameDraw();

	//スペースキーでファイル読み込みを実行する関数
	void LoadCsv(const wchar_t* fileName);

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	
	//カメラ
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model1 = nullptr;

	//岩のモデル
	FbxModel* stoneModel = nullptr;

	//キューブ
	std::unique_ptr<CubeModel> cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//キューブ(hitbox用)
	std::unique_ptr<CubeModel> hitBoxModel;

	//プレイヤー
	std::unique_ptr<Player> player;

	//障害物
	std::list<std::unique_ptr<Obstacle>> obstacles;
	//障害物の数
	size_t obstacleVal = 4;

	//スプライト
	Sprite* sprite = new Sprite;
	Sprite titleSprite;	//title.png
	//スプライト共通データ生成
	SpriteCommon spriteCommon;

	//シーン
	enum class Scene
	{
		Title,	//タイトル
		Game,	//ゲーム
	};
	//シーン	最初がタイトル
	size_t scene_ = static_cast<size_t>(Scene::Title);
	//メンバ関数のポインタテーブル
	static void (GameScene::* Scene_[])();

	//シーンの描画
	enum class SceneDraw
	{
		TitleDraw,	//タイトル
		GameDraw,	//ゲーム
	};
	//シーン	最初がタイトル
	size_t sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
	//メンバ関数のポインタテーブル
	static void (GameScene::* SceneDraw_[])();

	//ステージを分ける列挙型
	enum Stage
	{
		Title,
		Tutorial,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
	};
	//ステージ
	Stage stage = Stage::Title;
	//前のフレームのステージ
	Stage preStage = Stage::Title;
};

