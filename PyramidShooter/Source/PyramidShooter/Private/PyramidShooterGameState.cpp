#include "PyramidShooterGameState.h"
#include "PyramidShooterGameMode.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "PyramidShooterHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

APyramidShooterGameState::APyramidShooterGameState()
	: Super()
{
	TArray<AActor*> PyramidCubesInMap;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APyramidCube::StaticClass(), PyramidCubesInMap);

	PyramidCubesInMapQuantity = PyramidCubesInMap.Num();

	bGameFinished = false;
}

void APyramidShooterGameState::NotifyPyramidCubeDestruction()
{
	PyramidCubesInMapQuantity--;

	if (PyramidCubesInMapQuantity == 0)
	{
		FinishGame();
	}
}

bool APyramidShooterGameState::IsGameFinished() const
{
	return bGameFinished;
}

void APyramidShooterGameState::FinishGame()
{
	bGameFinished = true;

	FillPlayerScoreboardInformation();
	OnGameFinished().Broadcast();
}

TArray<FScoreboardPlayerInformation> APyramidShooterGameState::GetPlayerScoreboardInformation() const
{
	return PlayerScoreboardInformation;
}

FGameFinishedDelegate& APyramidShooterGameState::OnGameFinished()
{
	return GameFinishedEvent;
}

void APyramidShooterGameState::OnRep_bGameFinished()
{
	FillPlayerScoreboardInformation();
	OnGameFinished().Broadcast();
}

void APyramidShooterGameState::FillPlayerScoreboardInformation()
{
	for (const APlayerState* Player : PlayerArray)
	{
		FScoreboardPlayerInformation PlayerInformation;
		PlayerInformation.Score = Player->Score;
		PlayerInformation.PlayerName = Player->GetName();
		PlayerScoreboardInformation.Add(PlayerInformation);
	}

	PlayerScoreboardInformation.Sort([](const FScoreboardPlayerInformation& A, const FScoreboardPlayerInformation& B) {
		return A.Score > B.Score;
	});
}

void APyramidShooterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APyramidShooterGameState, bGameFinished);
}

