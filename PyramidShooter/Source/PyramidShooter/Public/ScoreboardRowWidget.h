// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDSHOOTER_API UScoreboardRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* PlayerScore;

	void SetScoreboardRowInformation(const struct FScoreboardPlayerInformation& ScoreboardPlayerInformation);
};
