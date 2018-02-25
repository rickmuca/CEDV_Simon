// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ScoreController.h"
#include "CoreMinimal.h"

/**
 * Holds The game status during game play
 */
class CEDV_SIMON_API GameStatus
{
public:
	static const int32 YELLOW_KEY = 0;
	static const int32 BLUE_KEY = 1;
	static const int32 RED_KEY = 2;
	static const int32 GREEN_KEY = 3;

	GameStatus();
	~GameStatus();

	bool IsWaitingForPlayerMove();
	bool IsPlayingSequence();

	void SetWaitingForPlayerMove(bool value);
	void SetPlayingSequence(bool value);

	void SetScoreController(AScoreController* ScoreControllerPtr);

	void LevelUp();
	bool EndOfSequenceReached();
	int32 GetScore();
	void IncrementScoreBy(int32 score);
	int32 GetCurrentItemInSequence();
	void ResetCurrentSequenceIndex();
	void IncrementCurrentSequenceIndex();
	void ShowSuccess();
	void ShowWrong();
	void ShowGo();
	void ShowReady();
	void HideResult();
	bool IsShowingSomeResult();
	void Evaluate(int32 Type);
	void SaveGame();

private:
	bool WaitingForPlayerMove;
	bool PlaySequence;

	int32 Level;

	int32 CurrentSequenceIndex;
	TArray<int32> Sequence;

	int32 CurrentScore;
	AScoreController* ScoreControllerPtr;

	FString PlayerName;

	void SetUpLevel();
};
