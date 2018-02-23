// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "LightButton.h"
#include "Runtime/Engine/Classes/Engine/PointLight.h"

const FString AGameManager::YELLOW_KEY = "Yellow";

// Sets default values
AGameManager::AGameManager() :
	AccumulatedDeltaTime(0.0f),
	LightToogleDelay(1.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	TWeakObjectPtr<AActor> YellowLightRef;
	FString YellowLightCtl = FString(TEXT("YellowLight"));

	TWeakObjectPtr<AActor> LightButtonYellowRef;
	FString LightButtonYellowCtl = FString(TEXT("LightButtonYellow"));
	
	bool allFound = false;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (YellowLightCtl.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer 
			YellowLightRef = *ActorItr;
		}
		else if (LightButtonYellowCtl.Equals(ActorItr->GetName())) 
		{
			LightButtonYellowRef = *ActorItr;
		}

		if (YellowLightRef.IsValid() && LightButtonYellowRef.IsValid())
		{
			allFound = true;
		}
	}

	if (CheckRefCast(YellowLightRef, APointLight::StaticClass()) &&
		CheckRefCast(LightButtonYellowRef, ALightButton::StaticClass())) 
	{
		APointLight* PointLightComponentPtr =
			Cast<APointLight>(YellowLightRef.Get());

		LightButtonYellow = Cast<ALightButton>(LightButtonYellowRef.Get());
		LightButtonYellow->PointLight = PointLightComponentPtr->PointLightComponent;

		//Lights.Emplace(YELLOW_KEY, LightButtonComponentPtr);
	}
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;
	if (AccumulatedDeltaTime >= LightToogleDelay)
	{
		if (LightButtonYellow) {
			LightButtonYellow->ToggleLight();
		}
		AccumulatedDeltaTime = 0.0f;
	}
}

bool AGameManager::CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const 
{
	bool result = ActorRef.IsValid();
	result = result && ActorRef.Get()->IsA(ClassCast);
	return result;
}

