#pragma once
#include "stdafx.h"

class Tutorial : public Layer
{
private:
	int cnt;
	Sprite* background;
public:
	static Scene* createScene()
	{
		auto scene = Scene::create();
		auto layer = Tutorial::create();
		scene->addChild(layer);
		return scene;
	}

	bool init() override
	{
		this->setTouchEnabled(true);
		this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

		background = Sprite::create("tutorial/1.png");
		background->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2));
		this->addChild(background);
		cnt = 1;
		return true;
	}
	bool onTouchBegan(Touch *touch, Event *unused_event) override
	{
		cnt++;
		switch (cnt)
		{
		case 2:
			background->setTexture("tutorial/2.png");
			break;
		case 3:
			background->setTexture("tutorial/3.png");
			break;
		case 4:
			background->setTexture("tutorial/4.png");
			break;
		case 5:
			Director::getInstance()->replaceScene(TransitionFade::create(0.4f,GameScene::createScene()));
			break;
		}

		return true;
	}

	CREATE_FUNC(Tutorial);
};
