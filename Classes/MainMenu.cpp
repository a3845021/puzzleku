#include "MainMenu.h"

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);
	return scene;
}

bool MainMenu::init()
{
	if (!Layer::init())
		return false;

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->setKeyboardEnabled(true);

	Sprite* background = Sprite::create("main/background.png");
	background->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2));
	this->addChild(background);

	Sprite* touch = Sprite::create("main/touch.png");
	touch->setPosition(Vec2(D_DESIGN_WIDTH / 2, 300));
	this->addChild(touch);

	touch->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1.0f), FadeIn::create(1.0f), nullptr)));

	_popup = Sprite::create("popup/ExitBG.png");
	_popup->setScale(0);
	_popup->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(_popup);

	auto popupSize = _popup->getContentSize();

	log("%f, %f", popupSize.width, popupSize.height);

	_yes = Sprite::create("popup/Yes.png");
	_yes->setPosition(popupSize.width * 0.27, popupSize.height * 0.25);
	_popup->addChild(_yes);

	_no = Sprite::create("popup/No.png");
	_no->setPosition(popupSize.width * 0.72, popupSize.height * 0.25);
	_popup->addChild(_no);
	return true;
}

bool MainMenu::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (_event && _yes->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_yes->setTexture("popup/Yes_press.png");
	}
	else if (_event && _no->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_no->setTexture("popup/No_press.png");
	}

	if (!_event)
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("background_sound.mp3", true);
		UserDefault::getInstance()->setBoolForKey("BG_SOUND", true);
		UserDefault::getInstance()->setBoolForKey("EF_SOUND", true);
		Director::getInstance()->replaceScene(TransitionFade::create(0.4f, StageScene::createScene()));
	}
	return true;
}

void MainMenu::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (_event && !_yes->getBoundingBox().containsPoint(_popup->convertToNodeSpace(touch->getLocation())))
	{
		_yes->setTexture("popup/Yes.png");
	}
	else if (_event && !_no->getBoundingBox().containsPoint(_popup->convertToNodeSpace(touch->getLocation())))
	{
		_no->setTexture("popup/No.png");
	}
}

void MainMenu::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!_event)
		Director::getInstance()->replaceScene(TransitionFade::create(0.4f, StageScene::createScene()));

	if (_event && _yes->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		Director::getInstance()->end();
	}
	else if (_event && _no->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		hideExit();
	}
}
void MainMenu::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_BACK:
		if (!_event)
			showExit();
		else
			hideExit();
		break;
	}
}

void MainMenu::showExit()
{
	_event = true;
	auto action = ScaleTo::create(0.3, 1.0f);
	_popup->runAction(action);
}

void MainMenu::hideExit()
{
	_event = false;
	auto action = ScaleTo::create(0.3, 0.0f);
	_popup->runAction(action);
	_yes->setTexture("popup/Yes.png");
	_no->setTexture("popup/No.png");
}