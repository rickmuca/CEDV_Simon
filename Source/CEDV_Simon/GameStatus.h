// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ScoreController.h"
#include "CoreMinimal.h"

/**
 * Holds The game status during game play and controls the game logic
 */
class CEDV_SIMON_API GameStatus
{
public:
	// Key codes for every light
	static const int32 YELLOW_KEY = 0;
	static const int32 BLUE_KEY = 1;
	static const int32 RED_KEY = 2;
	static const int32 GREEN_KEY = 3;

	GameStatus();
	~GameStatus();

	// It controls who is owning the game a this moment
	bool IsWaitingForPlayerMove();
	bool IsPlayingSequence();
	void SetWaitingForPlayerMove(bool value);
	void SetPlayingSequence(bool value);

	// Score controller for showing on screen messages
	void SetScoreController(AScoreController* ScoreControllerPtr);

	// handles sequence and game logic
	void LevelUp();
	bool EndOfSequenceReached();
	int32 GetScore();
	void IncrementScoreBy(int32 score);
	int32 GetCurrentItemInSequence();
	void ResetCurrentSequenceIndex();
	void IncrementCurrentSequenceIndex();
	
	// On screen message handling
	void ShowSuccess();
	void ShowWrong();
	void ShowGo();
	void ShowReady();
	void HideResult();
	bool IsShowingSomeResult();
	
	// Evaluate last player click
	void Evaluate(int32 Type);

	// Save game on game over
	void SaveGame();

	// Turn finish to true
	bool GameOver();

private:
	// Is player turn
	bool WaitingForPlayerMove;

	// Is sequence being played
	bool PlaySequence;

	// Current level
	int32 Level;

	// Current sequence Index played / to play
	int32 CurrentSequenceIndex;

	// Sequence played / to play
	TArray<int32> Sequence;

	// Player Score
	int32 CurrentScore;

	// Score controllerfor controlling on UX Score and messages
	AScoreController* ScoreControllerPtr;

	FString PlayerName;

	// Game Over? When pplayer is wrong
	bool Finish;

	// Set up a new level
	void SetUpLevel();
};
