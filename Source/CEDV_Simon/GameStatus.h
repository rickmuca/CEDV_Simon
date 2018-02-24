// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CEDV_SIMON_API GameStatus
{
public:
	GameStatus();
	~GameStatus();

	bool IsWaitingForPlayerMove();
	bool IsPlayingSequence();

	void SetWaitingForPlayerMove(bool value);
	void SetPlayingSequence(bool value);

private:
	bool WaitingForPlayerMove;
	bool PlaySequence;
};
