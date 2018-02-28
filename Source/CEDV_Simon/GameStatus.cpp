// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStatus.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

GameStatus::GameStatus() :
	WaitingForPlayerMove(false),
	PlaySequence(false),
	Level(0),
	CurrentScore(0),
	CurrentSequenceIndex(0),
	Finish(false)
{
	Sequence.Empty();
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
}

void GameStatus::SetPlayingSequence(bool value)
{
	this->PlaySequence = value;
}

void GameStatus::SetScoreController(AScoreController* ScoreControllerPtr) 
{
	this->ScoreControllerPtr = ScoreControllerPtr;
}

// Sets a new level, initially we have two tones, the we increase by one
// for each level
void GameStatus::SetUpLevel() 
{
	/*Sequence.Empty();
	for (int i = 0; i < Level + 1; i++) {
		Sequence.Emplace(FMath::RandRange(0, 3));
	}*/

	if (Sequence.Num() == 0) {
		Sequence.Emplace(FMath::RandRange(0, 3));
		Sequence.Emplace(FMath::RandRange(0, 3));
	}
	else {
		Sequence.Emplace(FMath::RandRange(0, 3));
	}

	CurrentSequenceIndex = 0;
	SetWaitingForPlayerMove(false);
	SetPlayingSequence(true);
}

// Go level up
void GameStatus::LevelUp() 
{
	Level++;
	this->SetUpLevel();
}

// All tones played by algorithm or player?
bool GameStatus::EndOfSequenceReached() 
{
	return CurrentSequenceIndex > Sequence.Num() - 1;
}

// Current Score
int32 GameStatus::GetScore() {
	return CurrentScore;
}

// Acum score
void GameStatus::IncrementScoreBy(int32 Score) 
{
	CurrentScore = CurrentScore + Score;
	if (ScoreControllerPtr) {
		ScoreControllerPtr->IncrementScoreBy(CurrentScore);
	}
}

// Current tone to be played/to play whitin sequence
int32 GameStatus::GetCurrentItemInSequence() 
{
	return Sequence[CurrentSequenceIndex];
}

// Reset tone sequence Index to start
// We should restart this index on every new level and player turn
void GameStatus::ResetCurrentSequenceIndex()
{
	CurrentSequenceIndex = 0;
}

// Increment tone sequence index to play
// If Player is right we also increment it 
void GameStatus::IncrementCurrentSequenceIndex() 
{
	CurrentSequenceIndex++;
}

// Show success message on screen
void GameStatus::ShowSuccess()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowSuccess();
	}
}

// Show wrong message on screen
void GameStatus::ShowWrong() 
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowFail();
	}
}

// Show go!! message on screen
void GameStatus::ShowGo()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowGo();
	}
}

// Show ready message on screen
void GameStatus::ShowReady()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->ShowReady();
	}
}

// Hide last message on screen
void GameStatus::HideResult()
{
	if (ScoreControllerPtr) {
		ScoreControllerPtr->HideResult();
	}
}

// Is currently showing some message during gameplay?
bool GameStatus::IsShowingSomeResult() {
	if (ScoreControllerPtr) {
		return ScoreControllerPtr->IsShowingSomeResult();
	}
	else
	{
		return false;
	}
}

// Evaluate a click done by player
// Type -> Tone code played by player
void GameStatus::Evaluate(int32 Type) 
{
	int32 Item = this->GetCurrentItemInSequence();

	if (Item == Type) // Correct, increment score and tone sequence index
	{
		this->IncrementScoreBy(100);
		this->IncrementCurrentSequenceIndex();
	}
	else // Wrong, save game and finish
	{
		this->ShowWrong();
		this->SaveGame();
		this->Finish = true;
	}

	if (this->EndOfSequenceReached()) { // Sequence played correctly, level up!
		this->ShowSuccess();
		this->ResetCurrentSequenceIndex();
		this->LevelUp();

		this->SetWaitingForPlayerMove(false);
		this->SetPlayingSequence(true);
	}
}

// Save current score and name
void GameStatus::SaveGame()
{	
	FDateTime currentDateTime = FDateTime::Now();

	// Player name will be the current date in this version
	UMySaveGame::SaveMaxScore(currentDateTime.ToString(), CurrentScore);
	UMySaveGame::PrintRanking();
}

// Set game about to be finished
bool GameStatus::GameOver()
{
	return this->Finish;
}