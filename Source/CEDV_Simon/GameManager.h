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

private:
	// Holds game logic and status
	GameStatus* CurrentStatus;

	// Delay to play tone within sequence
	float AccumulatedDeltaTime;
	float LightToggleDelay;

	// Show screen message delay
	float ShowResultDelay;
	float AccumulatedDeltaTimeForResult;

	bool Started;

	// Pointer to each light button
	ALightButton* LightButtonYellow;
	ALightButton* LightButtonBlue;
	ALightButton* LightButtonRed;
	ALightButton* LightButtonGreen;
	ALightButton* LastToggled;

private:
	ALightButton* SetUpLightButton(
		TWeakObjectPtr<AActor> LightRef,
		TWeakObjectPtr<AActor> LightButtonRef,
		TWeakObjectPtr<AActor> PlaneRef,
		int32 type);

	bool CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const;
	void EndSimon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
