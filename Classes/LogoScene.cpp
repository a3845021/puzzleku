#include "LogoScene.h"
#include "StageScene.h"

Scene* LogoScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LogoScene::create();
	scene->addChild(layer);
	return scene;
}

bool LogoScene::init()
{
	if (!LayerColor::initWithColor(Color4B::WHITE))
		return false;

	_logo = Sprite::create("logo/logo.png");
	_logo->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	_logo->setScale(2.0f);
	this->addChild(_logo);

	_logo->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(LogoScene::ChangeScene, this)),NULL));

	return true;
}

void LogoScene::ChangeScene()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainMenu::createScene()));
}