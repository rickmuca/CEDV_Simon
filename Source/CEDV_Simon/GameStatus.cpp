// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStatus.h"

GameStatus::GameStatus() :
	WaitingForPlayerMove(false),
	PlaySequence(true)
{
}


GameStatus::~GameStatus()
{
}

bool GameStatus::IsWaitingForPlayerMove()
{
	return this->WaitingForPlayerMove;
}

bool GameStatus::IsPlayingSequence()
{
	return this->PlaySequence;
}

void GameStatus::SetWaitingForPlayerMove(bool value)
{
	this->WaitingForPlayerMove = value;
}

void GameStatus::SetPlayingSequence(bool value)
{
	this->PlaySequence = value;
}
