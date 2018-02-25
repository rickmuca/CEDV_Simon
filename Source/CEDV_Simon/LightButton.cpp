// Fill out your copyright notice in the Description page of Project Settings.

#include "LightButton.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"


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
}

// Called when the game starts or when spawned
void ALightButton::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
}

void ALightButton::ToggleLight()
{
	if (PointLight) {
		PointLight->ToggleVisibility();
	}
}

void ALightButton::SetPLane(AActor* LightPlane) 
{
	Plane = LightPlane;
	if (Plane) {
		Plane->OnClicked.AddDynamic(this, &ALightButton::LightClicked);
	}
}

void ALightButton::SetGameStatus(GameStatus* CurrentStatus) {
	this->CurrentStatus = CurrentStatus;
}

void ALightButton::SetType(int32 Type) {
	this->Type = Type;
}

void ALightButton::LightClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	if (CurrentStatus && CurrentStatus->IsWaitingForPlayerMove()) {
		this->ToggleLight();
		this->TurnedOn = !this->TurnedOn;
		AccumulatedDeltaTime = 0.0f;
		PrimaryActorTick.bCanEverTick = true;

		this->EvaluateClick();
	}
}

void ALightButton::EvaluateClick() 
{
	CurrentStatus->Evaluate(this->Type);
}

// Called every frame
void ALightButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AccumulatedDeltaTime += DeltaTime;

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

