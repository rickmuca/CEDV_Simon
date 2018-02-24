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
	TWeakObjectPtr<AActor> YellowLightButtonRef;
	FString LightButtonCtl = FString(TEXT("LightButtonYellow"));

	TWeakObjectPtr<AActor> BlueLightRef;
	FString BlueLightCtl = FString(TEXT("BlueLight"));
	TWeakObjectPtr<AActor> BlueLightButtonRef;
	FString BlueButtonCtl = FString(TEXT("LightButtonBlue"));

	TWeakObjectPtr<AActor> RedLightRef;
	FString RedLightCtl = FString(TEXT("RedLight"));
	TWeakObjectPtr<AActor> RedLightButtonRef;
	FString RedButtonCtl = FString(TEXT("LightButtonRed"));

	TWeakObjectPtr<AActor> GreenLightRef;
	FString GreenLightCtl = FString(TEXT("Greenlight"));
	TWeakObjectPtr<AActor> GreenLightButtonRef;
	FString GreenButtonCtl = FString(TEXT("LightButtonGreen"));
	
	bool allFound = false;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr && !allFound; ++ActorItr)
	{
		if (YellowLightCtl.Equals(ActorItr->GetName()))
		{
			YellowLightRef = *ActorItr;
		}
		else if (LightButtonCtl.Equals(ActorItr->GetName())) 
		{
			YellowLightButtonRef = *ActorItr;
		}
		else if (BlueLightCtl.Equals(ActorItr->GetName()))
		{
			BlueLightRef = *ActorItr;
		}
		else if (BlueButtonCtl.Equals(ActorItr->GetName()))
		{
			BlueLightButtonRef = *ActorItr;
		}
		else if (RedLightCtl.Equals(ActorItr->GetName()))
		{
			RedLightRef = *ActorItr;
		}
		else if (RedButtonCtl.Equals(ActorItr->GetName()))
		{
			RedLightButtonRef = *ActorItr;
		}
		else if (GreenLightCtl.Equals(ActorItr->GetName()))
		{
			GreenLightRef = *ActorItr;
		}
		else if (GreenButtonCtl.Equals(ActorItr->GetName()))
		{
			GreenLightButtonRef = *ActorItr;
		}

		if (YellowLightRef.IsValid() 
			&& YellowLightButtonRef.IsValid()
			&& YellowLightRef.IsValid()
			&& BlueLightButtonRef.IsValid()
			&& BlueLightRef.IsValid()
			&& RedLightButtonRef.IsValid()
			&& RedLightRef.IsValid()
			&& GreenLightButtonRef.IsValid()
			&& GreenLightRef.IsValid())
		{
			allFound = true;
		}
	}

	LightButtonYellow = AssignPointLightComponentToLightButton(YellowLightRef, YellowLightButtonRef);
	LightButtonBlue = AssignPointLightComponentToLightButton(BlueLightRef, BlueLightButtonRef);
	LightButtonRed = AssignPointLightComponentToLightButton(RedLightRef, RedLightButtonRef);
	LightButtonGreen = AssignPointLightComponentToLightButton(GreenLightRef, GreenLightButtonRef);

	/*if (CheckRefCast(YellowLightRef, APointLight::StaticClass()) &&
		CheckRefCast(YellowLightButtonRef, ALightButton::StaticClass())) 
	{
		APointLight* PointLightComponentPtr =
			Cast<APointLight>(YellowLightRef.Get());

		LightButtonYellow = Cast<ALightButton>(YellowLightButtonRef.Get());
		LightButtonYellow->PointLight = PointLightComponentPtr->PointLightComponent;

		//Lights.Emplace(YELLOW_KEY, LightButtonComponentPtr);
	}*/
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

		if (LightButtonBlue) {
			LightButtonBlue->ToggleLight();
		}

		if (LightButtonRed) {
			LightButtonRed->ToggleLight();
		}

		if (LightButtonGreen) {
			LightButtonGreen->ToggleLight();
		}
		AccumulatedDeltaTime = 0.0f;
	}
}

ALightButton* AGameManager::AssignPointLightComponentToLightButton(TWeakObjectPtr<AActor> LightRef,
	                                        TWeakObjectPtr<AActor> LightButtonRef) {

	if (CheckRefCast(LightRef, APointLight::StaticClass()) &&
		CheckRefCast(LightButtonRef, ALightButton::StaticClass()))
	{
		APointLight* PointLightComponentPtr = Cast<APointLight>(LightRef.Get());

		ALightButton* LightButton = Cast<ALightButton>(LightButtonRef.Get());
		LightButton->PointLight = PointLightComponentPtr->PointLightComponent;
		return LightButton;
	}
	else 
	{
		return NULL;
	}
}

bool AGameManager::CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const 
{
	bool result = ActorRef.IsValid();
	result = result && ActorRef.Get()->IsA(ClassCast);
	return result;
}

