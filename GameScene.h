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
	//�����o�֐�
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�^�C�g���V�[��
	void TitleUpdate();
	void TitleDraw();
	//�Q�[���V�[��
	void GameUpdate();
	void GameDraw();

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	
	//�J����
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;

	//��̃��f��
	/*FbxModel* stoneModel = nullptr;*/

	//�L���[�u
	std::unique_ptr<CubeModel> cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//�v���C���[
	std::unique_ptr<Player> player;

	//��Q��
	/*std::unique_ptr<Obstacle>obstacle;*/

	//�X�v���C�g
	Sprite* sprite = new Sprite;
	Sprite titleSprite;	//title.png
	//�X�v���C�g���ʃf�[�^����
	SpriteCommon spriteCommon;

	//�V�[��
	enum class Scene
	{
		Title,	//�^�C�g��
		Game,	//�Q�[��
	};
	//�V�[��	�ŏ����^�C�g��
	size_t scene_ = static_cast<size_t>(Scene::Game);
	//�����o�֐��̃|�C���^�e�[�u��
	static void (GameScene::* Scene_[])();

	//�V�[���̕`��
	enum class SceneDraw
	{
		TitleDraw,	//�^�C�g��
		GameDraw,	//�Q�[��
	};
	//�V�[��	�ŏ����^�C�g��
	size_t sceneDraw_ = static_cast<size_t>(SceneDraw::GameDraw);
	//�����o�֐��̃|�C���^�e�[�u��
	static void (GameScene::* SceneDraw_[])();
};

