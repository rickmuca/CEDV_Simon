// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "LightButton.h"
#include "Runtime/Engine/Classes/Engine/PointLight.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

const int32 GameStatus::YELLOW_KEY = 0;
const int32 GameStatus::BLUE_KEY = 1;
const int32 GameStatus::RED_KEY = 2;
const int32 GameStatus::GREEN_KEY = 3;

// Sets default values
AGameManager::AGameManager() :
	AccumulatedDeltaTime(0.0f),
	LightToogleDelay(2.0f)
{
	this->CurrentStatus = new GameStatus();
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

	FString ScoreCtl = FString(TEXT("ScoreController_1"));
	TWeakObjectPtr<AActor> ScoreControllerRef;

	FString YellowPlaneCtl = FString(TEXT("YellowPlane"));
	TWeakObjectPtr<AActor> YellowPlaneRef;

	FString BluePlaneCtl = FString(TEXT("BluePlane"));
	TWeakObjectPtr<AActor> BluePlaneRef;

	FString RedPlaneCtl = FString(TEXT("RedPlane"));
	TWeakObjectPtr<AActor> RedPlaneRef;

	FString GreenPlaneCtl = FString(TEXT("GreenPlane"));
	TWeakObjectPtr<AActor> GreenPlaneRef;
	
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
		else if (ScoreCtl.Equals(ActorItr->GetName()))
		{
			ScoreControllerRef = *ActorItr;
		}
		else if (YellowPlaneCtl.Equals(ActorItr->GetName()))
		{
			YellowPlaneRef = *ActorItr;
		}
		else if (BluePlaneCtl.Equals(ActorItr->GetName()))
		{
			BluePlaneRef = *ActorItr;
		}
		else if (RedPlaneCtl.Equals(ActorItr->GetName()))
		{
			RedPlaneRef = *ActorItr;
		}
		else if (GreenPlaneCtl.Equals(ActorItr->GetName()))
		{
			GreenPlaneRef = *ActorItr;
		}

		if (YellowLightRef.IsValid() 
			&& YellowLightButtonRef.IsValid()
			&& YellowLightRef.IsValid()
			&& BlueLightButtonRef.IsValid()
			&& BlueLightRef.IsValid()
			&& RedLightButtonRef.IsValid()
			&& RedLightRef.IsValid()
			&& GreenLightButtonRef.IsValid()
			&& GreenLightRef.IsValid()
			&& ScoreControllerRef.IsValid()
			&& YellowPlaneRef.IsValid()
			&& BluePlaneRef.IsValid()
			&& RedPlaneRef.IsValid()
			&& GreenPlaneRef.IsValid())
		{
			allFound = true;
		}
	}

	LightButtonYellow = AssignPointLightComponentToLightButton(YellowLightRef, YellowLightButtonRef, YellowPlaneRef);
	LightButtonBlue = AssignPointLightComponentToLightButton(BlueLightRef, BlueLightButtonRef, BluePlaneRef);
	LightButtonRed = AssignPointLightComponentToLightButton(RedLightRef, RedLightButtonRef, RedPlaneRef);
	LightButtonGreen = AssignPointLightComponentToLightButton(GreenLightRef, GreenLightButtonRef, GreenPlaneRef);

	if (CheckRefCast(ScoreControllerRef, AScoreController::StaticClass())) {
		AScoreController* ScoreControllerPtr = Cast<AScoreController>(ScoreControllerRef.Get());
		CurrentStatus->SetScoreController(ScoreControllerPtr);
	}

	CurrentStatus->SetUpLevel();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;
	if (AccumulatedDeltaTime >= LightToogleDelay && CurrentStatus->IsPlayingSequence())
	{
		AccumulatedDeltaTime = 0.0f;
		if (LastToggled) {
			// Apagamos la anterior iluminada y esperamos a encender la siguiente
			LastToggled->ToggleLight();
			LastToggled = NULL;
			return;
		}

		if (CurrentStatus->EndOfSequenceReached()) {
			CurrentStatus->SetPlayingSequence(false);
			CurrentStatus->SetWaitingForPlayerMove(true);
		}

		if (CurrentStatus->IsWaitingForPlayerMove()) {
			// Ejemplo de incrementar puntuación
			CurrentStatus->IncrementScoreBy(100);
			CurrentStatus->ResetCurrentSequenceIndex();
		}
		else {
			bool match = false;
			switch (CurrentStatus->GetCurrentItemInSequence()) {
			case GameStatus::YELLOW_KEY:
				if (LightButtonYellow) {
					LastToggled = LightButtonYellow;
					match = true;
				}
				break;
			case GameStatus::BLUE_KEY:
				if (LightButtonBlue) {
					LastToggled = LightButtonBlue;
					match = true;
				}
				break;
			case GameStatus::RED_KEY:
				if (LightButtonRed) {
					LastToggled = LightButtonRed;
					match = true;
				}
				break;
			case GameStatus::GREEN_KEY:
				if (LightButtonGreen) {
					LastToggled = LightButtonGreen;
					match = true;
				}
				break;
			}

			if (match) {
				LastToggled->ToggleLight();
				CurrentStatus->IncrementCurrentSequenceIndex();
			}
		}
	}
}

ALightButton* AGameManager::AssignPointLightComponentToLightButton(TWeakObjectPtr<AActor> LightRef,
	                                        TWeakObjectPtr<AActor> LightButtonRef,
											TWeakObjectPtr<AActor> PlaneRef) {

	if (CheckRefCast(LightRef, APointLight::StaticClass()) &&
		CheckRefCast(LightButtonRef, ALightButton::StaticClass()))
	{
		APointLight* PointLightComponentPtr = Cast<APointLight>(LightRef.Get());

		ALightButton* LightButton = Cast<ALightButton>(LightButtonRef.Get());
		LightButton->PointLight = PointLightComponentPtr->PointLightComponent;

		if (PlaneRef.IsValid()) {
			AActor* Plane = PlaneRef.Get();
			LightButton->SetPLane(Plane);
		}

		LightButton->SetGameStatus(CurrentStatus);

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

