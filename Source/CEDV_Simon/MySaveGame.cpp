// Fill out your copyright notice in the Description page of Project Settings.

#include "MySaveGame.h"
#include "CEDV_Simon.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void UMySaveGame::SaveMaxScore(FString PlayerName, int Score)
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(
		UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->Ranking.Add(PlayerName, Score);

	UGameplayStatics::SaveGameToSlot(SaveGameInstance,
		SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void UMySaveGame::PrintRanking()
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(
		UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(
		LoadGameInstance->SaveSlotName,
		LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		TMap<FString, int32> RankingMap = LoadGameInstance->Ranking;

		FString TextToShow = "";

		for (auto& Elem : RankingMap)
		{
			TextToShow.Append(
				*FString::Printf(
					TEXT("(%s, \"%d\")\n"),
					*Elem.Key,
					Elem.Value
				)
			);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TextToShow);
		}
	}
}

TMap<FString, int32> UMySaveGame::GetRanking()
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(
		UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(
		LoadGameInstance->SaveSlotName,
		LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		TMap<FString, int32> RankingMap = LoadGameInstance->Ranking;

		RankingMap.ValueSort([](const int32 A, const int32 B) {
			return A < B; // sort strings by length
		});

		return RankingMap;
	}
	else {
		return Ranking;
	}
}

void UMySaveGame::SavePlayer(FString PlayerName, int Score)
{
	UMySaveGame::SaveMaxScore(PlayerName, Score);
}