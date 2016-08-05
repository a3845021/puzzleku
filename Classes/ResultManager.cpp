#include "ResultManager.h"
#include "StageScene.h"

#define KEY_GOAL_SCORE "GOAL_STAGE"
#define KEY_SLIDE_COUNT "SLIDE_COUNT"
#define KEY_GAME_TIME "GAME_COUNT"
#define KEY_BOOM_COUNT "KEY_BOOM_COUNT"
//-----------------------------------
#define KEY_IS_SOUND_PLAY "SOUND_PLAY"
#define KEY_IS_FX_PLAY "FX_PLAY"

ResultManager::ResultManager()
{

}

ResultManager::~ResultManager()
{

}

bool ResultManager::init()
{
	if (!Layer::init())
		return false;

	return true;
}

void ResultManager::GameOver()
{
	auto action = ScaleTo::create(0.4f, 1.0f);

	_popup = Sprite::create("popup/FailBG.png");
	_popup->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(_popup);

	auto popupPos = _popup->getBoundingBox().size;


	auto stage = MenuItemImage::create("popup/Stage.png", "popup/Stage_press.png",
		[&](Ref* sender)
	{
		OnMenu(1);
	});
	stage->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2 - 80);

	auto retry = MenuItemImage::create("popup/rebut.png", "popup/rebut1.png",
		[&](Ref*sender)
	{
		OnMenu(2);
	});
	retry->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2 - 155);
	
	auto menu = Menu::create(retry, stage, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_score = LabelTTF::create("0", "thePudding.ttf", 30);
	_score->setAnchorPoint(Vec2(0, 0.5));
	_score->setPosition(popupPos.width * 0.45, popupPos.height * 0.6);
	_score->setColor(Color3B::BLACK);
	_popup->addChild(_score, 100);

	_highScore = LabelTTF::create("0", "thePudding.ttf", 30);
	_highScore->setPosition(popupPos.width  * 0.45, popupPos.height * 0.5);
	_highScore->setAnchorPoint(Vec2(0, 0.5));
	_highScore->setColor(Color3B::BLACK);
	_popup->addChild(_highScore, 100);

	_popup->runAction(action);
}

void ResultManager::GameClear()
{
	auto action = ScaleTo::create(0.4f, 1.0f);

	_popup = Sprite::create("popup/ClearBG.png");
	_popup->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2);
	this->addChild(_popup);

	Size popupPos = _popup->getBoundingBox().size;

	auto stage = MenuItemImage::create("popup/Stage.png", "popup/Stage_press.png",
		[&](Ref* sender)
	{
		OnMenu(1);
	});
	stage->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT /2 - 80);

	auto next = MenuItemImage::create("popup/Next.png", "popup/Next_press.png", 
		[&](Ref*sender)
	{
		OnMenu(3);
	});
	next->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT /2 - 155);

	auto menu = Menu::create(next, stage, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	_score = LabelTTF::create("0", "thePudding.ttf", 30);
	_score->setAnchorPoint(Vec2(0, 0.5));
	_score->setPosition(popupPos.width * 0.45, popupPos.height * 0.6);
	_score->setColor(Color3B::BLACK);
	_popup->addChild(_score, 100);

	_highScore = LabelTTF::create("0", "thePudding.ttf", 30);
	_highScore->setPosition(popupPos.width  * 0.45, popupPos.height * 0.5);
	_highScore->setAnchorPoint(Vec2(0, 0.5));
	_highScore->setColor(Color3B::BLACK);
	_popup->addChild(_highScore, 100);

	_popup->runAction(action);
}

void ResultManager::OnMenu(int i)
{
	switch (i)
	{
	case 1:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, StageScene::createScene()));
		break;

	case 2:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene()));
		break;

	case 3:
		NowStage(_stageValue + 1);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene()));
		break;
	}
}

int ResultManager::NowStage(int stage)
{
	auto userDefault = UserDefault::getInstance();

	switch (stage)
	{
	case 1:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 1);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 10000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 0);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 0);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);
		break;

	case 2:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 2);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 4000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 50);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 20);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);
		break;

	case 3:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 3);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 10000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 80);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 30);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);		break;

	case 4:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 3);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 12000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 150);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 40);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);		break;

	case 5:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 5);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 15000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 300);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 50);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);		break;

	case 6:
		userDefault->setIntegerForKey(KEY_STAGE_NUM, 6);
		userDefault->setIntegerForKey(KEY_GOAL_SCORE, 30000);
		userDefault->setIntegerForKey(KEY_GAME_TIME, 300);
		userDefault->setIntegerForKey(KEY_SLIDE_COUNT, 100);
		userDefault->setIntegerForKey(KEY_BOOM_COUNT, 0);		break;
	}
	return 0;
}

void ResultManager::SetScore(int score)
{	
	_score->setString(StringUtils::format("%d", score));
}

void ResultManager::SetHighScore(int score)
{
	_highScore->setString(StringUtils::format("%d", score));
}