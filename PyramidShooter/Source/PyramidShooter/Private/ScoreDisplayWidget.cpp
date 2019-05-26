#include "ScoreDisplayWidget.h"
#include "PyramidShooterPlayerState.h"
#include "Components/TextBlock.h"
#include "PyramidShooterGameState.h"
#include "ScoreboardRowWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"

UScoreDisplayWidget::UScoreDisplayWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UScoreboardRowWidget> WidgetAsset(TEXT("/Game/UI/BP_ScoreboardRowWidget"));
	if (WidgetAsset.Succeeded())
	{
		ScoreboardRowWidgetClass = WidgetAsset.Class;
	}
}

void UScoreDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* GameWorld = GetWorld();

	APyramidShooterPlayerState* PyramidShooterPlayerState = Cast<APyramidShooterPlayerState>(GameWorld->GetFirstPlayerController()->PlayerState);

	if (PyramidShooterPlayerState != nullptr)
	{
		PyramidShooterPlayerState->OnPlayerScoreModified().AddUObject(this, &UScoreDisplayWidget::UpdateScore);
	}

	APyramidShooterGameState* PyramidShooterGameState = Cast<APyramidShooterGameState>(GameWorld->GetGameState());

	if (PyramidShooterGameState != nullptr)
	{
		PyramidShooterGameState->OnGameFinished().AddUObject(this, &UScoreDisplayWidget::DisplayPlayersScoreboard);
	}

	ScoreBoardTitle->SetVisibility(ESlateVisibility::Hidden);
}

void UScoreDisplayWidget::UpdateScore(int InScore)
{
	PlayerScoreContent->SetText(FText::FromString(FString::FromInt(InScore)));
}

void UScoreDisplayWidget::DisplayPlayersScoreboard()
{
	if (ScoreboardRowWidgetClass != nullptr)
	{
		APyramidShooterGameState* GameState = Cast<APyramidShooterGameState>(GetWorld()->GetGameState());
		TArray<FScoreboardPlayerInformation> ScoreboardInformation;

		ScoreBoardTitle->SetVisibility(ESlateVisibility::Visible);

		if (GameState != nullptr)
		{
			ScoreboardInformation = GameState->GetPlayerScoreboardInformation();

			for (const FScoreboardPlayerInformation& RowInformation : ScoreboardInformation)
			{
				UScoreboardRowWidget* ScoreboardRowWidget = CreateWidget<UScoreboardRowWidget>(GetWorld(), ScoreboardRowWidgetClass);

				if (ScoreboardRowWidget != nullptr)
				{
					ScoreboardRowWidget->SetScoreboardRowInformation(RowInformation);
					ScoreboardContent->AddChild(ScoreboardRowWidget);
				}
			}
		}
	}
}
