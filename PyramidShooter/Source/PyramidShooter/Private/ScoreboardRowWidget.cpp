// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreboardRowWidget.h"
#include "PyramidShooterGameState.h"
#include "Components/TextBlock.h"

void UScoreboardRowWidget::SetScoreboardRowInformation(const struct FScoreboardPlayerInformation& ScoreboardPlayerInformation)
{
	PlayerName->SetText(FText::FromString(ScoreboardPlayerInformation.PlayerName));
	PlayerScore->SetText(FText::FromString(FString::FromInt(ScoreboardPlayerInformation.Score)));
}
