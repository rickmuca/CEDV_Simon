// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "LightButton.h"
#include "Runtime/Engine/Classes/Engine/PointLight.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

const int32 AGameManager::YELLOW_KEY = 0;
const int32 AGameManager::BLUE_KEY = 1;
const int32 AGameManager::RED_KEY = 2;
const int32 AGameManager::GREEN_KEY = 3;

// Sets default values
AGameManager::AGameManager() :
	AccumulatedDeltaTime(0.0f),
	LightToogleDelay(1.5f),
	Level(1),
	PlaySequence(false),
	WaitingForPlayerMove(false)
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

	this->SetUpLevel();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;
	if (AccumulatedDeltaTime >= LightToogleDelay && PlaySequence)
	{
		AccumulatedDeltaTime = 0.0f;
		if (LastToggled) {
			LastToggled->ToggleLight();
			LastToggled = NULL;
		}

		if (CurrentSequenceIndex > Sequence.Num() - 1) {
			PlaySequence = false;
			WaitingForPlayerMove = true;
			return;
		}

		switch (Sequence[CurrentSequenceIndex]) {
		case AGameManager::YELLOW_KEY:
			if (LightButtonYellow) {
				LightButtonYellow->ToggleLight();
				CurrentSequenceIndex++;
				LastToggled = LightButtonYellow;
			}
			break;
		case AGameManager::BLUE_KEY:
			if (LightButtonBlue) {
				LightButtonBlue->ToggleLight();
				CurrentSequenceIndex++;
				LastToggled = LightButtonBlue;
			}
			break;
		case AGameManager::RED_KEY:
			if (LightButtonRed) {
				LightButtonRed->ToggleLight();
				CurrentSequenceIndex++;
				LastToggled = LightButtonRed;
			}
			break;
		case AGameManager::GREEN_KEY:
			if (LightButtonGreen) {
				LightButtonGreen->ToggleLight();
				CurrentSequenceIndex++;
				LastToggled = LightButtonGreen;
			}
			break;
		}
	}
}

void AGameManager::SetUpLevel() {
	Sequence.Empty();
	for (int i = 0; i < Level * 8; i++) {
		Sequence.Emplace(FMath::RandRange(0, 3));
	}

	CurrentSequenceIndex = 0;
	PlaySequence = true;
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

