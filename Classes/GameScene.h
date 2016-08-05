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
	
	// ���� ���ھ�
	LabelTTF* score_Label;
	int score;
	int cur_score;
	// ��ǥ ���ھ�
	LabelTTF* aim_score_Label;
	int aim_score;

	bool score_enable;

	// �� ���� ���ھ�
	int blockScore;
	Sprite* spr;

	// Ÿ���� �� ��
	bool nextBlock;
	
	// ���ӻ簣/�����̵� Ƚ��
	LabelTTF* GameTimerLabel;

	// �귯���� �ð�
	float GameTimer;

	// ���� �ð�
	int   GameTime;
	// �ð��� �帣�� ������
	bool  GodGameTime;
	// �߰� ���� �ð�
	bool  gameTimeBooster;

	// ���� Ƚ��
	int Slidenumber;
	// �����̵忡 ������ ������
	bool NoSlide;

	// ��ź ����
	int  boomCnt;
	// ��ź
	Vector<Sprite*> _vBoom;

	// ���ʽ�
	bool bonus;

	// ������
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

	// Ÿ���� �ʱ�ȭ �մϴ�.
	void initTile();
	// ���� �����մϴ�.
	void createBlock(int posX, int posY);
	// �ֺ��� ����� üũ�Ѵ�.
	bool checkAroundBlock(Block* block, Block* dragBlock);


	void createBonusBlock();
	void createBonusBlock_func(int posX, int posY,int type);

	// ��ġ ������
	bool onTouchBegan(Touch* touch, Event* event) override;
	void onTouchEnded(Touch* touch, Event* event) override;
	void onTouchMoved(Touch* touch, Event* event) override;

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void game_clear();
	void game_over();
	CREATE_FUNC(GameScene);
};
