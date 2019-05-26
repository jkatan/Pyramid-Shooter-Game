#include "PyramidCube.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "PyramidShooterGameMode.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "PyramidShooterGameState.h"

// Sets default values
APyramidCube::APyramidCube()
{
	PyramidCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PyramidCubeMesh"));
	RootComponent = PyramidCubeMesh;

	CreateCubeCollider();
	CreateMaterialsForRandomColors();

	//Default color material index
	ColorMaterialIndex = 0;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APyramidCube::BeginPlay()
{
	Super::BeginPlay();	

	if (Role == ROLE_Authority)
	{
		ColorMaterialIndex = FMath::FRandRange(0, 3);
	}

	PyramidCubeMesh->SetMaterial(0, RandomColorsToAssign[ColorMaterialIndex]);
}

void APyramidCube::CreateCubeCollider()
{
	//Use a UBoxComponent as a trigger to detect other adyacent cubes
	PyramidCubeCollider = CreateDefaultSubobject<UBoxComponent>("PyramidCubeCollider");
	PyramidCubeCollider->InitBoxExtent(FVector(60.0f, 60.0f, 60.0f));
	PyramidCubeCollider->SetGenerateOverlapEvents(true);
	PyramidCubeCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PyramidCubeCollider->SetCollisionProfileName("Trigger");
	PyramidCubeCollider->SetupAttachment(RootComponent);
}

void APyramidCube::CreateMaterialsForRandomColors()
{
	RandomColorsToAssign.SetNum(3);

	RandomColorsToAssign[0] = CreateDefaultSubobject<UMaterial>(TEXT("Color0"));
	RandomColorsToAssign[1] = CreateDefaultSubobject<UMaterial>(TEXT("Color1"));
	RandomColorsToAssign[2] = CreateDefaultSubobject<UMaterial>(TEXT("Color2"));
}

void APyramidCube::DestroyOnCascade(APyramidShooterCharacter* PlayerWhoShotTheCube)
{
	DestroyOnFibonacciCascade(0, 1, PlayerWhoShotTheCube);
}

void APyramidCube::DestroyOnFibonacciCascade(int PreviousFibonacciTerm, int CurrentFibonacciTerm, APyramidShooterCharacter* PlayerWhoShotTheCube)
{
	PlayerWhoShotTheCube->IncrementPlayerScore(CurrentFibonacciTerm);

	// Get adyacent cubes
	TArray<AActor*> AdyacentCubes;
	GetOverlappingActors(AdyacentCubes, TSubclassOf<APyramidCube>());

	// Disable collision with other cubes to avoid being called from another DestroyOnFibonacciCascade call
	SetActorEnableCollision(false);

	int NextFibonacciTerm = CurrentFibonacciTerm + PreviousFibonacciTerm;

	// Call DestroyOnFibonacciCascade on all adyacent cubes
	ExecuteFibonacciCascadeDestruction(CurrentFibonacciTerm, NextFibonacciTerm, PlayerWhoShotTheCube, AdyacentCubes);

	// Effectively destroy this cube
	Destroy();
	NotifyGameStateOfCubeDestruction();
}


void APyramidCube::ExecuteFibonacciCascadeDestruction(int PreviousFibonacciTerm, int CurrentFibonacciTerm,
	APyramidShooterCharacter* PlayerWhoShotTheCube, TArray<AActor*> &AdyacentCubes)
{
	for (AActor* Actor : AdyacentCubes)
	{
		APyramidCube* PyramidCube = Cast<APyramidCube>(Actor);

		if (PyramidCube != nullptr && PyramidCube->GetActorEnableCollision() && PyramidCube->ColorMaterialIndex == ColorMaterialIndex)
		{
			PyramidCube->DestroyOnFibonacciCascade(PreviousFibonacciTerm, CurrentFibonacciTerm, PlayerWhoShotTheCube);
		}
	}
}

void APyramidCube::NotifyGameStateOfCubeDestruction() const
{
	UWorld* GameWorld = GetWorld();

	if (GameWorld != nullptr)
	{
		APyramidShooterGameState* GameState = GameWorld->GetGameState<APyramidShooterGameState>();

		if (GameState != nullptr)
		{
			GameState->NotifyPyramidCubeDestruction();
		}
	}
}

void APyramidCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APyramidCube, ColorMaterialIndex);
}
