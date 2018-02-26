// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStatus.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LightButton.generated.h"

UCLASS()
class CEDV_SIMON_API ALightButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightButton();

	// Light properties and light component to toggle
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, Category = "Sphere")
		class USphereComponent* LightSphere;

	UPROPERTY(VisibleAnywhere, Category = "LightIntensity")
		float LightIntensity;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue* AudioCue;

	// Audio component associated to light (tone)
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		UAudioComponent* AudioComponent;

private:
	// Light toogle delay
	float AccumulatedDeltaTime;
	float LightTurnedOnDelay;

	// PLane for handling on click events
	AActor* Plane;

	// GameStatus to evaluate every click
	GameStatus* CurrentStatus;

	// Type code associated with this light
	int32 Type;

	// Is now turned on?
	bool TurnedOn;

	// Evaluate current click
	void EvaluateClick();

	// Sound handling, configure
	void SetUpAudioComponent(int32 LightButtonType);

	// Play associated sound
	void PlaySound();

protected:
	UPROPERTY()
		TWeakObjectPtr<USoundCue> YellowCue;
	UPROPERTY()
		TWeakObjectPtr<USoundCue> BlueCue;
	UPROPERTY()
		TWeakObjectPtr<USoundCue> RedCue;
	UPROPERTY()
		TWeakObjectPtr<USoundCue> GreenCue;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	 virtual void Tick(float DeltaTime) override;

	 void SetPLane(AActor* Plane);
	 void SetGameStatus(GameStatus* CurrentStatus);
	 void SetType(int32 Type);

	 void ToggleLight();
	 
	 UFUNCTION()
	 void LightClicked(AActor* TouchedActor, FKey ButtonPressed);
};
