#include "StageScene.h"

Scene* StageScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StageScene::create();
	scene->addChild(layer);
	return scene;
}

void StageScene::onEnter()
{
	Layer::onEnter();

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->setKeyboardEnabled(true);

	auto key = EventListenerKeyboard::create();
	key->onKeyPressed = CC_CALLBACK_2(StageScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(key, this);
}

bool StageScene::init()
{
	if (!Layer::init())
		return false;
	_event = false;
	buttonLayer = Layer::create();
	buttonLayer->setPosition(Vec2(0,-169));
	this->addChild(buttonLayer,2);

	Sprite* background = Sprite::create("stage/background.png");
	background->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(background, 1);

	auto action = MoveBy::create(0.5f, Vec2(0, 10.0f));
	auto action2 = MoveBy::create(0.5f, Vec2(0, -10.0f));
	for (int i = 0; i < 6; i++)
	{	
		MenuItemImage* button = MenuItemImage::create(StringUtils::format("stage/%d.png", i).c_str(), StringUtils::format("stage/%dd.png", i).c_str(),
			[=](Ref* sender)
		{
			this->SetStage(i);
		});
		if (i % 2 != 0)
			button->setPosition(D_DESIGN_WIDTH * 0.6, D_DESIGN_HEIGHT * (0.2 * i) + 50);
		else
			button->setPosition(D_DESIGN_WIDTH * 0.4, D_DESIGN_HEIGHT * 0.2 * i);


		Menu* menu = Menu::create(button, nullptr);
		menu->setPosition(Vec2::ZERO);
		buttonLayer->addChild(menu, 2);
	}

	_credit = Sprite::create("credit/credit_button.png");
	_credit->setPosition(D_DESIGN_WIDTH * 0.93, D_DESIGN_HEIGHT * 0.95);
	_credit->setGlobalZOrder(100);
	this->addChild(_credit,6);

	_popup = Sprite::create("popup/ExitBG.png");
	_popup->setScale(0);
	_popup->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(_popup,5);

	auto popupSize = _popup->getContentSize();

	_yes = Sprite::create("popup/Yes.png");
	_yes->setPosition(popupSize.width * 0.27, popupSize.height * 0.25);
	_popup->addChild(_yes);

	_no = Sprite::create("popup/No.png");
	_no->setPosition(popupSize.width * 0.72, popupSize.height * 0.25);
	_popup->addChild(_no);
	return true;
}

void StageScene::showExit()
{
	_event = true;
	auto action = ScaleTo::create(0.3, 1.0f);
	_popup->runAction(action);
}

void StageScene::hideExit()
{
	_event = false;
	auto action = ScaleTo::create(0.3, 0.0f);
	_popup->runAction(action);
	_yes->setTexture("popup/Yes.png");
	_no->setTexture("popup/No.png");
}

void StageScene::SetStage(int stage)
{
	auto userDefault = UserDefault::getInstance();

	switch (stage)
	{
	case 1:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 1);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 10000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 0);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 0);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 1);

		Director::getInstance()->replaceScene(TransitionFade::create(0.4f, Tutorial::createScene()));

		break;

	case 2:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 2);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 4000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 50);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 20);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 2);;
		break;

	case 3:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 3);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 10000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 80);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 30);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 2);
		break;

	case 4:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 3);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 12000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 150);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 40);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 2);
		break;

	case 5:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 5);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 15000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 300);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 50);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 1);
		break;

	case 6:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 6);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 30000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 300);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 100);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 4);
		break;
	}

	if (stage != 1){
		Director::getInstance()->replaceScene(TransitionFade::create(0.4f, GameScene::createScene()));
	}
}

bool StageScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.1, 0.8);
	if (!_event && _credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_credit->setOpacity(180);
		_credit->runAction(action);
	}

	if (_event && _yes->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_yes->setTexture("popup/Yes_press.png");
	}
	else if (_event && _no->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_no->setTexture("popup/No_press.png");
	}
	return true;
}

void StageScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	log("%.f", buttonLayer->getPosition().y);
	buttonLayer->setPositionY(buttonLayer->getPositionY() + touch->getDelta().y);
	auto action = ScaleTo::create(0.1, 1.0);

	if (!_event && !_credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_credit->setOpacity(255);
		_credit->runAction(action);
	}

	if (_event && !_yes->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_yes->setTexture("popup/Yes.png");
	}
	else if (_event && !_no->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		_no->setTexture("popup/No.png");
	}
}

void StageScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (_event && _yes->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		Director::getInstance()->end();
	}
	else if (_event && _no->getBoundingBox().containsPoint(_popup->convertTouchToNodeSpace(touch)))
	{
		hideExit();
	}
	if (!_event && _credit->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_eventDispatcher->removeAllEventListeners();
		Director::getInstance()->replaceScene(TransitionFade::create(0.4, CreditScene::createScene()));
	}
}

void StageScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (!_event)
			showExit();
		else
			hideExit();
	}
}