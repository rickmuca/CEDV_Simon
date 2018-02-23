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

public:	
	// Called every frame
	 virtual void Tick(float DeltaTime) override;

	 void ToggleLight();

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, Category = "Sphere")
		class USphereComponent* LightSphere;

	UPROPERTY(VisibleAnywhere, Category = "LightIntensity")
		float LightIntensity;

};
