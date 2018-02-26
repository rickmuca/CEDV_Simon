// Fill out your copyright notice in the Description page of Project Settings.

#include "LightButton.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameStatus.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
ALightButton::ALightButton() :
	AccumulatedDeltaTime(0.0f),
	LightTurnedOnDelay(1.0f),
	TurnedOn(false)
{
 	LightIntensity = 6000.0f;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->Intensity = LightIntensity;
	PointLight->bVisible = true;
	
	RootComponent = PointLight;

	LightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Light Sphere Component"));
	LightSphere->InitSphereRadius(600.0f);
	LightSphere->SetCollisionProfileName(TEXT("Trigger"));
	LightSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LightSphere->SetupAttachment(RootComponent);

	// Create an audio component, the audio component wraps the Cue, and allows us to ineract with
	// it, and its parameters from code.
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	auto YellowSoundAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounds/YellowCue.YellowCue'"));
	if (YellowSoundAsset.Succeeded()) {
		YellowCue = YellowSoundAsset.Object;
	}

	auto BlueSoundAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounds/BlueCue.BlueCue'"));
	if (BlueSoundAsset.Succeeded()) {
		BlueCue = BlueSoundAsset.Object;
	}

	auto RedSoundAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounds/RedCue.RedCue'"));
	if (RedSoundAsset.Succeeded()) {
		RedCue = RedSoundAsset.Object;
	}

	auto GreenSoundAsset = ConstructorHelpers::FObjectFinder<USoundCue>(TEXT("SoundCue'/Game/Sounds/GreenCue.GreenCue'"));
	if (GreenSoundAsset.Succeeded()) {
		GreenCue = GreenSoundAsset.Object;
	}
}

// Called when the game starts or when spawned
void ALightButton::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
}

void ALightButton::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALightButton::ToggleLight()
{
	if (PointLight) {
		PointLight->ToggleVisibility();
		if (!TurnedOn) {
			// Play CUE if were already turned off because if about to turn it on
			this->PlaySound();
		}
		this->TurnedOn = !this->TurnedOn;

		// reset delay
		AccumulatedDeltaTime = 0.0f;

		// Call on tick again
		PrimaryActorTick.bCanEverTick = true;
	}
}

// Set plane to handle click events
void ALightButton::SetPLane(AActor* LightPlane) 
{
	Plane = LightPlane;
	if (Plane) {
		// Configure click listener
		Plane->OnClicked.AddDynamic(this, &ALightButton::LightClicked);
	}
}

// Set current game status instance
void ALightButton::SetGameStatus(GameStatus* CurrentStatus) {
	this->CurrentStatus = CurrentStatus;
}

// Set light tone type
void ALightButton::SetType(int32 Type) {
	this->Type = Type;

	this->SetUpAudioComponent(this->Type);
}

// Configuring sound...
void ALightButton::SetUpAudioComponent(int32 LightButtonType) {
	// Store a reference to the Cue asset - we'll need it later.
	switch (LightButtonType) {
	case GameStatus::YELLOW_KEY:
		AudioCue = YellowCue.Get();
		break;
	case GameStatus::BLUE_KEY:
		AudioCue = BlueCue.Get();
		break;
	case GameStatus::RED_KEY:
		AudioCue = RedCue.Get();
		break;
	case GameStatus::GREEN_KEY:
		AudioCue = GreenCue.Get();
		break;
	}

	if (AudioCue && AudioCue->IsValidLowLevelFast()) {
		AudioComponent->SetSound(AudioCue);

		// I don't want the sound playing the moment it's created.
		AudioComponent->bAutoActivate = false; // don't play the sound immediately.

		// I want the sound to follow the pawn around, so I attach it to the Pawns root.
		//AudioComponent->SetupAttachment(GetRootComponent());

		// I want the sound to come from slighty in front of the pawn.
		AudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	}
}

// On light clicked during player turn
void ALightButton::LightClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	// Is player turn?
	if (CurrentStatus && CurrentStatus->IsWaitingForPlayerMove()) {
		this->ToggleLight();
		// Evaluate click in game tone sequence
		this->EvaluateClick();
	}
}

void ALightButton::PlaySound()
{
	if (AudioComponent)
	{
		// You can fade the sound in...
		// ->FadeIn(fadeTime, volume, startTime);
		AudioComponent->Play();
	}
}

// Evaluate click in current game status
void ALightButton::EvaluateClick() 
{
	CurrentStatus->Evaluate(this->Type);
}

// Called every frame
void ALightButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;

	// If toggle delay expires, toggle it again...
	if (AccumulatedDeltaTime >= LightTurnedOnDelay)
	{
		AccumulatedDeltaTime = 0.0f;
		PrimaryActorTick.bCanEverTick = false;
		if (this->TurnedOn) {
			this->ToggleLight();
			this->TurnedOn = false;
		}
	}
}

