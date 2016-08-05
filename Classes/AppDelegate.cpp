#include "stdafx.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() 
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLView::createWithRect(D_GAME_NAME, cocos2d::Rect(0, 0, D_DESIGN_WIDTH, D_DESIGN_HEIGHT), D_WINDOWS_SHOW);	//!< 우리가 사용해야 할 함수
		director->setOpenGLView(glview);
	}
	glview->setDesignResolutionSize(D_DESIGN_WIDTH, D_DESIGN_HEIGHT, kResolutionShowAll);
	director->setDisplayStats(false);
	director->setAnimationInterval(1.0 / 60);
	auto scene = LogoScene::createScene();

	director->runWithScene(scene);

	return true;
}
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();
}
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();
}
