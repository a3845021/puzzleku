#pragma once

#include "stdafx.h"

class CreditScene : public Layer
{
private:
	Sprite* _credit;
	Sprite* _back;

	EventListenerKeyboard* _keyListener;
	EventListenerTouchOneByOne* _touchListener;

private:
	void onEnter() override;

	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

public:
	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(CreditScene);
};