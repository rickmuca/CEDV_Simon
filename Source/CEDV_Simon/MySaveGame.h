// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * Holds max score ranking
 */
UCLASS()
class CEDV_SIMON_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
		TMap<FString, int32> Ranking;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UMySaveGame();

	UFUNCTION(BlueprintCallable, Category = "Ranking")
		TMap<FString, int32> GetRanking();
	UFUNCTION(BlueprintCallable, Category = "RankingRow")
		void SavePlayer(FString PlayerName, int Score);

	static void SaveMaxScore(FString PlayerName, int Score);

	static void PrintRanking();
};
