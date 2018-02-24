// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightButton.h"
#include "GameManager.generated.h"

UCLASS()
class CEDV_SIMON_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	static const FString YELLOW_KEY;

private:
	TMap<FString, class ALightButton> Lights;
	ALightButton* AssignPointLightComponentToLightButton(TWeakObjectPtr<AActor> LightRef,
		                                        TWeakObjectPtr<AActor> LightButtonRef);

	bool CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const;

	float AccumulatedDeltaTime;
	float LightToogleDelay;

	ALightButton* LightButtonYellow;
	ALightButton* LightButtonBlue;
	ALightButton* LightButtonRed;
	ALightButton* LightButtonGreen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
