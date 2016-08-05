#pragma once

#include "cocos2d.h"
#include <vector>
#include <map>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <random>
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

#include "AppDelegate.h"
#include "GameScene.h"
#include "StageScene.h"
#include "MainMenu.h"
#include "LogoScene.h"
#include "Tutorial.h"
#include "CreditScene.h"

inline void pauseRecursive(Node* node, bool pause)
{
	if (node->getTag() == 0x123)
		return;

	if (pause)
		node->pause();
	else
		node->resumeSchedulerAndActions();

	Ref* child = nullptr;
	auto children = node->getChildren();

	for (size_t i = 0; i<children.size(); i++)
	{
		pauseRecursive(children.at(i), pause);
	}
}