#include "PyramidShooterHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "PyramidShooterCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "PyramidShooterGameState.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "Blueprint/UserWidget.h"
#include "ScoreDisplayWidget.h"

APyramidShooterHUD::APyramidShooterHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> CrosshairDisplayWidgetAsset(TEXT("/Game/UI/BP_CrosshairDisplayWidget"));
	if (CrosshairDisplayWidgetAsset.Succeeded())
	{
		CrosshairDisplayWidgetClass = CrosshairDisplayWidgetAsset.Class;
	}

	static ConstructorHelpers::FClassFinder<UScoreDisplayWidget> ScoreDisplayWidgetAsset(TEXT("/Game/UI/BP_ScoreDisplayWidget"));
	if (ScoreDisplayWidgetAsset.Succeeded())
	{
		ScoreDisplayWidgetClass = ScoreDisplayWidgetAsset.Class;
	}
}

void APyramidShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ScoreDisplayWidgetClass != nullptr) 
	{
		UScoreDisplayWidget* ScoreDisplayWidget = CreateWidget<UScoreDisplayWidget>(GetWorld(), ScoreDisplayWidgetClass);

		if (ScoreDisplayWidget != nullptr)
		{
			ScoreDisplayWidget->AddToViewport();
		}
	}

	if (CrosshairDisplayWidgetClass != nullptr)
	{
		UUserWidget* CrosshairDisplayWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairDisplayWidgetClass);

		if (CrosshairDisplayWidget != nullptr)
		{
			CrosshairDisplayWidget->AddToViewport();
		}
	}
}