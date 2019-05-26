#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PyramidShooterHUD.generated.h"

UCLASS()
class APyramidShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	APyramidShooterHUD();

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<class UUserWidget> ScoreDisplayWidgetClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> CrosshairDisplayWidgetClass;
};

