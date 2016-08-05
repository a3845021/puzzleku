#pragma once

#include "stdafx.h"

class LogoScene : public LayerColor
{
private:
	Sprite* _logo;

private:
	void ChangeScene();

public:
	static Scene* createScene();
	bool init() override;
	CREATE_FUNC(LogoScene);
};