#pragma once
#include "stdafx.h"

class PauseMenu : public Layer
{
private:
	Sprite* background;
	Menu*   menu;


public:
	Sprite* bg_sound;
	Sprite* ef_sound;
	bool isContinue;
	Layer* ownerLayer;
	static PauseMenu *create(Layer* owner)
	{
		PauseMenu *ret = new PauseMenu();
		if (ret && ret->init())
		{
			ret->autorelease();
			ret->setLayer(owner);
			owner->addChild(ret);
			return ret;
		}
		else
		{
			CC_SAFE_DELETE(ret);
			return nullptr;
		}
	}
	void setLayer(Layer* target)
	{
		ownerLayer = target;
	}
	bool init() override
	{
		isContinue = false;
		background = Sprite::create("popup/PauseBG.png");
		background->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2));
		this->addChild(background);


		MenuItemImage* reset = MenuItemImage::create("popup/rebut.png", "popup/rebut1.png",
			[&](Ref* sender)
		{
			Director::getInstance()->replaceScene(TransitionFade::create(0.4f, GameScene::createScene()));
		});
		reset->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2 + 65 + 25);
		MenuItemImage* strt = MenuItemImage::create("popup/Next.png", "popup/Next_press.png", 
			[&](Ref* sender)
		{
			this->hide();
			isContinue = true;
		});
		strt->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2 +25);
		MenuItemImage* stage = MenuItemImage::create("popup/Stage.png", "popup/Stage_press.png", 
			[&](Ref* sender)
		{
			Director::getInstance()->replaceScene(TransitionFade::create(0.4f, StageScene::createScene()));
		});
		stage->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2 - 65+25);


		bg_sound = Sprite::create("popup/Sound_on.png");
		bg_sound->setPosition(Vec2(D_DESIGN_WIDTH / 2 - 78, D_DESIGN_HEIGHT / 2 - 65 -120-5));
		this->addChild(bg_sound);

		ef_sound = Sprite::create("popup/Sound_on.png");
		ef_sound->setPosition(Vec2(D_DESIGN_WIDTH / 2 + 78, D_DESIGN_HEIGHT / 2 - 65 - 120-5));
		this->addChild(ef_sound);

		menu = Menu::create(reset, strt, stage, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu);

		return true;
	}

	void hide()
	{
		background->setVisible(false);
		menu->setVisible(false);
		menu->setEnabled(false);

		bg_sound->setVisible(false);
		ef_sound->setVisible(false);
	}

	void show()
	{
		background->setVisible(true);
		menu->setVisible(true);
		menu->setEnabled(true);
		bg_sound->setVisible(true);
		ef_sound->setVisible(true);
	}
};