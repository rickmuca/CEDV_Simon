// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreController.generated.h"

UCLASS()
class CEDV_SIMON_API AScoreController : public AActor
{
	GENERATED_BODY()
	
private:
	static const FString SCORE_TAG;
public:	
	// Sets default values for this actor's properties
	AScoreController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ScoreController")
	TSubclassOf<class UUserWidget> ScoreTextWidget;
	TWeakObjectPtr<class UUserWidget> pScoreTextWidget;
	TWeakObjectPtr<class UTextBlock> pScoreText;

	void IncrementScoreBy(int32 points);
};
