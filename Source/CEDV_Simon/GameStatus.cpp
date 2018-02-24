// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStatus.h"

const int32 GameStatus::SEQ_MULTIPLIER = 4;

GameStatus::GameStatus() :
	WaitingForPlayerMove(false),
	PlaySequence(false),
	Level(0),
	CurrentScore(0),
	CurrentSequenceIndex(0)
{
}

GameStatus::~GameStatus()
{
	Sequence.Empty();
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
	if (this->WaitingForPlayerMove) {
		this->ShowGo();
	}
}

void GameStatus::SetPlayingSequence(bool value)
{
	this->PlaySequence = value;
	if (this->PlaySequence) {
		this->ShowReady();
	}
}

void GameStatus::SetScoreController(AScoreController* ScoreControllerPtr) 
{
	this->ScoreControllerPtr = ScoreControllerPtr;
}

void GameStatus::SetUpLevel() 
{
	Sequence.Empty();
	for (int i = 0; i < Level * GameStatus::SEQ_MULTIPLIER; i++) {
		Sequence.Emplace(FMath::RandRange(0, 3));
	}

	CurrentSequenceIndex = 0;
	SetWaitingForPlayerMove(false);
	SetPlayingSequence(true);
}

void GameStatus::LevelUp() 
{
	Level++;
	this->SetUpLevel();
}

bool GameStatus::EndOfSequenceReached() 
{
	return CurrentSequenceIndex > Sequence.Num() - 1;
}

int32 GameStatus::GetScore() {
	return CurrentScore;
}

void GameStatus::IncrementScoreBy(int32 Score) 
{
	CurrentScore = CurrentScore + Score;
	if (ScoreControllerPtr) {
		ScoreControllerPtr->IncrementScoreBy(CurrentScore);
	}
}

int32 GameStatus::GetCurrentItemInSequence() 
{
	return Sequence[CurrentSequenceIndex];
}

void GameStatus::ResetCurrentSequenceIndex()
{
	CurrentSequenceIndex = 0;
}

void GameStatus::IncrementCurrentSequenceIndex() 
{
	CurrentSequenceIndex++;
}

void GameStatus::ShowSuccess()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowSuccess();
	}
}

void GameStatus::ShowWrong() 
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowFail();
	}
}

void GameStatus::ShowGo()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowGo();
	}
}

void GameStatus::ShowReady()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowReady();
	}
}

void GameStatus::HideResult()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->HideResult();
	}
}

bool GameStatus::IsShowingSomeResult() {
	if (ScoreControllerPtr) {
		return ScoreControllerPtr->IsShowingSomeResult();
	}
	else
	{
		return false;
	}
}

void GameStatus::Evaluate(int32 Type) 
{
	int32 Item = this->GetCurrentItemInSequence();

	if (Item == Type)
	{
		this->IncrementScoreBy(100);
		this->IncrementCurrentSequenceIndex();
	}
	else
	{
		this->ShowWrong();
		//Close Level aka Open MainMenu
	}

	if (this->EndOfSequenceReached()) {
		this->ShowSuccess();
		this->ResetCurrentSequenceIndex();
		this->LevelUp();

		this->SetWaitingForPlayerMove(false);
		this->SetPlayingSequence(true);
	}
}
