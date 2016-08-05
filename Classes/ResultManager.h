#pragma once

#include "stdafx.h"

enum EVENT_LIST
{
	EVENT_NOTHING,
	EVENT_WIN,
	EVENT_LOSE,
};

class ResultManager : public Layer
{
private:
	Sprite* _popup;

	LabelTTF* _score;
	LabelTTF* _highScore;

	int _stageValue;

private:
	int  NowStage(int stage);
	void OnMenu(int);

public:

public:
	ResultManager();
	~ResultManager();

	bool init() override;

	void SetStage(int stage) { _stageValue = stage; }
	void GameOver();
	void GameClear();

	void SetScore(int score);
	void SetHighScore(int score);

	CREATE_FUNC(ResultManager);
};