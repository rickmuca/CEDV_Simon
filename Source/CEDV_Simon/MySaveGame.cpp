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
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(
		UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(
		LoadGameInstance->SaveSlotName,
		LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		TMap<FString, int32> RankingMap = LoadGameInstance->Ranking;

		RankingMap.Add(PlayerName, Score);
		LoadGameInstance->Ranking = RankingMap;
		UGameplayStatics::SaveGameToSlot(LoadGameInstance,
			LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex);

	}
	else
	{
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

		SaveGameInstance->Ranking.Add(PlayerName, Score);

		UGameplayStatics::SaveGameToSlot(SaveGameInstance,
			SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	}
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
			return A > B; // sort strings by length
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

FString UMySaveGame::GetRankingString()
{
	TMap<FString, int32> RankingMap = GetRanking();

	FString TextToShow = "";
	int index = 1;
	for (auto& Elem : RankingMap)
	{
		TextToShow.Append(
			*FString::Printf(
				TEXT("%d. %s\t\t%d\n"),
				index,
				*Elem.Key,
				Elem.Value
			)
		);
		index++;
		// Only return the first 6 elements
		if (index > 6)
			break;
	}
	return TextToShow;
}