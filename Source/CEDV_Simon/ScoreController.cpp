// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreController.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"

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

	if (ResultTextWidget) {
		pResultTextWidget = CreateWidget<UUserWidget>
			(GetGameInstance(), ResultTextWidget);

		if (pResultTextWidget.IsValid()) {
			pResultTextWidget->AddToViewport();
			pResultText = (UTextBlock*)pResultTextWidget
				->GetWidgetFromName("TextResultWidget");
			this->IncrementScoreBy(0);
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
	FString TextToShow = "";
	if (pScoreText.IsValid()) {
		TextToShow = "Score: " + FString::FromInt(points);
		pScoreText->SetText(FText::FromString(TextToShow));
	}
}

void AScoreController::ShowSuccess()
{
	if (pResultText.IsValid())
	{
		pResultText->SetText(FText::FromString("GREAT!!!"));
		pResultText->SetVisibility(ESlateVisibility::Visible);
	}
}

void AScoreController::ShowFail() 
{
	if (pResultText.IsValid())
	{
		pResultText->SetText(FText::FromString("WRONG!!!"));
		pResultText->SetVisibility(ESlateVisibility::Visible);
	}
}

void AScoreController::ShowGo()
{
	if (pResultText.IsValid())
	{
		pResultText->SetText(FText::FromString("GO!!!"));
		pResultText->SetVisibility(ESlateVisibility::Visible);
	}
}

void AScoreController::ShowReady()
{
	if (pResultText.IsValid())
	{
		pResultText->SetText(FText::FromString("READY..."));
		pResultText->SetVisibility(ESlateVisibility::Visible);
	}
}

void AScoreController::HideResult()
{
	if (pResultText.IsValid())
	{
		pResultText->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool AScoreController::IsShowingSomeResult() {
	if (pResultText.IsValid())
	{
		return pResultText->IsVisible();
	}
	else
	{
		return false;
	}
}

