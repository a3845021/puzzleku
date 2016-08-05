#include "CreditScene.h"
#include "StageScene.h"

Scene* CreditScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CreditScene::create();
	scene->addChild(layer);
	return scene;
}

void CreditScene::onEnter()
{
	Layer::onEnter();

	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->onTouchBegan = CC_CALLBACK_2(CreditScene::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(CreditScene::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(CreditScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

	_keyListener = EventListenerKeyboard::create();
	_keyListener->onKeyReleased = CC_CALLBACK_2(CreditScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyListener, this);
}

bool CreditScene::init()
{
	if (!Layer::init())
		return false;

	_credit = Sprite::create("credit/credit.png");
	_credit->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(_credit);

	_back = Sprite::create("credit/credit_button.png");
	_back->setPosition(D_DESIGN_WIDTH * 0.93, D_DESIGN_HEIGHT * 0.95);
	this->addChild(_back);

	return true;
}

void CreditScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_BACK:
		_eventDispatcher->removeAllEventListeners();
		Director::getInstance()->replaceScene(TransitionFade::create(0.4f, StageScene::createScene()));
		break;
	}
}

bool CreditScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.1, 0.8);
	if (_back->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_back->setOpacity(180);
		_back->runAction(action);
	}

	return false;
}

void CreditScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto action = ScaleTo::create(0.1, 1.0);

	if (!_back->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_back->setOpacity(255);
		_back->runAction(action);
	}
}

void CreditScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (_back->getBoundingBox().containsPoint(touch->getLocation()))
	{
		_eventDispatcher->removeAllEventListeners();
		Director::getInstance()->replaceScene(TransitionFade::create(0.4, StageScene::createScene()));
	}
}