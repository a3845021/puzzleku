#pragma once

#include "stdafx.h"

class MainMenu : public Layer
{
private:
	Sprite* _popup;
	Sprite* _yes;
	Sprite* _no;
	bool _event = false;
public:
	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void showExit();
	void hideExit();

	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(MainMenu);
};