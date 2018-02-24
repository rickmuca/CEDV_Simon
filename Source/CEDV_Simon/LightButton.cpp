// Fill out your copyright notice in the Description page of Project Settings.

#include "LightButton.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ALightButton::ALightButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LightIntensity = 3000.0f;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	PointLight->Intensity = LightIntensity;
	PointLight->bVisible = true;
	
	RootComponent = PointLight;

	LightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Light Sphere Component"));
	LightSphere->InitSphereRadius(300.0f);
	LightSphere->SetCollisionProfileName(TEXT("Trigger"));
	LightSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LightSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALightButton::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void ALightButton::ToggleLight()
{
	PointLight->ToggleVisibility();
}

void ALightButton::SetPLane(AActor* LightPlane) 
{
	Plane = LightPlane;
	if (Plane) {
		Plane->OnClicked.AddDynamic(this, &ALightButton::LightClicked);
	}
}

void ALightButton::LightClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	this->ToggleLight();
}

// Called every frame

void ALightButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

