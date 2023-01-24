#pragma once
#include "Input.h"
#include "DirectXCommon.h"

class GameScene
{
	//ƒƒ“ƒoŠÖ”
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();
	void Draw();
};

