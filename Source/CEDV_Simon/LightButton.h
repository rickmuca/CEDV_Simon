// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightButton.generated.h"

UCLASS()
class CEDV_SIMON_API ALightButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ToggleLight();

public:	
	// Called every frame
	 virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Light Button")
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, Category = "Light Button")
		class USphereComponent* LightSphere;

	UPROPERTY(VisibleAnywhere, Category = "Light Button")
		float LightIntensity;


};
