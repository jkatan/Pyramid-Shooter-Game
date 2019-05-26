#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PyramidShooterGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FGameFinishedDelegate)

/**
 * 
 */

USTRUCT()
struct FScoreboardPlayerInformation
{
	GENERATED_USTRUCT_BODY()

	FString PlayerName;

	int Score;
};

UCLASS()
class PYRAMIDSHOOTER_API APyramidShooterGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	APyramidShooterGameState();

	void NotifyPyramidCubeDestruction();

	void FinishGame();

	bool IsGameFinished() const;

	TArray<FScoreboardPlayerInformation> GetPlayerScoreboardInformation() const;

	FGameFinishedDelegate& OnGameFinished();

private:
	int PyramidCubesInMapQuantity;

	UPROPERTY(ReplicatedUsing = OnRep_bGameFinished)
	bool bGameFinished;

	FGameFinishedDelegate GameFinishedEvent;

	TArray<FScoreboardPlayerInformation> PlayerScoreboardInformation;

	UFUNCTION()
	void OnRep_bGameFinished();

	void FillPlayerScoreboardInformation();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
