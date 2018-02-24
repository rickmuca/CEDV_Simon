// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreController.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"

const FString AScoreController::SCORE_TAG = "Score: ";

// Sets default values
AScoreController::AScoreController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScoreController::BeginPlay()
{
	Super::BeginPlay();

	if (ScoreTextWidget) {
		pScoreTextWidget = CreateWidget<UUserWidget>
			(GetGameInstance(), ScoreTextWidget);

		if (pScoreTextWidget.IsValid()) {
			pScoreTextWidget->AddToViewport();
			pScoreText = (UTextBlock*)pScoreTextWidget
				->GetWidgetFromName("TextScoreWidget");
		}
	}
	
}

// Called every frame
void AScoreController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScoreController::IncrementScoreBy(int32 points) 
{
	FString TextToShow = SCORE_TAG + FString::FromInt(points);
	if (pScoreText.IsValid()) {
		pScoreText->SetText(FText::FromString(TextToShow));
	}
}

