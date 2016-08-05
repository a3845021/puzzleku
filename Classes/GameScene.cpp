#include "stdafx.h"
#include "ResultManager.h"
#include "PauseMenu.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

GameScene::GameScene()
	: number(0),
	aim_score(UserDefault::getInstance()->getIntegerForKey(KEY_GOAL_SCORE)),
	score(0),
	blockScore(0),
	gameTimeBooster(false)
	, cur_score(0)
	, GameTimer(0)
	, Slidenumber(UserDefault::getInstance()->getIntegerForKey(KEY_SLIDE_COUNT))
	, GameTime(UserDefault::getInstance()->getIntegerForKey(KEY_GAME_TIME))
	, boomCnt(UserDefault::getInstance()->getIntegerForKey(KEY_BOOM_COUNT))
	, bonus(false)
	, nextBlock(false)
	, execute(true)
	, isPause(false)
	, bg_sound(UserDefault::getInstance()->getBoolForKey("BG_SOUND"))
	, ef_sound(UserDefault::getInstance()->getBoolForKey("EF_SOUND"))
{

}

GameScene::~GameScene()
{

}

bool GameScene::init()
{
	if (!Layer::init())
		return false;

	if (!bg_sound)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}


	auto listener2 = EventListenerKeyboard::create();
	listener2->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);
	// 터치 리스너 초기화
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	srand(time(NULL));

	// 배경 초기화
	_background = Sprite::create("game/background2.png");
	_background->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2));
	_background->setOpacity(150);
	this->addChild(_background);

	Sprite* background = Sprite::create("game/background3.png");
	background->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT / 2));
	this->addChild(background);
	// 혹시모를 초기화
	v_Block.clear();
	v_allBlock.clear();

	// 게임 시간
	if (GameTime == 0)
	{
		GodGameTime = true;
	}
	else
	{
		GodGameTime = false;
	}
	score_enable = false;

	if (Slidenumber == 0)
	{
		NoSlide = true;
	}
	else
	{
		NoSlide = false;
	}
	aim_score_Label = LabelTTF::create(StringUtils::format("%d", aim_score), "fonts/crayon.ttf", 60);
	aim_score_Label->setPosition(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT - 100);
	this->addChild(aim_score_Label);

	score_Label = LabelTTF::create("0", "fonts/crayon.ttf", 40);
	score_Label->setPosition(Vec2(D_DESIGN_WIDTH / 2, D_DESIGN_HEIGHT - 200));
	score_Label->setColor(Color3B::BLUE);
	this->addChild(score_Label);

	int bun = GameTime / 60;
	int cho = GameTime % 60;

	if (GodGameTime)
	{
		GameTimerLabel = LabelTTF::create(StringUtils::format("%d:%d/%d", bun, cho, Slidenumber), "fonts/crayon.ttf", 60);
		GameTimerLabel->setPosition(Vec2(D_DESIGN_WIDTH / 2, 110));
		GameTimerLabel->setColor(Color3B::ORANGE);
		this->addChild(GameTimerLabel);
	}
	else
	{
		GameTimerLabel = LabelTTF::create(StringUtils::format("%d:%d/%d", 0, 00, Slidenumber), "fonts/crayon.ttf", 60);
		GameTimerLabel->setPosition(Vec2(D_DESIGN_WIDTH / 2, 110));
		GameTimerLabel->setColor(Color3B::ORANGE);
		this->addChild(GameTimerLabel);
	}

	initTile();

	for (int i = 0; i < TILE_XSIZE; i++)
	{
		for (int j = 0; j < TILE_YSIZE; j++)
		{
			createBlock(i, j);
		}
	}
	
	pauseLayer = Layer::create();
	this->addChild(pauseLayer,5);

	pause = PauseMenu::create(pauseLayer);
	pause->hide();

	if (!bg_sound)
	{
		pause->bg_sound->setTexture("popup/Sound_off.png");
	}

	if (!ef_sound)
	{
		pause->ef_sound->setTexture("popup/Sound_off.png");
	}
	

	this->schedule(schedule_selector(GameScene::update));
	return true;
}


void GameScene::update(float delta)
{
	if (pause->isContinue)
	{
		pause->isContinue = false;
		isPause = false;
		execute = true;
	}

	if (execute){
		if (!GodGameTime){
			GameTimer += delta;
			// 1초에 한번씩
			if (GameTimer >= 1.0f)
			{
				GameTimer -= GameTimer;
				GameTime--;

				int bun = GameTime / 60;
				int cho = GameTime % 60;

				GameTimerLabel->setString(StringUtils::format("%d:%d/%d", bun, cho, Slidenumber));

				// 게임이 종료
				if (GameTime <= 0)
				{
					if (score >= aim_score)
					{
						log("Game Clear");
						this->game_clear();
						execute = false;
					}
					else
					{
						log("Game Fail");
						this->game_over();
						execute = false;
					}
				}
			}
		}

		// 스코어 올리기
		if (score_enable)
		{
			if (cur_score <= score)
			{
				cur_score += 33;
				score_Label->setString(StringUtils::format("%d", cur_score));
			}
			else
			{
				cur_score = score;
				score_Label->setString(StringUtils::format("%d", cur_score));
				score_enable = false;

				if (cur_score >= aim_score)
				{
					this->game_clear();
					execute = false;
				}
			}
		}
	}
}

void GameScene::initTile()
{
	//int white_black = 1;
	//// Y
	//int tile_num = 1;
	//for (int i = 0; i < TILE_YSIZE; i++)
	//{
	//	// X
	//	for (int j = 0; j < TILE_XSIZE; j++)
	//	{
	//		if (tile_num % 2 == 0){
	//			if (white_black >= 0)
	//			{
	//				_tile = Sprite::create("game/tile.png");
	//				_tile->setPosition(Vec2(
	//					D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * j,
	//					D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * i + 20));
	//				_tile->setOpacity(200);
	//				_tile->setScale(1.08f);
	//				this->addChild(_tile);
	//			}
	//			else
	//			{
	//				_tile = Sprite::create("game/tile.png");
	//				_tile->setPosition(Vec2(
	//					D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * j,
	//					D_DESIGN_HEIGHT / 2 - TILE_TSIZE *2.5f + TILE_TSIZE * i + 20));
	//				_tile->setOpacity(200);
	//				_tile->setScale(1.08f);
	//				this->addChild(_tile);
	//			}
	//		}

	//		else
	//		{
	//			if (white_black >= 0)
	//			{
	//				_tile = Sprite::create("game/tile.png");
	//				_tile->setPosition(Vec2(
	//					D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * j,
	//					D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * i + 20));
	//				_tile->setOpacity(200);
	//				_tile->setScale(1.08f);
	//				this->addChild(_tile);
	//			}
	//			else
	//			{
	//				_tile = Sprite::create("game/tile.png");
	//				_tile->setPosition(Vec2(
	//					D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * j,
	//					D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * i + 20));
	//				_tile->setOpacity(200);
	//				_tile->setScale(1.08f);
	//				this->addChild(_tile);
	//			}
	//		}

	//		tile_num++;
	//	}
	//	white_black *= -1;
	//}

	for (int i = 0; i < TILE_YSIZE; i++)
	{
		// X
		for (int j = 0; j < TILE_XSIZE; j++)
		{
			_tile = Sprite::create("game/tile.png");
			_tile->setPosition(Vec2(
				D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * j,
				D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * i + 20));
			//_tile->setOpacity(200);
			_tile->setScale(1.35f);
			this->addChild(_tile);
		}
	}
}


void GameScene::createBlock(int posX, int posY)
{
	if (execute){
		int randNum = (rand() % 6) + 1;

		Blockspr = Block::create();
		Blockspr->setTexture(StringUtils::format("game/%d.png", randNum));
		Blockspr->setPosition(Vec2(
			D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * posX,
			D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * posY + 20));
		Blockspr->setScale(ORIGINAL_SIZE);
		this->addChild(Blockspr);

		v_Block.pushBack(Blockspr); 
		
		v_Block.back()->type = randNum;
		v_Block.back()->nX = posX;
		v_Block.back()->nY = posY;
		v_Block.back()->nDown = 0;
	}
}


bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	if (isPause)
	{
		// 배경 음악 일시정지
		// 클릭시에 사진을 바꿔야 한다. ->setTexture()
		Rect pause_rect = pause->bg_sound->getBoundingBox();
		if (pause_rect.containsPoint(touch->getLocation()))
		{
			if (bg_sound){
				bg_sound = false;
				pause->bg_sound->setTexture("popup/Sound_off.png");
				UserDefault::getInstance()->setBoolForKey("BG_SOUND", false);
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			}
			else
			{
				bg_sound = true;
				pause->bg_sound->setTexture("popup/Sound_on.png");
				UserDefault::getInstance()->setBoolForKey("BG_SOUND", true);
				SimpleAudioEngine::getInstance()->resumeBackgroundMusic();

			}
		}
		Rect pause_rect2 = pause->ef_sound->getBoundingBox();
		// 이팩트 음악 일시정지
		if (pause_rect2.containsPoint(touch->getLocation()))
		{
			if (ef_sound){
				ef_sound = false;
				pause->ef_sound->setTexture("popup/Sound_off.png");
				UserDefault::getInstance()->setBoolForKey("EF_SOUND", false);
				//SimpleAudioEngine::getInstance()->stopEffect();
			}
			else
			{
				ef_sound = true;
				pause->ef_sound->setTexture("popup/Sound_on.png");
				UserDefault::getInstance()->setBoolForKey("EF_SOUND", true);
				//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			}
		}
	}

	if (execute){
	Vec2 Location = touch->getLocation();
	// 보너스를 클릭 ============
	for (auto it : _vBoom)
	{
		if (it->getBoundingBox().containsPoint(Location))
		{
			bonus = true;
			createBonusBlock();

			it->setVisible(false);
			_vBoom.eraseObject(it);
			return true;
		}
	}
	//---------------------------
	for (Block* block : v_Block)
	{
		Rect rect = block->getBoundingBox();
		if (rect.containsPoint(Location))
		{
			switch (block->type)
			{
			case 1:
				block->setTexture("game/1_.png");
				break;
			case 2:
				block->setTexture("game/2_.png");
				break;
			case 3:
				block->setTexture("game/3_.png");
				break;
			case 4:
				block->setTexture("game/4_.png");
				break;
			case 5:
				block->setTexture("game/5_.png");
				break;
			case 6:
				block->setTexture("game/6_.png");
				break;
			}
			block->setScale(SELECTED_SIZE);
			v_allBlock.pushBack(block);
			if (ef_sound)
				SimpleAudioEngine::getInstance()->playEffect("sound/link.wav");
			if (ef_sound)
				SimpleAudioEngine::getInstance()->setEffectsVolume(0.7f);

			if (v_allBlock.back()->type != block->type)
			{
				block->setOpacity(128);
				block->setColor(ccc3(128, 128, 128));
			}
		}
	}

	if (v_allBlock.size() > 0)
	{
		for (Block* block : v_Block)
		{
			if (v_allBlock.back()->type != block->type)
			{
				block->setOpacity(128);
				block->setColor(ccc3(128, 128, 128));
			}
		}
		return true;
	}
	return false;
}
}


void GameScene::onTouchMoved(Touch* touch, Event* event)
{
	if (execute){
		Vec2 location = touch->getLocation();

		if (v_Block.size() > 0)
		{
			for (Block* block : v_Block)
			{
				Rect rect = block->getBoundingBox();

				if (rect.containsPoint(location))
				{
					if (!v_allBlock.contains(block) && checkAroundBlock(v_allBlock.back(), block))
					{
						block->setScale(SELECTED_SIZE);
						blockScore = 100;
						switch (block->type)
						{
						case 1:
							block->setTexture("game/1_.png");
							break;
						case 2:
							block->setTexture("game/2_.png");
							break;
						case 3:
							block->setTexture("game/3_.png");
							break;
						case 4:
							block->setTexture("game/4_.png");
							break;
						case 5:
							block->setTexture("game/5_.png");
							break;
						case 6:
							block->setTexture("game/6_.png");
							if (UserDefault::getInstance()->getIntegerForKey(KEY_GAME_TIME) == 1){
								gameTimeBooster = true;
							}
							blockScore = 0;
							break;
						}
						if (ef_sound)
							SimpleAudioEngine::getInstance()->playEffect("sound/link.wav");
						if (ef_sound)
							SimpleAudioEngine::getInstance()->setEffectsVolume(0.7f);

						v_allBlock.pushBack(block);
					}
					else if (v_allBlock.contains(block) && v_allBlock.size() - 2 == v_allBlock.getIndex(block))
					{
						if (ef_sound)
							SimpleAudioEngine::getInstance()->playEffect("sound/unlink.wav");

						switch (v_allBlock.back()->type)
						{
						case 1:
							v_allBlock.back()->setTexture("game/1.png");
							break;
						case 2:
							v_allBlock.back()->setTexture("game/2.png");
							break;
						case 3:
							v_allBlock.back()->setTexture("game/3.png");
							break;
						case 4:
							v_allBlock.back()->setTexture("game/4.png");
							break;
						case 5:
							v_allBlock.back()->setTexture("game/5.png");
							break;
						case 6:
							v_allBlock.back()->setTexture("game/6.png");
							break;
						}

						v_allBlock.back()->setScale(ORIGINAL_SIZE);
						v_allBlock.eraseObject(v_allBlock.back());
					}
				}
			}
		}
	}
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{

	if (execute){
		int boostTimeScore = 0;

		// 3개이상 블럭을 선택했을때
		if (v_allBlock.size() >= 3)
		{
			if (!bonus){
				// 폭탄을 추가한다.
				if (v_allBlock.size() >= 7)
				{
					Sprite* boom = Sprite::create("game/boom.png");
					if (_vBoom.size())
					{
						boom->setPosition(Vec2(_vBoom.back()->getPosition().x, _vBoom.back()->getPosition().y));
					}
					else
					{
						float temp_pos = D_DESIGN_WIDTH - 100;
						boom->setPosition(Vec2(temp_pos, D_DESIGN_HEIGHT - 100));
					}
					this->addChild(boom);
					_vBoom.pushBack(boom);
				}
			}
			if (!bonus){
				if (!gameTimeBooster)
				{
					// 점수 계산
					score += blockScore * 3;
					if (v_allBlock.size() >= 4)
					{
						for (int i = 4; i <= v_allBlock.size(); i++)
						{
							score += 100 * i;
						}
					}
					score_enable = true;

					// 6번을 선택했을때
					if (gameTimeBooster)
					{
						gameTimeBooster = false;
						boostTimeScore = 1 * v_allBlock.size();

						// 게임 시간에 추가시간을 더함
						GameTime += boostTimeScore;
						int bun = GameTime / 60;
						int cho = GameTime % 60;

						GameTimerLabel->setString(StringUtils::format("%d:%d/%d", bun, cho, Slidenumber));
					}
				}
			}
			// 보너스 점수
			if (bonus)
			{
				bonus = false;
				score += v_allBlock.size() * (blockScore);
				score_enable = true;
			}

			// 사라진곳 위에서 블럭을 생성
			int maxY[TILE_YSIZE] = { 6, 6, 6, 6, 6, 6 };
			for (Block* block : v_allBlock)
			{
				maxY[block->nX] += 1;
				createBlock(block->nX, maxY[block->nX] - 1);
			}

			for (Block* s_Block : v_allBlock)
			{
				for (Block* allBlock : v_Block)
				{
					if ((allBlock->nX == s_Block->nX) && (allBlock->nY > s_Block->nY))
					{
						allBlock->nDown++;
					}
				}

				Animation* ani = Animation::create();
				ani->addSpriteFrameWithFileName("effect/2.png");
				ani->addSpriteFrameWithFileName("effect/3.png");
				ani->addSpriteFrameWithFileName("effect/4.png");
				ani->setDelayPerUnit(0.05f);

				Sprite* effect = Sprite::create("effect/1.png");
				effect->setPosition(s_Block->getPosition());
				effect->runAction(Sequence::create(Animate::create(ani), RemoveSelf::create(), nullptr));
				this->addChild(effect);

				v_Block.eraseObject(s_Block);
				this->removeChild(s_Block);
			}

			int temp_score = score;

			/*float pluse_score = (rand() % 500) + 20;
			score += pluse_score;
			score += blockScore;
			score_enable = true;*/

			log("score : %d", score);

			if (ef_sound)
				SimpleAudioEngine::getInstance()->playEffect("sound/collect.wav");

			for (Block* block : v_Block)
			{
				block->runAction(MoveBy::create(.3f, Vec2(0, (-block->nDown)*TILE_TSIZE)));
				block->nY -= block->nDown;
				block->nDown = 0;
			}

			// 제한 횟수 줄이기
			if (Slidenumber >= 0)
			{
				if (!NoSlide)
				{
					Slidenumber--;
					int bun = GameTime / 60;
					int cho = GameTime % 60;

					GameTimerLabel->setString(StringUtils::format("%d:%d/%d", bun, cho, Slidenumber));
				}
			}
			else
			{
				log("over slide");

				if (score >= aim_score)
				{
					log("Game Clear");
					this->game_clear();
					execute = false;
				}
				else
				{
					game_over();
				}
			}
		}

		v_allBlock.clear();
		for (Block* block : v_Block)
		{
			block->setOpacity(255);
			block->setColor(Color3B::WHITE);

			switch (block->type)
			{
			case 1:
				block->setTexture("game/1.png");
				break;
			case 2:
				block->setTexture("game/2.png");
				break;
			case 3:
				block->setTexture("game/3.png");
				break;
			case 4:
				block->setTexture("game/4.png");
				break;
			case 5:
				block->setTexture("game/5.png");
				break;
			case 6:
				block->setTexture("game/6.png");
				break;
			}

			block->setScale(ORIGINAL_SIZE);
		}

		for (int i = 0; i <= number; i++)
			removeChildByTag(i, true);
		number = 0;
	}
}

bool GameScene::checkAroundBlock(Block* block, Block* dragBlock) // 주변위치
{
	bool isdrag = false;
	int bX = block->nX;
	int bY = block->nY;
	int dX = dragBlock->nX;
	int dY = dragBlock->nY;

	// block -> 기존에 선택했던 블럭	// dragBlock -> 드래그 했을떄 충돌된 블럭
	if (dX <= bX + 1 && dX >= bX - 1 && dY <= bY + 1 && dY >= bY - 1) {
		if (!((dX == bX) && (dY == bY))){
			if (dragBlock->type == block->type)
				isdrag = true;
		}
	}
	return isdrag;
}

//-----------------------
// 보너스 블럭

void GameScene::createBonusBlock()
{
	if (execute){
		for (Block* it : v_Block)
		{
			this->removeChild(it);
			//it->removeFromParentAndCleanup(true);
		}

		v_Block.clear();

		int randNum = (rand() % 5) + 1;
		for (int i = 0; i < TILE_XSIZE; i++)
		{
			for (int j = 0; j < TILE_YSIZE; j++)
			{
				createBonusBlock_func(i, j, randNum);
			}
		}
	}
}
void GameScene::createBonusBlock_func(int posX, int posY, int type)
{
	Blockspr = Block::create();
	Blockspr->setTexture(StringUtils::format("game/%d.png", type));
	Blockspr->setPosition(Vec2(
		D_DESIGN_WIDTH / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * posX,
		D_DESIGN_HEIGHT / 2 - TILE_TSIZE * 2.5f + TILE_TSIZE * posY + 20));
	Blockspr->setScale(ORIGINAL_SIZE);
	this->addChild(Blockspr);

	v_Block.pushBack(Blockspr);
	v_Block.back()->type = type;
	v_Block.back()->nX = posX;
	v_Block.back()->nY = posY;
	v_Block.back()->nDown = 0;
}

void GameScene::game_clear()
{
	result = ResultManager::create();
	result->SetStage(UserDefault::getInstance()->getIntegerForKey(KEY_STAGE_NUM));
	result->GameClear();
	result->SetScore(cur_score);
	result->SetHighScore(aim_score);
	this->addChild(result);
}

void GameScene::game_over()
{
	result = ResultManager::create();
	result->SetStage(UserDefault::getInstance()->getIntegerForKey(KEY_STAGE_NUM));
	result->GameOver();
	result->SetScore(cur_score);
	result->SetHighScore(aim_score);
	this->addChild(result);
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		if (!isPause)
		{
			isPause = true;
			execute = false;
			pause->show();
		}
		else
		{
			isPause = false;
			execute = true;
			pause->hide();
		}
	}
}