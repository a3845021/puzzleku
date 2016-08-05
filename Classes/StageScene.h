#pragma once

#include "stdafx.h"

class StageScene : public Layer
{
private:
	Sprite* _credit;

	Sprite* _popup;
	Sprite* _yes;
	Sprite* _no;

	bool _event = false;

	Layer* buttonLayer;
	void SetStage(int stage);

	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

public:
	void showExit();
	void hideExit();

	static Scene* createScene();
	bool init()	   override;
	void onEnter() override;
	CREATE_FUNC(StageScene);
};