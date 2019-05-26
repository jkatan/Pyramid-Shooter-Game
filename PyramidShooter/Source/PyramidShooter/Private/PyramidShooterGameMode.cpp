#include "PyramidShooterGameMode.h"
#include "PyramidShooterHUD.h"
#include "PyramidShooterCharacter.h"
#include "PyramidShooterPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "PyramidShooterGameState.h"
#include "PyramidCube.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

APyramidShooterGameMode::APyramidShooterGameMode()
	: Super()
{
	// set default pawn class to Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	//Custom HUD class
	HUDClass = APyramidShooterHUD::StaticClass();

	//Custom GameState class
	GameStateClass = APyramidShooterGameState::StaticClass();

	//Custom PlayerState class
	PlayerStateClass = APyramidShooterPlayerState::StaticClass();
}
