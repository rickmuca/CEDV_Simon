// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"

#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "LightButton.h"
#include "Runtime/Engine/Classes/Engine/PointLight.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values
AGameManager::AGameManager() :
	AccumulatedDeltaTime(0.0f),
	LightToggleDelay(2.0f),
	ShowResultDelay(1.5f),
	AccumulatedDeltaTimeForResult(0.0f),
	Started(false)
{
	// Set up a new game status
	this->CurrentStatus = new GameStatus();

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	// Get all ref to actors
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

	// Configure Lights, it needs a PointLight to be shown, a place to listen to click events
	// and a tone code for playing a specific sound
	LightButtonYellow = SetUpLightButton(YellowLightRef, YellowLightButtonRef, YellowPlaneRef, GameStatus::YELLOW_KEY);
	LightButtonBlue = SetUpLightButton(BlueLightRef, BlueLightButtonRef, BluePlaneRef, GameStatus::BLUE_KEY);
	LightButtonRed = SetUpLightButton(RedLightRef, RedLightButtonRef, RedPlaneRef, GameStatus::RED_KEY);
	LightButtonGreen = SetUpLightButton(GreenLightRef, GreenLightButtonRef, GreenPlaneRef, GameStatus::GREEN_KEY);

	// Get ScoreController and messages UI and pass reference to GameStatus
	if (CheckRefCast(ScoreControllerRef, AScoreController::StaticClass())) {
		AScoreController* ScoreControllerPtr = Cast<AScoreController>(ScoreControllerRef.Get());
		CurrentStatus->SetScoreController(ScoreControllerPtr);
	}

	// Configure new level
	CurrentStatus->LevelUp();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;

	// Are we showing some message?
	if (CurrentStatus->IsShowingSomeResult()) {
		AccumulatedDeltaTimeForResult += DeltaTime;
		if (AccumulatedDeltaTime >= ShowResultDelay) // Message show delay
		{
			AccumulatedDeltaTimeForResult = 0.0f;
			AccumulatedDeltaTime = 0.0f;

			// Hide message on timeout
			CurrentStatus->HideResult();
			if (CurrentStatus->GameOver()) // Player misclick?
			{
				this->EndSimon();
			}
		}
		return;
	}

	// Light toggle on/off delay
	if (AccumulatedDeltaTime >= LightToggleDelay)
	{
		AccumulatedDeltaTime = 0.0f;
		
		// Algorithm dont manipulate lights if it is in player turn or showing some message
		if (CurrentStatus->IsPlayingSequence() && !CurrentStatus->IsShowingSomeResult())
		{
			if (!Started) { // Sequence tone not yet started, so we show ready and wait to start
				CurrentStatus->ShowReady();
				Started = true;
				return;
			}

			// Tone sequence already played?
			if (CurrentStatus->EndOfSequenceReached()) {
				CurrentStatus->ResetCurrentSequenceIndex();
				CurrentStatus->SetPlayingSequence(false);
				CurrentStatus->SetWaitingForPlayerMove(true);
				CurrentStatus->ShowGo();
			}

			// Are we playing sequence?
			if (CurrentStatus->IsPlayingSequence()) {
				
				bool match = true;
				// Get next light to turn on and play tone
				switch (CurrentStatus->GetCurrentItemInSequence()) {
				case GameStatus::YELLOW_KEY:
					if (LightButtonYellow) {
						LastToggled = LightButtonYellow;
					}
					break;
				case GameStatus::BLUE_KEY:
					if (LightButtonBlue) {
						LastToggled = LightButtonBlue;
					}
					break;
				case GameStatus::RED_KEY:
					if (LightButtonRed) {
						LastToggled = LightButtonRed;
					}
					break;
				case GameStatus::GREEN_KEY:
					if (LightButtonGreen) {
						LastToggled = LightButtonGreen;
					}
					break;
				default:
					match = false;
				}

				if (match) { // found
				    // Play tone
					LastToggled->ToggleLight();
					// Set up next
					CurrentStatus->IncrementCurrentSequenceIndex();
				}
			}
			else 
			{
				// If we just finish to play tone sequence, set started to false
				// see line 199 (Tone sequence already played?)
				if (!CurrentStatus->IsPlayingSequence())
				{
					Started = false;
				}
			}
		}
	}
}

// Get the LightButton attributes and set the up
ALightButton* AGameManager::SetUpLightButton(
	TWeakObjectPtr<AActor> LightRef,
	TWeakObjectPtr<AActor> LightButtonRef,
	TWeakObjectPtr<AActor> PlaneRef,
	int32 Type) {

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
		LightButton->SetType(Type);

		return LightButton;
	}
	else 
	{
		return NULL;
	}
}

// Check cast according to AActor ref and AActor specific sub class
bool AGameManager::CheckRefCast(TWeakObjectPtr<AActor> ActorRef, const UClass *ClassCast) const 
{
	bool result = ActorRef.IsValid();
	result = result && ActorRef.Get()->IsA(ClassCast);
	return result;
}

// Open main menu on Game Over
void AGameManager::EndSimon()
{
	UWorld* TheWorld = GetWorld();

	FString CurrentLevel = TheWorld->GetMapName();

	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}

