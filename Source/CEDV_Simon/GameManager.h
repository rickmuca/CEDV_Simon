// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightButton.h"
#include "ScoreController.h"
#include "GameStatus.h"
#include "GameManager.generated.h"

UCLASS()
class CEDV_SIMON_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	static const int32 YELLOW_KEY;
	static const int32 BLUE_KEY;
	static const int32 RED_KEY;
	static const int32 GREEN_KEY;
	static const int32 SEQ_MULTIPLIER;

private:
	GameStatus* CurrentStatus;
	float AccumulatedDeltaTime;
	float LightToogleDelay;

	ALightButton* LightButtonYellow;
	ALightButton* LightButtonBlue;
	ALightButton* LightButtonRed;
	ALightButton* LightButtonGreen;
	ALightButton* LastToggled;

	AScoreController* ScoreControllerPtr;

	int32 Level;

	int32 CurrentSequenceIndex;
	TArray<int32> Sequence;
	int32 CurrentScore;

	TMap<FString, class ALightButton> Lights;

private:
	void SetUpLevel();
	void LevelUp();

	ALightButton* AssignPointLightComponentToLightButton(TWeakObjectPtr<AActor> LightRef,
		                                        TWeakObjectPtr<AActor> LightButtonRef,
												TWeakObjectPtr<AActor> PlaneRef);

	bool CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
