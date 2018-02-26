// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreController.generated.h"

/* handles text widgets added to show on screen messages */

UCLASS()
class CEDV_SIMON_API AScoreController : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AScoreController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Score text widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScoreController")
	TSubclassOf<class UUserWidget> ScoreTextWidget;
	TWeakObjectPtr<class UUserWidget> pScoreTextWidget;
	TWeakObjectPtr<class UTextBlock> pScoreText;

	// On screen message widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ResultController")
	TSubclassOf<class UUserWidget> ResultTextWidget;
	TWeakObjectPtr<class UUserWidget> pResultTextWidget;
	TWeakObjectPtr<class UTextBlock> pResultText;

	// Update score
	void IncrementScoreBy(int32 points);

	// Showing on screen messages
	void ShowSuccess();
	void ShowFail();
	void ShowGo();
	void ShowReady();

	// Hide last on screen message
	void HideResult();

	// Is showing some on screen message? 
	bool IsShowingSomeResult();
};
