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
#include "Goal.h"
#include "key.h"
#include "Floor.h"
#include "TextObject.h"

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

	//�V�[�����Ƃ̃Z�b�g�֐�
	//�^�C�g�����Z�b�g
	void SetTitle();
	//�`���[�g���A�����Z�b�g
	void SetTutorial();
	void SetStage1();
	void SetStage2();
	void SetStage3();
	void SetStage4();
	void SetStage5();

	//�X�y�[�X�L�[�Ńt�@�C���ǂݍ��݂����s����֐�
	void LoadCsv(const wchar_t* fileName, int obstacleVal);
	void DebugLoadCsv(const wchar_t* fileName, int obstacleVal);

	//�����o�ϐ�
private:
	//�f�o�C�X��input
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	
	//�J����
	std::unique_ptr<Camera> camera_;

	//------------���f���֘A-------------
	//fbx
	FbxModel* model1 = nullptr;
	FbxModel* model2 = nullptr;
	//��̃��f��
	FbxModel* stoneModel = nullptr;
	//�S�[���̃��f��
	FbxModel* goalModel = nullptr;
	//���̃��f��
	FbxModel* keyModel = nullptr;

	//�X�e�[�W�Ƃ��^�C�g���̃��f��
	//�^�C�g��
	FbxModel* titleModel = nullptr;
	//stage1
	FbxModel* stage1Model = nullptr;
	//stage2
	FbxModel* stage2Model = nullptr;
	//stage3
	FbxModel* stage3Model = nullptr;
	//stage4
	FbxModel* stage4Model = nullptr;
	//stage5
	FbxModel* stage5Model = nullptr;
	//tutorial
	FbxModel* stageTutoModel = nullptr;
	//clear1
	FbxModel* clear1Model = nullptr;
	//instruction
	FbxModel* moveTextModel = nullptr;
	FbxModel* startTextModel = nullptr;
	FbxModel* jumpTextModel = nullptr;
	FbxModel* cameraTextModel = nullptr;
	FbxModel* zoomTextModel = nullptr;
	FbxModel* returnTextModel = nullptr;

	//�L���[�u
	std::unique_ptr<CubeModel> cubeModel;
	/*std::unique_ptr<CubeObject3D> cubeObject;*/

	//�L���[�u(hitbox�p)
	std::unique_ptr<CubeModel> hitBoxModel;

	//----------����N���X---------
	//�v���C���[
	std::unique_ptr<Player> player;
	std::unique_ptr<Player> player2;

	//��Q��
	std::list<std::unique_ptr<Obstacle>> obstacles;
	//��Q���̐�
	size_t obstacleVal = 350;

	//�S�[��
	std::unique_ptr<Goal>goal;
	//��
	std::unique_ptr<Key>key;
	//��
	//std::unique_ptr<Floor>floor;
	std::list<std::unique_ptr<Floor>>floors;
	size_t floorVol = 7;

	//�e�L�X�g�̃I�u�W�F�N�g
	std::list<std::unique_ptr<TextObject>>textObjects;
	size_t textObjectVol = 25;

	//---------------------------

	//�X�v���C�g
	Sprite* sprite = new Sprite;
	Sprite titleSprite;	//title.png
	Sprite keySprite;	//key.png
	//�X�v���C�g���ʃf�[�^����
	SpriteCommon spriteCommon;

	//�V�[��
	enum class Scene
	{
		Title,	//�^�C�g��
		Game,	//�Q�[��
	};
	//�V�[��	�ŏ����^�C�g��
	size_t scene_ = static_cast<size_t>(Scene::Title);
	//�O�̃t���[���̃V�[��
	size_t preScene_ = static_cast<size_t>(Scene::Title);
	//�����o�֐��̃|�C���^�e�[�u��
	static void (GameScene::* Scene_[])();

	//�V�[���̕`��
	enum class SceneDraw
	{
		TitleDraw,	//�^�C�g��
		GameDraw,	//�Q�[��
	};
	//�V�[��	�ŏ����^�C�g��
	size_t sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
	//�����o�֐��̃|�C���^�e�[�u��
	static void (GameScene::* SceneDraw_[])();

	//�X�e�[�W�𕪂���񋓌^
	enum Stage
	{
		Title,
		Tutorial,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
	};
	//�X�e�[�W
	Stage stage = Stage::Title;
	//�O�̃t���[���̃X�e�[�W
	Stage preStage = Stage::Title;

	//�X�e�[�W���Ƃ̏�Q���̐�
	//tutorial
	size_t tutorialObstacleVal = 10;
	size_t tutorialObstacleVal1 = 9;
	size_t tutorialObstacleVal2 = 21;
	size_t tutorialObstacleVal3 = 24;
	size_t tutorialObstacleVal4 = 126;
	size_t tutorialObstacleVal5 = 126;
	size_t stage1ObstacleVal = 5;

	float num = 0;

	//�N���A�[�t���b�O
	bool clearTutoFlag = false;
	bool clear1Flag = false;
	bool clear2Flag = false;
	bool clear3Flag = false;
	bool clear4Flag = false;
	bool clear5Flag = false;
	bool clear6Flag = false;
};