#pragma once
#include "stdafx.h"

class Block : public Sprite
{
private:
public:
	int type;
	int nX, nY;
	int nDown;
	CREATE_FUNC(Block);
};

class PauseMenu;
class ResultManager;
class GameScene : public Layer
{
private:
	int number;

	Sprite* _background;
	Sprite* _tile;
	Sprite* m_pBlock;
	Block* Blockspr;

	Vector<Block*> v_Block;
	Vector<Block*> v_allBlock;
	Vector<Sprite*>v_Tile;
	
	// 현재 스코어
	LabelTTF* score_Label;
	int score;
	int cur_score;
	// 목표 스코어
	LabelTTF* aim_score_Label;
	int aim_score;

	bool score_enable;

	// 그 블럭의 스코어
	int blockScore;
	Sprite* spr;

	// 타일을 깔때 씀
	bool nextBlock;
	
	// 게임사간/슬라이드 횟수
	LabelTTF* GameTimerLabel;

	// 흘러가는 시간
	float GameTimer;

	// 남은 시간
	int   GameTime;
	// 시간이 흐르지 않을때
	bool  GodGameTime;
	// 추가 게임 시간
	bool  gameTimeBooster;

	// 제한 횟수
	int Slidenumber;
	// 슬라이드에 제한이 없을때
	bool NoSlide;

	// 폭탄 개수
	int  boomCnt;
	// 폭탄
	Vector<Sprite*> _vBoom;

	// 보너스
	bool bonus;

	// 실행중
	bool execute;

	Layer* pauseLayer;
	ResultManager* result;
	PauseMenu* pause;

	bool	isPause;



	bool bg_sound;
	bool ef_sound;
public:
	 GameScene();
	~GameScene();

public:
	static Scene* createScene();
	bool init()				 override;
	void update(float delta) override;

	// 타일을 초기화 합니다.
	void initTile();
	// 블럭을 생성합니다.
	void createBlock(int posX, int posY);
	// 주변의 블록을 체크한다.
	bool checkAroundBlock(Block* block, Block* dragBlock);


	void createBonusBlock();
	void createBonusBlock_func(int posX, int posY,int type);

	// 터치 리스너
	bool onTouchBegan(Touch* touch, Event* event) override;
	void onTouchEnded(Touch* touch, Event* event) override;
	void onTouchMoved(Touch* touch, Event* event) override;

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void game_clear();
	void game_over();
	CREATE_FUNC(GameScene);
};
